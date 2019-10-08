/*
 * FILE: proj2.c
 * AUTOR: BENJAMIN KOSA
 * LOGIN: XKOSAB00
 * DATUM: 26.4.2018
 * PROJEKT: THE SENATE BUS PROBLEM
 * PREDMET: IOS
 */

#include "proj2.h"

/********************************************************************/
/*                           DEKLARACIE                             */
/********************************************************************/

FILE *file = NULL;

//shared memory pointers
int *process_number = NULL,  //cislo vykonavanej akcie
    *onboard_riders = NULL,    //pocet ludi v autobuse
    *killed_riders = NULL,  //pocet odvezenych  ludi
    *onStation_riders = NULL; //pocet ludi na stanici

int shm_process_number = 0,
    shm_onboard_riders = 0,
    shm_killed_riders = 0,
    shm_onStation_people = 0;

//semafors
sem_t
    *sem_writing,  //pristup k suboru
    *sem_load,  //povoluje nastupovanie
    *sem_unload,    //povoluje vystupovanie
    *sem_unload_finish,    //signalizuje ukoncenie nastupovania
    *sem_full,  //signalizuje plny vozik
    *sem_depart;    /*zabezpecuje ze sa moze nastupovat len ked 
                    autobus nieje na stanici*/

//process
pid_t pid_rider,
      pid_bus,
      pid_mainProc;


/********************************************************************/
/*                      HLAVNA LOGIKA PROGRAMU                      */
/********************************************************************/


void rider(Tparam param, int id){
    sem_wait(sem_writing);
        fprintf(file,"%d\t: RID %d\t: start\n",(*process_number)++,id);
        fflush(file);
    sem_post(sem_writing);

    sem_wait(sem_depart);
        sem_wait(sem_writing);
            fprintf(file,"%d\t: RID %d\t: enter: %d \n",(*process_number)++, id, ++(*onStation_riders));
            fflush(file);
        sem_post(sem_writing);   
    sem_post(sem_depart);

    sem_wait(sem_load);     //caka na povolenie nastupovat

    sem_wait(sem_writing);
        fprintf(file,"%d\t: RID %d\t: boarding\n",(*process_number)++,id);
        fflush(file);

        (*onboard_riders)++;
        (*onStation_riders)--;
        (*killed_riders)++;
    sem_post(sem_writing);
    
    //System nastupovania
    if((*onboard_riders) == param.C || (*onStation_riders) == 0) //nastupil posledny
        sem_post(sem_full);
    else
        if((*onboard_riders) < param.C)    //nastupil 
            sem_post(sem_load);

    //system vystupovania
    sem_wait(sem_unload);
    (*onboard_riders)--;

    if((*onboard_riders) > 0) { 
        sem_post(sem_unload);
    }else
        sem_post(sem_unload_finish);

    sem_wait(sem_writing);
        fprintf(file,"%d\t: RID %d\t: finish\n",(*process_number)++,id);
        fflush(file);
    sem_post(sem_writing); 

    exit(0);
}

