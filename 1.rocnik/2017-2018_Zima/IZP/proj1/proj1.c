#include <stdio.h>
#include <stdbool.h>
#include <string.h>
//#pragma GCC diagnostic ignored "-Wwrite-strings"
//#pragma GCC diagnostic ignored "-Wunused-variable"

/*
* 03.11.2017
* by Benjamín Koša, xkosab00
* Projekt 1 - PRACA S TEXTOM
*/

#define maxCity 91
#define maxLnCity 101

//zmeni male pismenka na velke
char charToCapital (int input){
	if(input >= 'a' && input <= 'z')
		return(input -('a'-'A'));
	else
		return input;
}

//zemni v celom stringu pismenka na velke
char* strToCapital(char* input){
	int len = strlen(input);
	for(int i = 0;i <len; i++)
        input[i] = charToCapital(input[i]);
    return input;
}


void swap(char *pa, char *pb){
	char pom = *pa; 
	*pa = *pb; 
	*pb = pom;
}


char* bubleSort(char* input){
	int finish;
	int len = strlen(input)-1;
	do{
	finish = true;
	for(int i = 0; i < len; i++)
		if(input[i] > input[i+1]){	
			swap(&input[i],&input[i+1]);
			finish = false;
		}
	}while(finish != true);
	return input;
}

//posunie pole znakov o 1 dolava s tym ze o znak na pozicii start prideme
char* moveArray(char* input, int start){
	int len = strlen(input);
	for(int i = start; i < len;i++){
		input[i] = input[i+1];
		len = strlen(input);
	}
	return input;
}

//ked najde v usporiadanom poli dva rovnake znaky tak jeden zmaze az kym neostane po kazdom znaku 1 ks
char* deleteDuplicate(char* input){
	int len = strlen(input);
	for(int i = 0; i < len;i+=1)
		if(input[i] == input[i+1]){
			input = moveArray(input, i);
			i = -1;
			len = strlen(input);
		}
	return input;

}

int errOverflow(int index, int capacity){
	if(index >= capacity){
    	fprintf(stderr, "%s\n", "Error: array overflow!\n");
    	return 1;
    }
    return 0;
}


//--------EVERITHING RELATED TO CITIES--------//

//v tejto strukture su ulozene mesta
struct cities {
	char name[maxLnCity];
	int used;
	int active;
	int strlen;
};

//vytvori sablonu struktúry a vratí sablonu
struct cities createCity(char cityName[maxLnCity], int strlen){
    struct cities entity;
    entity.strlen = strlen;
    entity.active = true;
    entity.used = true;

    for(int i = 0; i <= entity.strlen;i++)
        entity.name[i] = cityName[i];
    entity.name[entity.strlen] = '\0';

    return entity;
}

//v aktivnych mestach, ak nenajde zhodu medzi znakom tak nastavy mesto na neaktivne
void turnOffInappropriateCity (struct cities (*entity)[maxCity], char *searchLetters){    
    int len = strlen(searchLetters);
    for(int j = 0;j < len; j++)  
        for(int i = 0;(*entity)[i].used == true ; i++) 		
    		if((*entity)[i].active == true)
    		    if((*entity)[i].name[j] != searchLetters[j])
    		        (*entity)[i].active = false;
}

//spocita aktivne mesta
int countActiveCity(struct cities entity[]){
	int countActive = 0;
	for(int i = 0;entity[i].used == true ; i++)
        if(entity[i].active == true)
        	countActive++;
return countActive;
}

//zapíše nové mesto do pola štruktúr, ak pri procese nastane chyba tak vrati -1 else 0
int loadData (struct cities (*entity)[maxCity]){
	char name[maxLnCity];	//priebezne uklada nazvo mesta do tejto premnnej
    int wLength = -1;	//pocita dlzku slova
    int id = 0;	//id mesta

	do {
        wLength+=1; 
        if(errOverflow(wLength, maxLnCity)) 
            return 1;
        //vytahuje pismenka zo suboru a uklada ich do name
        name[wLength] = charToCapital(getchar());
    
    switch(name[wLength]){
        //ked narazi na koniec slova vytvori novu strukturu
        case '\n':
            (*entity)[id] = createCity(name, wLength);
            id++;
            if(errOverflow(id, maxCity))
            	return 1;
            //zacne počitat dlzku slova od znova
            wLength = -1;
            break;
        //ked narazy na eof && nenachadza sa na zaciatku riadka tak vytvori novu strukturu
        case EOF: 
            if(wLength!=0)
                (*entity)[id] = createCity(name, wLength);
            break;
		}

	} while (name[wLength] != EOF);
	return 0;
}

//ked spocita mesta a vysledok == 1 tak zavolá tuto fukciu kora to mesto najde
void findOnlyActiveCity(struct cities entity[]){
	for(int i = 0;entity[i].used == true ; i++)
        if(entity[i].active == true)
        	printf("Found: %s\n", entity[i].name);
}

//ked spocita mesta a vysledok > 1 tak sa zavola tato funkcia najde pismena pre vystup "Enable: pismena"
void findEnableLetters(struct cities entity[], int order){
	char outputLetters[maxLnCity];
	int count = 0;	
	
	//cyklus vybera struktury ktore sa pouzivaju
	for(int i = 0;entity[i].used == true ; i++)
        if(entity[i].active == true){
        	//ak narazy na znak '\0' tak to znamena ze nasiel cele mesto a vypise ho
        	if(entity[i].name[order] == '\0'){
        		printf("Found: %s\n", entity[i].name);
        	}else{
        		//uklada pismena pre vystup
        		outputLetters[count] = entity[i].name[order];
        		count++;    	
        	}
        }
    //na koniec slova prida znak '\0'
    outputLetters[count] = '\0';
    
    char* strOutputLetters = bubleSort(outputLetters);
    strOutputLetters = deleteDuplicate(strOutputLetters);
    printf("Enable: %s\n", strOutputLetters);
}

//podla toho kolko je aktivnych miest vola funkcie a vypisuje vystup
void analyzeData(struct cities entity[], char* inputLetters){
	switch(countActiveCity(entity)){
		case 0:
			printf("Not found\n");
			break;
		case 1:		//Found:
			findOnlyActiveCity(entity); 
			break;
		default:	//enable
			findEnableLetters(entity, strlen(inputLetters));
			break; 
	}
}

//--------END OF EVERITHING RELATED TO CITIES--------//

int main(int argc, char *argv[]){
    //v strukture "City" budu ulozene vsetky mestá
    struct cities City[maxCity];
    
    for(int i = 0; i <= maxCity; i++)
        City[i].used = false;

    //nacita mesta z file.txt do pola struktur, -1 vrati pri preteceni pola
    if(loadData(&City)){
    	return 1;
    }

    //ak nieje zadany argument tak vypise zaciatocne pismena vsetkych miest
    if(argc == 1){
    	findEnableLetters(City, 0);
        return 0;
    }

    char *searchLetters = strToCapital(argv[1]);
    //nevyhovujucim mestam nastavi parameter used na false
    turnOffInappropriateCity(&City, searchLetters);

    //zanalizuje pole struktur miest a vykona vystup
    analyzeData(City, searchLetters);

	return 0;
}