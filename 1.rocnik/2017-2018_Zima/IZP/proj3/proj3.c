/**
 * 3. projekt IZP 2017/18
 *
 * Benjamin Kosa, xkosab00
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h> // strcmp

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO //ak cap null tka nevolat malloc
    c->size = 0;
    c->capacity = cap;
    c->obj = malloc(cap * sizeof(struct obj_t));
    if(c->obj == NULL){
    	c->capacity = 0;
    	fprintf(stderr,"01: error in init_cluster, alloc problem!\n");
    }

}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
    free(c->obj);
    c->obj = NULL;
    
    c->capacity = 0;
    c->size = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO

    if(c->size >= c->capacity)
        if(resize_cluster(c, c->capacity + CLUSTER_CHUNK) == NULL)
    		fprintf(stderr,"02: error in append_cluster, alloc problem!\n");

    c->obj[c->size] = obj;
    c->size++;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
    for(int i = 0; i < c2->size; i++)
		append_cluster(c1, c2->obj[i]);
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO
    clear_cluster(&carr[idx]);
    for(int i = idx; i < narr - 1; i++)
    	carr[i] = carr[i + 1];
    return narr - 1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO
    float a = (o1->x - o2->x) * (o1->x - o2->x);
    float b = (o1->y - o2->y) * (o1->y - o2->y);

    return sqrtf(a + b);
}

/*
 Pocita vzdalenost dvou shluku.
*/
int premium_case = 0;
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO
    float sum = 0;
    float max = 0;
    float min = obj_distance(&c1->obj[0], &c2->obj[1]);
    float distance;

    for(int i = 0; i < c1->size; i++)
    	for(int j = 0; j < c2->size; j++){
    		distance = obj_distance(&c1->obj[i], &c2->obj[j]);
    		switch (premium_case){
    			case 1:
    				if(max < distance)
    					max = distance;
    				break;
    			case 2:
    				if(min > distance)
    					min = distance;
    				break;
    			default:
    				sum += distance;
    		}
		}
	
    switch (premium_case){
    	case 1:
    		return max;
    	case 2:
    		return min;
    	default:
    		return sum / (c1->size*c2->size);
    }
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO
    float dist = 0;
    float min = cluster_distance(&carr[0], &carr[1]);

    for(int i = 0; i < narr; i++)
      for(int j = i+1; j < narr; j++){
        dist = cluster_distance(&carr[i], &carr[j]);
        if(min > dist){
          min = dist;
          *c1 = i;
          *c2 = j;
        }
      }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Uvolni cluster_t od [0] po [narr]