void bus(Tparam param) {

    sem_wait(sem_writing);
        fprintf(file,"%d\t: BUS \t: start\n",(*process_number)++);
        fflush(file);
    sem_post(sem_writing);


    while(*killed_riders != param.R) {
        //zaistuje ze sa moze vstupit na stanicu len ked autobus nieje na stanici
        sem_wait(sem_depart);
            sem_wait(sem_writing);
                fprintf(file,"%d\t: BUS \t: arrival\n",(*process_number)++);
                fflush(file);
            sem_post(sem_writing);

            if((*onStation_riders) > 0) {
                sem_wait(sem_writing);
                    fprintf(file,"%d\t: BUS \t: start boarding: %d \n",(*process_number)++, (*onStation_riders));
                    fflush(file);
                sem_post(sem_writing);

                sem_post(sem_load);     //mozete nastupovat
                sem_wait(sem_full);     //vsetci nastupeny

                sem_wait(sem_writing);
                    fprintf(file,"%d\t: BUS \t: end boarding: %d \n",(*process_number)++, (*onStation_riders));
                    fflush(file);
                sem_post(sem_writing);
            }
            
            sem_wait(sem_writing);
                fprintf(file,"%d\t: BUS \t: depart\n",(*process_number)++);
                fflush(file);
            sem_post(sem_writing);
        sem_post(sem_depart);

        //simulacia jazdy
        if(param.ABT != 0){
            int run_time = (random() % (param.ABT + 1));
            usleep(run_time * 1000);
        }

        sem_wait(sem_writing);
            fprintf(file,"%d\t: BUS \t: end\n",(*process_number)++);
            fflush(file);
        sem_post(sem_writing);
        
        //spusti vystupovanie a caka kym vsetci nastupili
        if(*onboard_riders > 0){
            sem_post(sem_unload);  //mozete vystupit
            sem_wait(sem_unload_finish); //vseci mrtvy
        }
    }
    sem_wait(sem_writing);
        fprintf(file,"%d\t: BUS \t: finish\n",(*process_number)++);
        fflush(file);
    sem_post(sem_writing);
    
    exit(0);
}

int main(int argc, char* argv[]) {
    Tparam param; // tu sa ulozia argumenty

    //zapne generovanie
    srand(time(NULL) * getpid());

    if(load_parameters(argc, argv, &param) == PAR_ERROR) 
        input_error();

    pid_t bus_pid;
    pid_t riders[param.R];
    pid_t main_process = 0;

    signal(SIGTERM, emergency_end);
    signal(SIGINT, emergency_end);

    // otvorenie suboru
    if((file = fopen("proj2.out", "w+")) == NULL) {
        fprintf(stderr, "[E.01]: opening proj2.out gone wrong\n");
        return EXIT_FAILURE;
    }
    setbuf(file, NULL);

    //alokacia
    if(allocation()) {
        fprintf(stderr, "[E.02]: Allocation error\n");    
        free_allocation();
        return(EXIT_FAILURE);
    }

    *process_number = 1;
    *onboard_riders = 0;

    //fork main
    pid_mainProc = fork();
    if (pid_mainProc < 0) {
        fprintf(stderr, "[E.03]: fork main error.\n");
        emergency_end();
    } else if (pid_mainProc == 0) {
        //fork bus
        pid_bus = fork();

        if (pid_bus < 0) {
            fprintf(stderr, "[E.04]: fork bus error.\n");
            emergency_end();
        }
        else if(pid_bus == 0) 
            bus(param);
        else 
            bus_pid = pid_bus;
         
        waitpid(bus_pid, NULL, 0);
    } else {   
        for(int id=1; id <= param.R; id++){ 
            if(param.ART != 0) { //generuje delay medzi generovanim riderov
                int random_time = (random() % (param.ART + 1)); 
                usleep(random_time * 1000); //prevod micro na mili s
            }
            //fork rider
            pid_rider = fork();
            if(pid_rider < 0) {
                fprintf(stderr, "[E.05]: fork rider error\n");
                emergency_end();

            } else if (pid_rider == 0) 
                rider(param,id);    
            else 
                riders[id] = pid_rider;
        }
        // cakanie na ukoncenie riders
        for(int id = 0; id < param.R; id++)
                waitpid(riders[id], NULL, 0);

        main_process = pid_mainProc;
        waitpid(main_process, NULL, 0);
    }

    free_allocation();
    return(ET_OK);
}


/********************************************************************/
/*                           POMOCNE FUNKCIE                        */
/********************************************************************/


int load_parameters(int argc, char *argv[], Tparam *tmp_param) {

    if(argc != 5)
        return PAR_ERROR;
      
    if(isdigit(*argv[1]) && isdigit(*argv[2]) && isdigit(*argv[3]) && isdigit(*argv[4])){
        tmp_param->R=strtol(argv[1], NULL, 10);
        tmp_param->C = strtol(argv[2], NULL, 10);
        tmp_param->ART=strtol(argv[3], NULL, 10);
        tmp_param->ABT = strtol(argv[4], NULL, 10);
    }else
        return PAR_ERROR;

    if (tmp_param->R <= 0) input_error();
    if (tmp_param->C <= 0) input_error();
    if ((tmp_param->ART < 0) || (tmp_param->ART > 1000)) input_error();
    if ((tmp_param->ABT < 0) || (tmp_param->ABT > 1000)) input_error();

    return ET_OK;
}

