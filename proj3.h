/**
 * @file proj3.h
 * @brief Project 3 - Simple cluster analysis
 * @author Peter Koprda
 * @date December 2018
 */

/**
 * @defgroup Structures
 * @{
 */

/**
 * @brief Structure of object
 */
struct obj_t {
	/** identifier **/
    int id;
    /** first coordinate **/
    float x;
    /** second coordinate **/
    float y;
};

/**
 * @brief Cluster of objects
 */
struct cluster_t {
	/** count of objects in cluster **/
    int size;
	/** size of cluster **/
    int capacity;
	/** pointer on array of clusters **/
    struct obj_t *obj;
};
/**
 * @}
 */


/**
 * @defgroup Functions
 * @{
 */

/**
 * @brief Initialization of cluster \a c.
 * 		Allocates memory for capacity of object.
 *
 * @pre
 * Cluster \a c is not NULL
 *
 * @post
 * Capacity of cluster \a c is greater or equal to 0
 *
 * @param c Cluster with parameters
 * @param cap Capacity of object
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief Function remove all objects of cluster and initialize them on empty cluster.
 *
 * @post
 * Allocated memory for all objects of cluster
 *
 * @param c Cluster with objects
 */
void clear_cluster(struct cluster_t *c);


extern const int CLUSTER_CHUNK;

/**
 * @brief Changes capacity of cluster.
 *
 * @pre
 * Capacity of cluster is greater or equal to 0
 * @pre
 * New capacity of cluster is greater or equal to 0
 *
 * @post
 * Capacity of cluster is changed to new capacity
 *
 * @param c Cluster with objects
 * @param new_cap New capacity of cluster
 * @return cluster with parameters
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief Adds object at the end of cluster.
 * Expands cluster, if cluster is too small for object.
 *
 * @post
 * At the end of cluster is added object.
 *
 * @param c Cluster with parameters.
 * @param obj Object with parameters.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief Sorts objects of cluster \a c1 in ascending order by their identification numbers
 * @brief Adds objects from cluster \a c2 to the cluster \a c1.
 * Cluster \a c1 will be expand in case of necessity.
 *
 * @pre
 * Clusters \a c1 and \a c2 are not NULL
 *
 * @post
 * Cluster \a c2 is unchanged.
 * @post
 * Clusters are merged.
 *
 * @param c1 First cluster
 * @param c2 Second cluster
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

 /**
 * @brief Remove cluster from array of clusters.
 *
 * @post
 * Cluster is situated on the index of cluster.
 *
 * @param carr Array of clusters
 * @param narr Count of items of cluster.
 * @param idx Index of cluster
 * @return new count of clusters in array
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @param o1 Object 1
 * @param o2 Object 2
 *
 * @pre
 * Objects \a o1 and \a o2 are not NULL
 *
 * @return Euclidean distance between two objects
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @param c1 First cluster
 * @param c2 Second cluster
 *
 * @pre
 * Clusters \a c1 and \a c2 are not NULL
 * @pre
 * Size of clusters \a c1 and \a c2 are greater than 0
 *
 * @return distance between two clusters
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Finds two closest clusters.
 * Function in array of clusters 'carr' with size 'narr' looks for two closest clusters.
 *
 * @pre
 * Count of clusters are greater than 0
 *
 * @post
 * Index of two closest clusters are stored in \a c1 and \a c2
 *
 * @param carr Array of clusters
 * @param narr Count of clusters
 * @param c1 Index of cluster \a c1
 * @param c2 Index of cluster \a c2
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief Sorts objects in cluster \a c in ascending order by their identification numbers.
 *
 * @param c Cluster with objects
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief Printing out cluster to standard output.
 *
 * @param c Cluster with objects
 */
void print_cluster(struct cluster_t *c);

/**
 * @brief Loads objects from input text file.
 * For every object creates cluster and saves it into array of clusters.
 * Function allocates area for array of all clusters and pointer on the first item of array
 * (pointer on the first cluster in allocated memory) saves into the memory,
 * where parameter \a arr refers to.
 *
 * @param filename The input text file with loaded objects
 * @param arr Clusters from the input file
 *
 * @pre
 * \a arr is not NULL
 *
 * @post
 * loaded clusters with objects
 *
 * @return Count of loaded objects of cluster
 */
int load_clusters(char *filename, struct cluster_t **arr);

 /**
 * @brief Function prints out array of clusters. Function prints out first \a narr of cluster.
 *
 * @post
 * Printed out clusters on standard output
 *
 * @param carr Pointer on the first item of cluster
 * @param narr Count of clusters
 */
void print_clusters(struct cluster_t *carr, int narr);

/**
 * @}
 */