*/
void free_clust(struct cluster_t **c, int narr){
	for(int i = 0; i < narr; i++)
        clear_cluster(&(*c)[i]);

    if(narr != 0)
    	free(*c);
    *c = NULL; 
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
#define MAX_LINE 100
#define MAX_NMB 10
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO
    FILE *in_file;

    if((in_file = fopen(filename, "r")) == NULL){
        fprintf(stderr, "01.Error, could not open file %s.\n", filename);
        *arr = NULL;
        return 0;
    }

    int count;
    char charID[MAX_NMB], charX[MAX_NMB], charY[MAX_NMB];
    char *ep1, *ep2, *ep3;	//pre errory pri prevode str to double

    if(!fscanf(in_file, "count=%d", &count)){
        printf("02.Error wrong document format\n");
        *arr = NULL;
        return 0;
    }

    *arr = malloc(count *sizeof(struct cluster_t));
    if(*arr == NULL){
    	free(*arr);
        fprintf(stderr, "04.Error malloc failure\n");
        return 1;
    }
    
    char line[MAX_LINE];//na ulozenie celeho riadka
    fgets(line, sizeof line, in_file);	//preskoci prvy riadok
    int i = 0;
    while (fgets(line, sizeof line, in_file) != NULL) {
        if(sscanf(line, "%10s %10s %10s", charID, charX, charY) < 3){//z riadka vybere 3 cisla
            fprintf(stderr, "00.Error wrong document format on line: %s\n", line);
            free_clust(arr, i+1);
            return 1;
        }

        init_cluster(&(*arr)[i], 1);
        if(&(arr)[i] == NULL){
            fprintf(stderr, "05.Error malloc failure\n");
            free_clust(arr, i+1);
            return 1;
        }

        (*arr)[i].obj[0].id = strtod (charID, &ep1);	//[0] lebo prvy obj sa uklada vždy na nulty index
        (*arr)[i].obj[0].x = strtod (charX, &ep2);
        (*arr)[i].obj[0].y = strtod (charY, &ep3);
        if((*arr)[i].obj[0].x > 1000 || (*arr)[i].obj[0].y > 1000){
            fprintf(stderr, "13.Error %s is out of range", line);
            free_clust(arr, i+1);
            return 1;
        }
        (*arr)[i].size++;

        if(*ep1 || *ep2 || *ep3){
            fprintf(stderr, "03.Error wrong document format on line: %s\n", line);
            free_clust(arr, i+1);
            return 1;
        }
        
        if(++i >= count)
            break;	//ak je v dokumente viac riadkov ako count= tak vyskoci z cyklu 
    }

    if(fclose(in_file) == EOF){
        fprintf(stderr, "06.Error fclose failure\n");
        free_clust(&(*arr), count);
        return 1;
    }

    if(i < count){
        fprintf(stderr, "07.Error lack of clustures in %s \n", filename);
        free_clust(&(*arr), count);
        return 1;
    }
    return count;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[]){
	struct cluster_t *clusters;
	//osternenie vstupov z argv
	int required_clust = 1;

	if(argc < 2 || argc > 4){
        fprintf(stderr, "08.Error wrong input");
        return 1;
    }

    if(argc >= 3){
    	char *ep;
    	required_clust = strtod (argv[2], &ep);
    	if(*ep || required_clust <= 0){
    		fprintf(stderr, "09.Error wrong input\n");
    		return 1;
    	}
    	if(argc == 4){
    		if(!strcmp(argv[3], "--avg")){
    			premium_case = 0;
			}else if(!strcmp(argv[3], "--max")){
    			premium_case = 1;
			}else if(!strcmp(argv[3], "--min")){
    			premium_case = 2;
			}else{
				fprintf(stderr, "09.Error wrong input\n");
    			return 1;
			}
    	}

    }

    int count_clust = load_clusters(argv[1], &clusters);
    if(clusters == NULL)
    	return 1;
    if(count_clust < required_clust){
    	fprintf(stderr, "10.Error lack of clustures in %s \n", argv[1]);
    	free_clust(&clusters, count_clust);
    	return 1;
    }

    int c1, c2;
    if(required_clust == 1){//ak ma byt vysledna clust == 1 tak iba pospaja vsetky struktury
    	for (int i = 1; i < count_clust; ++i){
    		merge_clusters(&clusters[0], &clusters[i]);
    		if(&clusters[0] == NULL){
    	        fprintf(stderr, "11.Error merge_clusters failure\n");
    	        free_clust(&clusters, count_clust);
    	        return 1;
    	    }
    	    remove_cluster(clusters, count_clust, i);
    	}
    }else
    	while(count_clust > required_clust){
    		find_neighbours(clusters, count_clust, &c1, &c2);
    		merge_clusters(&clusters[c1], &clusters[c2]);
    		if(&clusters[c1] == NULL){
    	        fprintf(stderr, "12.Error merge_clusters failure\n");
    	        free_clust(&clusters, count_clust);
    	        return 1;
    	    }
    	    count_clust = remove_cluster(clusters, count_clust, c2);
    	}
    print_clusters(clusters, required_clust);
	
	free_clust(&clusters, required_clust);
    return 0;
}