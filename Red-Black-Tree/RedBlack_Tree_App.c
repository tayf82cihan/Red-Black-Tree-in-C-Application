#include <stdio.h>
#include <stdlib.h>

#include "RedBlack_Tree.h"


/*
*   compare function must afford 
*   the following conditions
*
*   lhs < rhs,    1
*   lhs > rhs,   -1
*   lhs = rhs,    0
*
*/
int compare_ints(Object lhs, Object rhs) {

    int rhs_int = *((Integer) rhs);
    int lhs_int = *((Integer) lhs);

    if (rhs_int > lhs_int) return 1;
    else if (rhs_int < lhs_int) return -1;
    else return 0;

} /* end of int compare_ints(Object lhs, Object rhs) */

Object copy_int(Object int_to_be_copied) {

    Integer ret_val = (Integer) malloc(sizeof(int));

    *ret_val = *((Integer) int_to_be_copied);

    return ret_val;

} /* end of Object copy_int(Object) */

void destroy_int(Object int_to_be_destroyed) {

    if (int_to_be_destroyed)
        free((Integer) int_to_be_destroyed);

} /* end of void destroy_int(Object) */

Object wrap_int(int int_to_be_wrapped) {

    Integer ip = (Integer) malloc(sizeof(int));
    *ip = int_to_be_wrapped;

    return ip;

} /* end of Object wrap_int(int) */

Object* prepareObjectArray(int* int_arr, int n) {
    
	Object* ret_arr = (Object*) malloc(sizeof(Object) * n);
	int i;
	
	for (i = 0; i < n; i++)
	    ret_arr[i] = wrap_int(int_arr[i]);
	
	return ret_arr;
	
} /* end of Object* prepareObjectArray(int*, int) */


int main(void) {

    RedBlack_Tree_Component_Funcs int_funcs = { &compare_ints, &copy_int, &destroy_int };
    RedBlack_Root root;
	
    int insertList[] = { 
	5746,3906,856,2067,6906,9665,2408,2477,4527,2100,
    9099,1247,1786,1524,674,9569,8171,4335,2891,8809,
    4474,7935,4424,1662,9743,1108,2769,7428,8677,1485,
    9115,3764,9714,6484,1684,6785,9014,8959,3067,4238,
    2430,8393,8327,3760,3186,9727,710,3751,8404,4406,
    3127,9623,5455,9009,4742,6869,3743,2072,4780,30,
    6708,9967,105,405,5943,5554,9622,2247,3480,3180,
    902,2534,8714,4068,5175,4934,6865,979,4651,6656,
    3336,9334,1826,9157,1951,1756,1418,4292,3209,9463,
    8418,6566,4114,2802,6442,6728,2081,2826,6891,1757 };
	
	int deleteList[] = { 4424,1662,9743,5943,2534,1757,1247,3336,856,6442,9967 };
	
	int size_of_insertList = sizeof(insertList)/sizeof(int);
	int size_of_deleteList = sizeof(deleteList)/sizeof(int);
    Object *obj_array, *deleting_array;				 
    Object founded_obj, to_be_found = wrap_int(9463);    
    int i;

    obj_array = prepareObjectArray(insertList, sizeof(insertList)/sizeof(int));
	deleting_array = prepareObjectArray(deleteList, sizeof(deleteList)/sizeof(int));
	
    if ((root = RedBlack_Tree_Create(int_funcs)) == NULL) return -1;

    for (i = 0; i < size_of_insertList; i++)
        RedBlack_Tree_InsertElement(root, obj_array[i]);
		
	printf("\nAfter insertion, size of RedBlack tree: %d\n", RedBlack_Tree_GetNumberOfElements(root));
	printf("\nLargest Element: %d\n", *(Integer) RedBlack_Tree_FindLargestElement(root));
	printf("\nSmallest Element: %d\n",*(Integer) RedBlack_Tree_FindSmallestElement(root));

    founded_obj = RedBlack_Tree_BFS_Search(root, to_be_found);
    
    if (founded_obj)    
        printf("By using RedBlack_Tree_BFS_Search, find %d in RedBlack tree: %d\n", *(Integer)to_be_found,  *(Integer)founded_obj);
    destroy_int(to_be_found);
	
	
	printf("Removing Operation:\n");
	for (i = 0; i < size_of_deleteList; i++) {
	    printf("\nRemoving %d\n",  *(Integer)deleting_array[i]);
        destroy_int(RedBlack_Tree_DeleteElement(root, deleting_array[i]));
	}

#ifdef DEBUG	
    RedBlack_Tree_BFS_Search(root, RedBlack_Tree_FindLargestElement(root)); 
#endif
	
	printf("\nAfter deletion, size of RedBlack tree: %d\n", RedBlack_Tree_GetNumberOfElements(root));
	
#ifdef DEBUG    
    RedBlack_Tree_PreorderTraversal(root);  
    RedBlack_Tree_InorderTraversal(root);   
    RedBlack_Tree_PostorderTraversal(root);  
#endif   

    RedBlack_Tree_Destroy(&root);
   
    return 0;

} /* end of int main(void) */