void input_error(void) {
    fprintf(stderr, "[E.06]: error when loading parameters.\n");
    exit(PAR_ERROR);
}

int allocation() {
    //semafory
    if((sem_writing = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) return SYS_ERROR;
    if((sem_load = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) return SYS_ERROR;;
    if((sem_unload = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) return SYS_ERROR;
    if((sem_unload_finish = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) return SYS_ERROR;
    if((sem_full = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) return SYS_ERROR;
    if((sem_depart = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) return SYS_ERROR;

    if(sem_init(sem_writing, 1, 1) == -1) return SYS_ERROR;
    if(sem_init(sem_load, 1, 0) == -1) return SYS_ERROR;
    if(sem_init(sem_unload, 1, 0) == -1) return SYS_ERROR;
    if(sem_init(sem_unload_finish, 1, 0) == -1) return SYS_ERROR;
    if(sem_init(sem_full, 1, 0) == -1) return SYS_ERROR;
    if(sem_init(sem_depart, 1, 1) == -1) return SYS_ERROR;

    //zdielana pamat
    if((shm_process_number = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1) return SYS_ERROR;
    if((shm_onboard_riders = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1) return SYS_ERROR;
    if((shm_killed_riders = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1) return SYS_ERROR;
    if((shm_onStation_people = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1) return SYS_ERROR;

    if((process_number = (int *) shmat(shm_process_number, NULL, 0)) == NULL) return SYS_ERROR;
    if((onboard_riders = (int *) shmat(shm_onboard_riders, NULL, 0)) == NULL) return SYS_ERROR;
    if((killed_riders = (int *) shmat(shm_killed_riders, NULL, 0)) == NULL) return SYS_ERROR;
    if((onStation_riders = (int *) shmat(shm_onStation_people, NULL, 0)) == NULL) return SYS_ERROR;
    
    return ET_OK;
}

void free_allocation(void) {
    int noError = ET_OK;
    
    if(fclose(file)) noError = SYS_ERROR;

    // uvolnenie semaforov
    if(sem_destroy(sem_writing) == -1) noError = SYS_ERROR;
    if(sem_destroy(sem_load) == -1) noError = SYS_ERROR;
    if(sem_destroy(sem_unload) == -1) noError = SYS_ERROR;
    if(sem_destroy(sem_unload_finish) == -1) noError = SYS_ERROR;
    if(sem_destroy(sem_full) == -1) noError = SYS_ERROR;
    if(sem_destroy(sem_depart) == -1) noError = SYS_ERROR;
    
    //uvolnenie zddielanej pamate
    if(shmctl(shm_process_number, IPC_RMID, NULL) == -1) noError = SYS_ERROR;
    if(shmctl(shm_onboard_riders, IPC_RMID, NULL) == -1) noError = SYS_ERROR;
    if(shmctl(shm_killed_riders, IPC_RMID, NULL) == -1) noError = SYS_ERROR;
    if(shmctl(shm_onStation_people, IPC_RMID, NULL) == -1) noError = SYS_ERROR;

    if(shmdt(process_number) == -1) noError = SYS_ERROR;
    if(shmdt(onboard_riders) == -1) noError = SYS_ERROR;
    if(shmdt(killed_riders) == -1) noError = SYS_ERROR;
    if(shmdt(onStation_riders) == -1) noError = SYS_ERROR;

    if(noError) {
        fprintf(stderr, "[E.07]: error when free memory\n");
        exit (SYS_ERROR);
    }
}

void emergency_end() {
    free_allocation();
    kill(pid_mainProc, SIGTERM);
    kill(getpid(), SIGTERM);
    exit(SYS_ERROR);
}
