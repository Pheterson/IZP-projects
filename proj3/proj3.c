/**
 * File proj3.c
 * Project 3 - Simple cluster analysis
 * Unweighted pair-group average
 * Author Peter Koprda
 * Date December 2018
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // square root from float number
#include <limits.h> // INT_MAX

/**
 * Debugging macros. Their effect can be turned off by definition of macro.
 * NDEBUG, e.g.:
 *   a) during compile using argument of compiler -DNDEBUG
 *   b) in file (on line before #include <assert.h>
 *      #define NDEBUG
 */

#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// prints out debugging string
#define debug(s) printf("- %s\n", s)

// prints out formatted debugging output ->
// -> application similar to printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// prints out debugging information about variable ->
// -> using dint(identifier of variable)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// prints out debugging information about variable of data type float ->
// -> using dfloat (identifier of variable)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif


/**
 * 		DECLARATION OF REQUIRED DATA TYPES:
 *
 * struct obj_t - structure of object: identifier and coordinates
 * struct cluster_t - cluster of object:
 *	count of objects in cluster,
 *	size of cluster (count of objects for which are reserved location in array),
 *	pointer on array of clusters.
 *
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



////////// DECLARATION OF REQUIRED FUNCTIONS //////////

/**
 * Initialization of cluster 'c'. Allocate memory for cap(capacity) of object.
 * Pointer NULL in the array of object means capacity=0.
 */
void init_cluster(struct cluster_t *c, int cap)
{
	assert(cap >= 0);
	assert(c != NULL);

	if(c == NULL)
	/* c is object of cluster; cap is capacity of cluster */
	{
		cap=0;
	}


	if(cap > 0)
	/* allocates memory for object and assigns capacity to the cluster */
	{
		c->obj = malloc(sizeof(struct obj_t)*cap);
		if((c->obj) == NULL)
		{
			fprintf(stderr,"ERROR!\n");
			return;
		}
		c->capacity=cap;
	}

	if(cap <= 0)
	/* assigns 0 to the capacity of cluster and to the pointer on array of cluster */
	{
		c->capacity = 0;
		c->obj = 0;
	}

	c->size = 0;
	/* assigns 0 to the count of objects in cluster */
}


/**
 * Function removes all objects of cluster and initialize them on empty cluster.
 */
void clear_cluster(struct cluster_t *c)
{
	assert(c);

	free(c->obj);

	init_cluster(c, 0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;


/**
 * Change cluster capacity 'c' on capacity 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
	{
		 return c;
	}

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
	{
		return NULL;
	}


    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/**
 * Adds object 'obj' at the end of cluster 'c'.
 * Expands cluster, if cluster is too small for object.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
	if(c->capacity == c->size)
	{
		c = resize_cluster(c,c->capacity+CLUSTER_CHUNK);
	}

	if(c->capacity > c->size)
	{
		c->obj[c->size]=obj;
		c->size ++;
	}

}


/**
 * Sorts objects in cluster 'c' from the greatest to the lowest by their identification number.
 */
void sort_cluster(struct cluster_t *c);


/**
 * Adds objects 'c2' to the cluster 'c1'.
 * Cluster 'c1' will be expand in case of necessity.
 * Objects in cluster 'c1' will be sorted from the lowest to the greatest by their identification numbers.
 * Cluster 'c2' will not change.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

	if(c1==NULL || c2==NULL)
	{
		fprintf(stderr,"ERROR!\n");
		return;
	}

	for (int i = 0; i < c2->size; i++)
	/** Adds clusters to the array of clusters **/
	{
		append_cluster(c1, c2->obj[i]);
	}


    sort_cluster(c1);
    /* Sorts clusters from the lowest to the greatest by their identification number */
}



////////// TASK WITH ARRAY OF CLUSTER //////////
 
/*
 * Remove cluster from array of clusters 'carr'.
 * Array of cluster contains 'narr' items (of cluster).
 * Cluster after removing is situated on the index 'idx'.
 * Function returns new count of clusters in array.
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

	clear_cluster(&carr[idx]);
	for(int i=idx; i<narr-1; i++)
	/* cluster will be situated on the index 'idx' */
	{
		carr[i]=carr[i+1];
	}


	return narr-1;
	/* returns new count of clusters in array */
}

/**
 * Counts Euclidean distance between two objects.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    if(o1==NULL || o2==NULL)
	{
		fprintf(stderr,"ERROR!\n");
		return -1;
	}

	float first = o2->x - o1->x;
	float second = o2->y - o1->y;
	/* distance between coordinates */

	return sqrtf((first*first)+(second*second));
	/* returns Euclidean distance between two objects */
}

/**
 * Counts distance of two clusters.
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

	if(c1==NULL || c2==NULL || c1->size<=0 || c2->size<=0)
	{
		fprintf(stderr,"ERROR!\n");
		return -1;
	}

	//the smallest distance between two objects
	float distance=obj_distance(&c1->obj[0],&c2->obj[0]);
		
	float helpDistance=0.0;										

	for(int i=0; i< c1->size; i++)
	/* counts distance between two clusters */
	{
		for(int j=0; j < c2->size; j++)
		{
			helpDistance = obj_distance(&c1->obj[i],&c2->obj[j]);
			if(distance > helpDistance)
			{
				distance = helpDistance;
			}

		}
	}

	return distance;
	/* returns distance between two clusters */
}


