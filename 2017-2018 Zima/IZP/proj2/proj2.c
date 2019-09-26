#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/** 05.11.2017
  * by Benjamín Koša, xkosab00
  * Projekt 2 - ITERACNI VYPOCTY
  */

/** @return absolutna hodnotu z inpit
  */
double absolute_value(double input){
	if(input < 0)
		return input * -1;
	return input;
}

////////////////EVERITHING RELATED TO DETECTIONS & ERRORS////////////////

/** Otestuje ci je @input z daneho intervalu od @m do @n
  * @return 1 - error/0 - correct 
  */
int check_range(double input, double m, double n){
	if (input <= m || input > n){
		fprintf(stderr, "01.Error %.01f != ( %.01f, %.01f>\n", input, m, n);
		return 1;
	}else{ 
		return 0;
	}
}

/** Otestuje ci je dostatok argumentov na bezpecne vykonanie príkazu
  * @actual_id + @range == pocet argumento ktory potrebujeme
  * @argc == realny pocet argumentov
  * @write_errors == 1/0 nastavuje ci ma alebo nema vypisovat errory
  * @return 1 - error/0 - correct 
  */
int is_argument(int actual_id, int range, int argc, int write_errors){
	if(actual_id + range >= argc){
		if(write_errors)
			fprintf(stderr, "04.Error: missing value!\n");
		return 1;
	}
	return 0;
}


////////////////EVERITHING RELATED TO TANGS////////////////
/** Vypocita tangens podla taylora
  * @return tangens [rad]
  */
double taylor_tan(double x,  unsigned int n){
	double tan = 0;
	double exponent = x;
	long long int citatel[] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604};
	long long int menovatel[] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
	
	for (unsigned int i = 0; i <= n-1; i++){
		tan += ((citatel[i]*exponent) / menovatel[i]);
		exponent *= x * x;
	}
	return tan;
}

/** Vypocita tangens pomocou vnorenych zlomkov
  * @return tangens [rad]
  */
double cfrac_tan(double x, unsigned int n){
	double cf = 0.;
	double fb = x*x;
	double fa;
	for (unsigned int k = n; k > 1; k--){	
		fa = 2*k-1;
		cf = fb/(fa - cf);
	}
	return x/(1-cf);
}

////////////////EVERITHING RELATED TO analyze_arguments////////////////
void print_help(){
	printf("%s\n", "*******************************************************************************");
	printf("%s\n", "PROGRAM SE SPOUSTI V NASLEDUJICI PODOBE: ");
	printf("%s\n", "./proj2 --tan A N M ");
	printf("%s\n\n", "./proj2 [-c X] -m A [B]");
	printf("%s\n", "ARGUMENTY PROGRAMU:");
	printf("%s\n", "--tan: porovna presnosti vypoctu tangens uhlu A (v radianech) mezi volanim"); 
	printf("%s\n", "  * porovna presnosti vypoctu tangens uhlu A (v radianech) mezi volanim tan");
	printf("%s\n", "    z matematicke knihovny, a vypoctu tangens pomocy Taylorova polynomu");
	printf("%s\n", "    a zretezeneho zlomku. Argumenty N a M udavaji, ve kterych iteracich ");
	printf("%s\n", "    iteracniho vypoctu ma porovnani probihat. 0 < N <= M < 14");
	printf("%s\n", " -m: vypocita a zmeri vzdalenosti.");
	printf("%s\n", "     * Uhel alfa je dan argumentem A v radianech. Program vypocita a vypise ");
	printf("%s\n", "       vzdalenost mereneho objektu. 0 < A <= 1.4 < pi/2.");
	printf("%s\n", "     * Pokud je zadan, uhel beta udava argument B v radianech. Program vypocita");
	printf("%s\n", "       a vypise i vysku mereneho objektu. 0 < B <= 1.4 < pi/2");
	printf("%s\n", "   -c: nastavuje vysku mericiho pristroje c pro vypocet. Vyska c je");
	printf("%s\n", "       dana argumentem X (0 < X <= 100). Argument je volitelny - implicitni");
	printf("%s\n", "       vyska je 1.5 metru");
	printf("%s\n", "*******************************************************************************");
}

