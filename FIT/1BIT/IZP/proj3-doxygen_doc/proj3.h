/** \file
 * @file    proj3.h
 * @author 	Zdenek Kamensky
 * @date 	14 Dec 2015
 * @brief 	This program loads objects from stdin and sort them to clusters by their distance.
 */

/**
 * @brief      Struct for objects. It contains ID of object and its x and y values.
 */
struct obj_t {
    /// ID of object
    int id;
    /// Coordinate of object on axis X 
    float x;
    /// Coordinate of object on axis y
    float y;
};

/**
 * @brief      Struct of cluster. It contains size and capacity of cluster and pointer to an object.
 */
struct cluster_t {
    ///Size of cluster
    int size;
    /// Capacity (reserved memory for how many objects)
    int capacity;
    /// Array of objects in the cluster, NULL if it is uninitialized
    struct obj_t *obj;
};

/**
 * @defgroup    cluster		Cluster operations
 * @addtogroup  cluster 
 * @{
 */


/**
 * @brief      Initialization of cluster 'c'. It allocates the memory
 *             for cap (capacity). Pointer NULL by the array of object means capacity 0.
 * @param      c     Cluster 'c', which will be allocated.
 * @param      cap   Value of capacity for cluster 'c'.
 * @pre        c != NULL, c >= 0
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief      This function clears all objects from cluster 'c' and initializes to an empty cluster.
 * @param      c     Cluster 'c', which will be cleared.
 * @post       c = {0, 0, NULL}
 */
void clear_cluster(struct cluster_t *c);

/// Chunk of cluster objects. Value recommended for reallocation.
extern const int CLUSTER_CHUNK;

/** 
 * @brief      This function changes capacity of the cluster 'c' to a new capacity (new_cap).
 * @param      c        Cluster 'c', which will be resized.
 * @param      new_cap  New capacity for cluster 'c'.
 * @pre        c->capacity >= 0, new_cap >= 0
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/** 
 * @brief      It adds the objcect 'obj' to an end of cluster 'c'. It will expand the cluster 'c', 
 * 			   if there isn't allocated memory for this object.   
 * @param      c     Cluster 'c' for adding some objects.
 * @param      obj   Object 'obj', which will be added on.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief      This function adds objects from cluster 'c2' to cluster 'c1'. 
 * 			   If it's necessary, the cluster 'c1' will be extended. 
 * 			   Ojects in cluster 'c1' will be sorted in ascending order by their ID.
 * @param      c1    Cluster , where will be added objects from cluster 'c2'.
 * @param      c2    Cluster  of objects, which will be added to cluster 'c1'.
 * @pre        c1 != NULL, c2 != NULL
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief      This function clears the cluster of index idx in the array of clusters (carr) and remove this 
 * 			   cluster. Other clusters folowing this cluster will be relocating back one space.
 *
 * @param      carr  Array of clusters.
 * @param      narr  Number of clusters in array (array).
 * @param      idx   Index of cluster, which should be removed.
 * @return     Function returns new number of cluster in array (carr)
 * @pre        idzx < narr, narr > 0 
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @brief      It counts the Euclidean distance between two objects.
 * @param      o1    Object number one.
 * @param      o2    Object number two.
 * @return     It returns the distance between these two objects.
 * @pre        o1 != NULL, o2 != NULL
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/** 
 * @brief      This function counts the distance between two clusters by the function obj_distance.
 *             This distance is counted by the nearest neighbor. 
 * @param      c1    Cluster number one.
 * @param      c2    Cluster number two.
 * @return     Function returns the disetance between these two clusters.
 * @pre        c1 != NULL, c1->size > 0, c2 != NULL, c2->size > 0 
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief     Function find the nearest cluster by function cluster_distance 
 *            in the array carr. Two nearest clusters are in c1 and c2. 
 * @param      carr  Araay of clusters.
 * @param      narr  Number of clusters in array carr.
 * @param      c1    First of two nearest clusters.
 * @param      c2    Second of two nearest clusters.
 * @pre        0 < narr
 * @post       0 <= *c1 < *c2 < narr	
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/** 
 * @brief      Function sorts objects in cluster 'c' by the ID. 
 * @param      c     Cluster of objects.
 */
void sort_cluster(struct cluster_t *c);

/** 
 * @brief      This function prints the objects from cluster 'c' to stdout.
 * @param      c     Cluster of objects.
 */
void print_cluster(struct cluster_t *c);

///@}

/**
 * @defgroup array Operations over array of clusters
 * @addtogroup array
 * @{
 */

/** 
 * @brief      Function loads objects from file. It makes the clusters for all objects and saves 
 *             the clusters to an array. It allocates a space for all clusters in array and saves 
 *             the pointer, which points to the first cluster in array. This pointer will be saved to
 *             memory, where points the parameter 'arr'.  
 * @param      filename  Name of file, from which we are reading.
 * @param      arr       Pointer to an array of clusters.
 * @return     It returns number of loaded clusters in array.
 * @pre        arr != NULL
 */
int load_clusters(char *filename, struct cluster_t **arr);

/*! 
 * @brief      Function prints the array of clusters by the function print_cluster.
 * @param      carr  Pointer to the first cluster in array.
 * @param      narr  Number of cluster in array.
 */
void print_clusters(struct cluster_t *carr, int narr);

///@}