/**
 * Function finds two closest clusters.
 * Function in array of clusters 'carr' with size 'narr' looks for two closest clusters.
 * Found clusters identifies in their index of array 'carr'.
 * Function saves found clusters into memory on address 'c1' resp. 'c2'.
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

	if(narr <= 0)
	{
		fprintf(stderr,"ERROR!\n");
		return;
	}

	float closestDis = cluster_distance(&carr[0],&carr[1]);

	for(int i=0; i<narr; i++)
	/* finds two closest clusters */
	{
		for(int j=i+1; j<narr; j++)
		{
			if(closestDis > (cluster_distance(&carr[i],&carr[j])))
			{
				closestDis=cluster_distance(&carr[i],&carr[j]);
				*c1=i;
				*c2=j;
			}
		}
	}
}

// help function for sorting a cluster
static int obj_sort_compar(const void *a, const void *b)
{
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}


/*
 * Sorting objects in cluster from the greatest to the lowest by their identifiers.
 */
void sort_cluster(struct cluster_t *c)
{
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}


/*
 * Printing out 'c' on standard output.
 */
void print_cluster(struct cluster_t *c)
{
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}


/**
 * From file 'filename' reads objects.
 * For every object creates cluster and saves it into array of clusters.
 * Function allocates area for array of all clusters and pointer on the first item of array 
 * (pointer on the first cluster in allocated memory) saves into the memory,
 * where parameter 'arr' refers to.
 * Function returns count of read objects (of cluster).
 * In case of some error saves into memory value NULL, where parameter 'arr' refers to.
 */

int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    FILE *objects=fopen(filename,"r");

    if(objects == NULL)
	/** File could not be opened. **/
	{
		fprintf(stderr,"ERROR! File could not be opened!\n");
		return -1;
	}

	int count;
	/* count of clusters loaded from file */

	fscanf(objects,"count=%d",&count);

    if(count<=0 || count>INT_MAX)
	/* function load_clusters terminates */
	{
		fclose(objects);
		return 0;
	}

    *arr=malloc(sizeof(struct cluster_t)*count);
    /* memory allocation for *arr */

    if(arr == NULL)
	{
		fclose(objects);
		fprintf(stderr,"ERROR!\n");
		return 0;
	}


	struct obj_t objekt;
	/* declaration of structure obj_t */

	unsigned int i;
	int const MAX = 1000;
	int const MIN = 0;

	for(i=0; i<(unsigned int) count; i++)
	/* loads objects, initialize clusters and adds object at the end of the cluster */
	{
		fscanf(objects,"%d ",&objekt.id);
		fscanf(objects,"%f ",&objekt.x);
		fscanf(objects,"%f",&objekt.y);
		/* loads ID of object and their coordinates */

		if(objekt.x<MIN || objekt.x>MAX || objekt.y<MIN || objekt.y>MAX )
		/* Error handling */
		{
			fclose(objects);
			return -1;
		}


		init_cluster(&(*arr)[i],CLUSTER_CHUNK);
		/* initialize cluster on position i with capacity 10*/

		append_cluster(&(*arr)[i],objekt);
		/* adds object at the end of the cluster */
	}


	fclose(objects);


	return i;
}


/**
 * Function prints out array of clusters.
 * Parameter 'carr' is pointer on the first item of cluster.
 * Function prints out first 'narr' of cluster.
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


int main(int argc, char *argv[])
{
    if((argc == 2)||(argc == 3))
	/* argv[2] is file with clusters and argv[3] is count of clusters */
	{
		struct cluster_t *clusters;
		int n;

		if(argc == 3)
		{
			sscanf(argv[2],"%d",&n);
			if((n<=0) || (sscanf(argv[2],"%d",&n)==0) || n>INT_MAX)
			/* checks if argv[3] is number and if this -> 0 <= n > INT_MAX(2 147 483 647) <- is true */
			{
				fprintf(stderr,"Invalid input!\n");
				return EXIT_FAILURE;
			}
		}
		else
		/* missing third argument */
		{
			n = 1;
		}

		int readClusters=load_clusters(argv[1],&clusters);
		/* loads count of clusters from file */

		if(readClusters == -1)
		/** file error **/
		{
			fprintf(stderr,"Please insert valid file or try again...\n");
			return EXIT_FAILURE;
		}

		else if(readClusters == 0)
		/* no allocated memory, count in file is equal to or smaller than 0 */
		{
			fprintf(stderr,"Function load_clusters stopped working!");
			free(clusters);
			return EXIT_FAILURE;
		}

		if(n > readClusters)
		/* count of clusters from argv[2] is greater than count of clusters from file */
		{
			fprintf(stderr,"ERROR! Variable n must be smaller than or equal to the count of clusters from file!\n");
			free(clusters);
			return EXIT_FAILURE;
		}

		int idx1,idx2 = 0;
		/* help variables */

		while(n < readClusters)
		{
			find_neighbours(clusters, n, &idx1, &idx2);
			/* finds two closest clusters*/

			merge_clusters(&clusters[idx1], &clusters[idx2]);
			/* sorts clusters by their identification numbers */

			readClusters = remove_cluster(clusters, readClusters, idx2);
			/* loads count of remaining clusters */
		}

		print_clusters(clusters,n);

		for(int i=0; i<n; i++)
		{
			clear_cluster(&clusters[i]);
		}

		free(clusters);
	}


	else
	/* wrong arguments */
	{
		fprintf(stderr,"ERROR! Wrong arguments!\n"
		"\n"
		"Usage: ./proj3 FILE [N]\n"
		"       FILE => name of the file with input data\n"
		"       N    => target number of clusters (optional argument)\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