/** Vykona prikaz --tan (popiseane v print_help())
  */
void mm_tan(double x, int m, int n){
	for (int i = m; i <= n; ++i){
		double rTaylorTan = taylor_tan(x, i);
		double rCfracTan = cfrac_tan(x, i);
		double mathTan = tan(x);
		double diffTaylor = mathTan - rTaylorTan;
		double diffCfrac = mathTan - rCfracTan;
		diffTaylor = absolute_value(diffTaylor);
		diffCfrac = absolute_value(diffCfrac);

		printf("%d. %e %e %e %e %e\n",i, mathTan, rTaylorTan, diffTaylor, rCfracTan, diffCfrac);
	}
}

/** vypocita vzdialenosť od predmetu (popiseane v print_help())
  */
double m_M_dist(double a, double device_height){
	double alfa = cfrac_tan(a, 9);	//po 9 iteraciach je cislo dostatocne presne
	double distance = device_height / alfa;
	printf("%.10e\n", distance);
	return distance;
}

/** vypocita výšku od predmetu (popiseane v print_help())
  */
void m_M_height(double distance, double b, double device_height){
	double beta = cfrac_tan(b, 9);
	double height = distance * beta + device_height;
	printf("%.10e\n", height);
}

//////////////// MAIN ////////////////
int main(int argc, char *argv[]){
	if(argc == 1){
		fprintf(stderr, "00.Error: missing argument!\n");	
		return 1;
	}
	double device_height = 1.5;

	int count = 0; //premenna pociat kolko argumentov vytvorilo akciu v analyze_arguments
	char *ep1, *ep2, *ep3;	//sluzi na detekovanie chyb pr prevode str to double
	for(int i = 0; i < argc; i++)
		if(!strcmp(argv[i], "--help")){
			print_help();
			return 0;

		}else if(!strcmp(argv[i], "--tan")){
			if(is_argument(i, 3, argc, 1))
				return 1;
			int m = strtod (argv[i+2], &ep1);
			int n = strtod (argv[i+3], &ep2);
			double x = strtod (argv[i+1], &ep3);
			if(*ep1 || *ep2 || *ep3){
				fprintf(stderr, "05.Error when do %s\n", argv[i]);
				return 1;
			}
			if(check_range(m, 0, 13) || check_range(n, 0, 13))
				return 1;
			if(m > n){
				fprintf(stderr, "05.Error: %d > %d\n", m, n);
				return 1;
			}
			mm_tan(x, m, n);

		}else if(!strcmp(argv[i], "-c")){
			if(is_argument(i, 1, argc, 1))
				return 1;
			double height = strtod(argv[i+1], &ep1);
			if(*ep1){
				fprintf(stderr, "05.Error when do %s\n", argv[i]);
				return 1;
			}
			if(check_range(height , 0, 100))
				return 1;
			device_height = height;
			count++;

		}else if(!strcmp(argv[i], "-m")){
			if(is_argument(i, 1, argc, 1))
				return 1;
			double alfa = strtod(argv[i+1], &ep1);
			if(*ep1){
				fprintf(stderr, "05.Error when do %s\n", argv[i]);
				return 1;
			}
			if(check_range(alfa, 0, 1.4))
				return 1;
			double distance = m_M_dist(alfa, device_height);

			if(is_argument(i+1, 1, argc, 0))
				continue;	//continue pretoze vyysledok nieje chyba, 2. argument pre -m je dobrovolny
			double beta = strtod(argv[i+2], &ep1);
			if(*ep1)
				continue;
			if(check_range(beta, 0, 1.4))
				return 1;
			m_M_height(distance, beta,  device_height);
		}else
			//ak nenastala akcia tak pripocita 1
			count++;
	
	if(count ==  argc){	//ak v analyze_arguments nenastala ziadna akcia - error
		fprintf(stderr, "06.Error: wrong inpur!\n");
		return 1;
	}
	return 0;
}