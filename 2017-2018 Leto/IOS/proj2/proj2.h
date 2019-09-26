/*
 * FILE: proj2.h
 * AUTOR: BENJAMIN KOSA
 * LOGIN: XKOSAB00
 * DATUM: 26.4.2018
 * PROJEKT: THE SENATE BUS PROBLEM
 * PREDMET: IOS
 */

#include <ctype.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ET_OK 0
#define PAR_ERROR 1
#define SYS_ERROR 2


//parameters struct
typedef struct parameters
{
    int R; // pocet riderov
    int C; // kapacita autovusu
    int ART; // max cas generovania ridera
    int ABT; // max dlzka jazdy 
} Tparam;


int load_parameters(int argc, char *argv[], Tparam *tmp_param);//nacita parametre
void emergency_end() ;     //bezpecne ukonci program
void input_error(void);    //vypisuje chybu v parametrch
int allocation();      //alokacia zdielanej pamate a semaforov
void bus(Tparam param);     //logika autobusu
void rider(Tparam param, int i);    //logika jazdcov
void free_allocation(void);      //uprace po fukcii allocation