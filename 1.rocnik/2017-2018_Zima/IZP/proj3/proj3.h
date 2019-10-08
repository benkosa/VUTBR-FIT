/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2017/18
 * a pro dokumentaci Javadoc.
 * @author Benjamin Kosa
 * @date 2017.12.11
 * @file proj3.h
 * 
 */

/**
 * \defgroup Struktry skupina struktur
 * @{
 */

/** 
 * @struct obj_t
 * podstruktura struktury scluster_c
 * @addtogroup struct
 */
struct obj_t {
    int id;		/**< id id objektu*/
    float x;	/**< x x suradnice v 2D priestore*/	
    float y;	/**< y y suradnice v 2D priestore*/
};

/**
 * @struct cluster_t
 * podstruktura struktury scluster_c
 * @addtogroup struct
 */
struct cluster_t {
    int size;			/**< size id objektu*/
    int capacity;		/**< capacity id objektu*/
    struct obj_t *obj;	/**< obj id objektu*/
};
/**@}*/

/** \addtogroup Deklarace Deklarace
 *  @{
 */
/**
 * Inicializace shluku 'c'.
 * @pre cap sa nesmie rovnat 0
 * @post NULL u pole objektu znamena kapacitu 0.
 * @param c Alokuje pamet pro cap objektu
 * @param cap kapacita objektu
 */
void init_cluster(struct cluster_t *c, int cap);


/**
 * Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 * @param c odstrani c objekt
 */
void clear_cluster(struct cluster_t *c);

/**
 * Chunk of cluster objects. Value recommended for reallocation.
 */
extern const int CLUSTER_CHUNK;

/**
 * Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 * @param c zmeni capacitu shluku c
 * @param new_cap zmeni kapacitu na hodnotu new_cap
 * @return  NULL chyba, c bez chyby
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 * nevejde.
 * @param c prida obj do c
 * @param obj novy objekt
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/** @}*/

/** \addtogroup Pole_shluku Prace s polem shluku
 *  @{
 */

/**
 * Do shluku 'c1' prida objekty 'c2'. 
 * @pre spoji dva shluky ktore najde funkcia find_neighbours
 * @post vysledok su spojene shluky c1 a c2
 * @param c1 do c1 priradi c2, a zoradi
 * @param c2 sa nezmeni
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Odstrani shluk z pole shluku 
 * @param carr odstrani vsetky shluky od idx po narr
 * @param idx odstarnuje od idx
 * @param narr odstranuje po narr
 * @return  narr -1
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * Pocita Euklidovskou vzdalenost mezi dvema objekty.
 * @param o1 objekt 1
 * @param o2 objekt 2
 * @return vzdialenost dvoch objektov
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * Pocita vzdalenost dvou shluku.
 * @param c1 shluk 1
 * @param c2 shluk 2
 * @return bzdialeost dvoch shlukov
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Funkce najde dva nejblizsi shluky.
 * @pre pouziva sa v main na najdenie shlukov
 * @post najdene shluky nasledne spracuje funkcia merge_clusters
 * @param[in] carr pole shlukov
 * @param[in] narr velkost pola shlukov carr
 * @param[out] c1 index najblizsich zhlukov
 * @param[out] c2 index najblizsich zhlukov
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 * @param c usporiad shluk c
 */
void sort_cluster(struct cluster_t *c);

/**
 * Tisk shluku 'c' na stdout.
 * @param c vypise shluk c
 */
void print_cluster(struct cluster_t *c);

/**
 * Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 * jej do pole shluku. 
 * @param filename nazov suboru
 * @param arr pole shlukov
 * @ return 1 error, 0 v poriadku
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * Tisk pole shluku.
 * @param carr ukazatel na prvni polozku
 * @param narr Tiskne se prvnich 'narr' shluku.
 */
void print_clusters(struct cluster_t *carr, int narr);
/** @}*/
