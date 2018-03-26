#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#include "General.h"
#include "RedBlack_Tree.h"
#include "Queue.h"

#define NOTHING    0
#define LEFT_NODE  1
#define RIGHT_NODE 2

#define BLACK 0
#define RED   1

#define MAX(a,b) (((a)>(b))?(a):(b))

struct _NODE {
    Object info;
    int colour;
    struct _NODE* parent;
    struct _NODE* left;
    struct _NODE* right;
};

typedef struct _NODE* _Node;   

struct REDBLACK_ROOT {
    COMPARISON_FUNC compare_component;
    COPY_FUNC copy_component;
    DESTRUCTION_FUNC destroy_component;
    int num_of_elements;    
    struct _NODE* childs;
};

static void case1(const _Node);
static void case2(const _Node);
static void case3(const _Node);
static void case4(_Node);
static void case5(const _Node);

static _Node Tree_FindLargestElement(const _Node);
static _Node Tree_FindSmallestElement(const _Node);
static void delete_child(_Node);
static void del_case1(const _Node);
static void del_case2(const _Node);
static void del_case3(const _Node);
static void del_case4(const _Node);
static void del_case5(const _Node);
static void del_case6(const _Node);

static _Node root_child;

static _Node getGrantParent(const _Node this) {

    _Node gParent = NULL;
     
    if ((this) && (this->parent))
        gParent = this->parent->parent;
    
    return gParent;
    
} /* end of static _Node getGrantParent(const _Node) */

static _Node getUncle(const _Node this) {

    _Node gParent;

    if ((gParent = getGrantParent(this)) == NULL)
        return NULL;
        
    return (this->parent == gParent->left) ? gParent->right : gParent->left;

} /* end of static _Node getUncle(const _Node) */

static _Node getSibling(const _Node this) {

    if ((this) && (this->parent)) {
        return (this == this->parent->left) ? this->parent->right : this->parent->left;
    } else 
        return NULL;
        
} /* end of static _Node getSibling(const _Node) */

static int getColour(const _Node this) {

    return (this) ? this->colour : BLACK;
	
} /* end of static int getColour(const _Node) */

/* Null nodes are leafs, no data is stored in the leaf nodes.*/
static BOOL isLeaf(const _Node this) {

    //return (!(this->left) && !(this->right)) ? TRUE : FALSE;
    return (this == NULL) ? TRUE : FALSE;
        
} /* end of static BOOL isLeaf(const _Node) */

static void replaceNode(const _Node* lhs_node, _Node* rhs_node) {

    if (*rhs_node == NULL) {
	    (*lhs_node)->info = NULL;
		*rhs_node = *lhs_node;
		return;
	} /* end of if (*rhs_node == NULL) */

    (*rhs_node)->parent = (*lhs_node)->parent;
	
    if ((*lhs_node)->parent->left == (*lhs_node))
        (*lhs_node)->parent->left = (*rhs_node);
    else
       	(*lhs_node)->parent->right = (*rhs_node);
    
} /* end of static void replaceNodeContent(const _Node*, const _Node*) */

static void rotate_left(_Node this) {
    
    _Node right_child = this->right, parent = this->parent;
    
    if (right_child == NULL) return;
          
    if (parent) {
        if (parent->left == this)
            parent->left = right_child;
        else 
            parent->right = right_child;
    } /* end of if (parent) */
	
    right_child->parent = parent;
    
    if (right_child->left)
        right_child->left->parent = this;
		
    this->right = right_child->left;
    right_child->left = this;
    this->parent = right_child;
	
    if (right_child->parent == NULL)
        root_child = right_child;
    
} /* end of static void rotate_left(_Node) */

static void rotate_right(_Node this) {
    
    _Node left_child = this->left, parent = this->parent;
    
    if (left_child == NULL) return;
          
    if (parent) {
        if (parent->left == this)
            parent->left = left_child;
        else 
            parent->right = left_child;
    } /* end of if (parent) */
	
    left_child->parent = parent;
    
    if (left_child->right)
        left_child->right->parent = this;
		
    this->left = left_child->right;
    left_child->right = this;
    this->parent = left_child;
	
    if (left_child->parent == NULL)
        root_child = left_child;

} /* end of static void rotate_right(_Node) */

static _Node create_Node(const Object info) {

    _Node this = (_Node) malloc(sizeof(struct _NODE));
	
    if (!this) {
        fprintf(stderr, "Out of memory for creating _Node...\n");
        return NULL;
    } /* end of if (!this) */
    
    this->info = info;
    this->colour = RED;
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
    
    return this;
    
} /* end of static _Node create_Node(const Object) */

/* 1. The colour of a node is either red or black. */
/* 2. The colour of the root node is always black  */
static void case1(const _Node this) {

    if (this == NULL) return;

    if (this->parent == NULL) this->colour = BLACK;
    else case2(this);
    
} /* end of static void case1(const _Node) */

/* The new node’s Parent is black */
static void case2(const _Node this) {

    if (this->parent->colour == BLACK) 
        return;    /* Red black tree property is not violated*/
    else case3(this);
    
} /* end of static void case2(const _Node) */

/* If both the parent and the uncle (u) are red */
static void case3(const _Node this)
{
    _Node u, g;
    u = getUncle(this);
    g = getGrantParent(this);
	
    if ((u != NULL) && (u->colour == RED)) {
        this->parent->colour = BLACK;
        u->colour = BLACK;
        g->colour = RED;
        case1(g);
    }
    else case4(this);

} /* end of static void case3(_Node) */

/*
   The parent (p) is red but the uncle (u) is black and new node (n) is the right child of p and p is the left
   child of g 
   OR
   The parent (p) is red but the uncle (u) is black and new node (n) is the left child of p and p is the right
   child of g   
*/
static void case4(_Node this) {

    _Node g;
    
    if ((g = getGrantParent(this)) == NULL) return;
    
    if ((this == this->parent->right) && (this->parent == g->left)) {
        rotate_left(this->parent);
        this = this->left;
    }
    else if ((this == this->parent->left) && (this->parent == g->right)) {
        rotate_right(this->parent);
        this = this->right;
    } /*end of if-else */
    
    case5(this);
    
} /* end of static void case4(_Node) */

/*
   The Parent (p) is red but the uncle (u) is black and the new node (n) is the left child of p, and
   p is the left child of its parent g.
   OR
   The Parent (p) is red but the uncle (u) is black and the new node (n) is the right child of p, and
   p is the right child of its parent g.
   
*/
static void case5(const _Node this) {

    _Node g = getGrantParent(this);
    
    if ((this == this->parent->left) && (this->parent == g->left))
        rotate_right(g);
    else if ((this == this->parent->right) && (this->parent == g->right))
        rotate_left(g);  
		
    this->parent->colour = BLACK;
    g->colour = RED;
      
} /* end of static void case5(const _Node) */

RedBlack_Root RedBlack_Tree_Create(RedBlack_Tree_Component_Funcs funcs) {
    RedBlack_Root ret_root;
    
    if ((ret_root = (RedBlack_Root) malloc(sizeof(struct REDBLACK_ROOT))) == NULL) {
        fprintf(stderr, "Out of memory for creating REDBLACK tree!!!\n");
        return NULL;
    } /* end of if ((ret_root = (RedBlack_Root) malloc(sizeof(struct REDBLACK_ROOT))) == NULL) */
	
    ret_root->compare_component = funcs.compare_component;
    ret_root->copy_component = funcs.copy_component;
    ret_root->destroy_component = funcs.destroy_component;
    ret_root->num_of_elements = 0;
    ret_root->childs = NULL;
    
    return ret_root;
    
} /* end of RedBlack_Root RedBlack_Tree_Create(RedBlack_Tree_Component_Funcs) */

int RedBlack_Tree_GetNumberOfElements(const RedBlack_Root root) {

    return ((root != NULL) && (root->childs != NULL)) ? root->num_of_elements : 0;
    
} /* end of int RedBlack_Tree_GetNumberOfElements(const RedBlack_Root) */

Object RedBlack_Tree_GetElement(const RedBlack_Root this, const Object info) {

    _Node node;
    Object ret_info = NULL;
    if (!this) return NULL;
    
    node = this->childs;
    
    while ((node && node->info) && (!ret_info)) {
	
        int cmp_result = this->compare_component(info, node->info);
        
        if (cmp_result > 0) node = node->left;
        else if (cmp_result < 0) node = node->right;
        else ret_info = node->info;
		
    } /* end of while ((node && node->info) && (!ret_info)) */
    
    return ret_info;
    
} /* end of Object RedBlack_Tree_GetElement(const RedBlack_Root, const Object) */

void Tree_Destroy(_Node *this, DESTRUCTION_FUNC destruction_func) {

    if (*this != NULL) {        
        Tree_Destroy(&((*this)->left), destruction_func);
        Tree_Destroy(&((*this)->right), destruction_func);        
        destruction_func((*this)->info);
        (*this)->info = NULL;
        (*this)->parent = NULL;
        free(*this);
        *this = NULL;
    } /* end of if (*this != NULL) */
	
} /* end of void Tree_Destroy(Node *this, DESTRUCTION_FUNC) */

void RedBlack_Tree_Destroy(RedBlack_Root *this) {

    if (*this != NULL) {
        Tree_Destroy(&((*this)->childs), (*this)->destroy_component);        
        free(*this);
        *this = NULL;
    } /* end of if (*this != NULL) */
	
} /* end of void RedBlack_Tree_Destroy(RedBlack_Root *) */

static BOOL insert_node(const RedBlack_Root root, _Node this, const _Node new_node) {

    COMPARISON_FUNC cmp = root->compare_component;   
    _Node parent;
    int which_node;
    
    if (this == NULL) return FALSE;    
    
    while (this) {
	
        parent = this;
        if (cmp(new_node->info, this->info) > 0) {
            which_node = LEFT_NODE;
            this = this->left;
        } else {
            which_node = RIGHT_NODE;
            this = this->right;
        } /* end of if-else */
		
    } /* end of while (this) */
    
    this = parent;
    
    if (which_node == LEFT_NODE) {
        this->left = new_node;
        new_node->parent = this;
    } else {
        this->right = new_node;
        new_node->parent = this;
    } /* end of if-else */   
    
    return TRUE;

} /* end of static BOOL insert_node(const RedBlack_Root, const _Node, const _Node) */

void RedBlack_Tree_InsertElement(const RedBlack_Root this, const Object info) {

    _Node temp_node;        
    
    if ((temp_node = create_Node(info)) == NULL) return;

    if (!this->childs) {        
        this->childs = temp_node;
        this->num_of_elements++;
        case1(this->childs);      
        return;
    } /* end of if (!this->childs) */
    
    if (insert_node(this, this->childs, temp_node) == TRUE) {
        case1(temp_node);
        if (root_child)
            this->childs = root_child;       
        this->num_of_elements++;
    } /* end of if (insert_node(this, this->childs, temp_node) == TRUE) */
    
    return;
    
} /* end of void RedBlack_Tree_Insert(const AVL_Root, const Object) */

Object RedBlack_Tree_DeleteElement(const RedBlack_Root this, const Object info) {

    _Node node, copied;
    Object ret_info = NULL;
    if ((!this) || (this->num_of_elements == 0)) return NULL;
    
    node = this->childs;
    
    while ((node && node->info) && (!ret_info)) {
	
        int cmp_result = this->compare_component(info, node->info);
        
        if (cmp_result > 0) node = node->left;
        else if (cmp_result < 0) node = node->right;
        else ret_info = node->info;
		
    } /* end of while ((node && node->info) && (!ret_info)) */
	
	if (node) {
	    if ((node->left) || (node->right)) {
	        if (node->left)
		        copied = Tree_FindLargestElement(node->left);
	        else if (node->right)
		        copied = Tree_FindSmallestElement(node->right);
		    node->info = copied->info;
		} else
		    copied = node;
		
		delete_child(copied); 
        
        if ((--this->num_of_elements) == 0) 
		    this->childs = NULL;
			
	} /* end of if (node) */
    
    return ret_info;
    
} /* end of Object RedBlack_Tree_DeleteElement(const RedBlack_Root, const Object) */

Object RedBlack_Tree_FindLargestElement(const RedBlack_Root this) {

    _Node node;
	
    if (!this) return NULL; 
	
    node = Tree_FindLargestElement(this->childs);
    
    return (node != NULL) ? node->info : NULL;
    
} /* end of Object RedBlack_Tree_FindLargestElement(const AVL_Root) */ 

Object RedBlack_Tree_FindSmallestElement(const RedBlack_Root this) {

    _Node node;
	
    if (!this) return NULL;    
	
    node = Tree_FindSmallestElement(this->childs);
    
    return (node != NULL) ? node->info : NULL;
    
} /* end of Object RedBlack_Tree_FindSmallestElement(const RedBlack_Root) */ 

static _Node Tree_FindLargestElement(const _Node this) {

    _Node ret_node = this;
	
    if (ret_node == NULL) return NULL;    
	
    while (ret_node->right) ret_node = ret_node->right;
        
    return ret_node;
        
} /* end of static _Node Tree_FindLargestElement(const _Node) */

static _Node Tree_FindSmallestElement(const _Node this) {

    _Node ret_node = this;
	
    if (ret_node == NULL) return NULL;    
	
    while (ret_node->left) ret_node = ret_node->left;
        
    return ret_node;    

} /* end of static _Node Tree_FindSmallestElement(const _Node) */

static void delete_child(_Node this) {

    _Node child ;
	
	/* If n has at most one non–null child */
    if (isLeaf(this->right) == TRUE)
        child = this->left;
    else
        child = this->right;
    
    replaceNode(&this, &child);
    
    if (getColour(this) == BLACK) {
        if (getColour(child) == RED)
            child->colour = BLACK;
		else
		    del_case1(child);
	} /* end of if (getColour(this) == BLACK) */

    if (this->parent) {	
        if (this->parent->left == this)
	        this->parent->left = NULL;
		
        if (this->parent->right == this)
            this->parent->right = NULL; 
    } /* end of if (this->parent) */
	
	free(this);
	
} /* end of static void delete_child(_Node) */

static void del_case1(const _Node this) {

    if (this->parent != NULL)
        del_case2(this);
		
} /* end of static void del_case1(const _Node) */

static void del_case2(const _Node this) {

    _Node s = getSibling(this);
		
    if (getColour(s) == RED) {
        if (this == this->parent->left)
            rotate_left(this->parent);
        else
            rotate_right(this->parent);
        this->parent->colour = RED;
        s->colour = BLACK;
    } /* end of if (getColour(s) == RED) */
	
    del_case3(this);
	
} /* end of static void del_case2(const _Node) */

static void del_case3(const _Node this) {

    _Node s = getSibling(this);
	
	if (s == NULL) return;
    
	if ((getColour(this->parent) == BLACK) && (getColour(s) == BLACK) &&
	    (getColour(s->left) == BLACK) && (getColour(s->right) == BLACK)) {
		s->colour = RED;
		del_case1(this->parent);
	} else
	    del_case4(this);

} /* end of static void del_case3(const _Node) */

static void del_case4(const _Node this) {

    _Node s = getSibling(this);

	if ((getColour(this->parent) == RED) && (getColour(s) == BLACK) &&
	    (getColour(s->left) == BLACK) && (getColour(s->right) == BLACK)) {
		s->colour = RED;
		this->parent->colour = BLACK;
	} else
	    del_case5(this);
		
} /* end of static void del_case4(const _Node) */

static void del_case5(const _Node this) {

    _Node s = getSibling(this);
	
	if ((s) && getColour(s) == BLACK) {

	    if ((this == this->parent->left) &&  (getColour(s->left) == RED) && (getColour(s->right) == BLACK)) 
	        rotate_right(s);
	    else if ((this == this->parent->right) &&  (getColour(s->left) == BLACK) && (getColour(s->right) == RED)) 
	        rotate_left(s);
			
		s->colour = RED;
		
		if (s->right)
		    s->right->colour = BLACK;
	    
    } /* end of if ((s) && getColour(s) == BLACK) */
	
	del_case6(this);
	
} /* end of static void del_case5(const _Node) */

static void del_case6(const _Node this) {

    _Node s = getSibling(this);

	if (s == NULL) return;
	
    s->colour = this->parent->colour;
    this->parent->colour = BLACK;
	
    if (this == this->parent->left) {
        s->right->colour = BLACK;
        rotate_left(this->parent);
    } else {
        s->left->colour = BLACK;
        rotate_right(this->parent);
    } /* end of if-else */
	
} /* end of static void del_case6(const _Node) */


 void empty_destroy(Object to_be_destroyed) {}

Object RedBlack_Tree_BFS_Search(const RedBlack_Root this, const Object info) {

    Queue_Component_Funcs queue_comp_funcs = { (COMPARISON_FUNC)this->compare_component, (COPY_FUNC)this->copy_component, (DESTRUCTION_FUNC) empty_destroy };
    Queue queue;    
    _Node node;
    Object ret_info = NULL;
	
    if ((this == NULL) || (this->childs == NULL)) return NULL;
	
	queue = Queue_Create(DEFAULT, queue_comp_funcs);    
    node = this->childs;
	
#ifdef DEBUG
    printf("\n Node\tColour\tParent\n");
	printf("------\t------\t------\n");
#endif

#ifdef DEBUG
    while (node) {
#else    
    while (node && !ret_info) {
#endif	
#ifdef DEBUG     
		printf(" %d \t%s \t%d\n", 
               *((Integer) node->info), (node->colour == RED) ? "RED" : "BLACK", 
               (node->parent != NULL) ? *((Integer) node->parent->info) : 0);
#endif

        if (this->compare_component(info ,node->info) == 0) { 
            ret_info = node->info;
#ifndef DEBUG			
			continue;
#endif
		} /* end of if (this->compare_component(info ,node->info) == 0) */
            
        if (node->left) Queue_Add(queue, (Object)(node->left));
        if (node->right) Queue_Add(queue, (Object)(node->right));
        node =  (Queue_IsEmpty(queue) == FALSE) ? (_Node) Queue_Remove(queue) : NULL;
        
    } /* end of while (node && !ret_info) */
    
    Queue_Destroy(&queue);
    
    return ret_info;

} /* end of Object RedBlack_Tree_BFS_Search(const RedBlack_Root, const Object) */

#ifdef DEBUG

static void Tree_PreorderTraversal(_Node this) {

    if (this != NULL) {
        printf(" %d \t%s \t%d\n", *((Integer) this->info), (this->colour == RED) ? "RED" : "BLACK", (this->parent != NULL) ? *((Integer) this->parent->info) : 0);
        Tree_PreorderTraversal(this->left);    
        Tree_PreorderTraversal(this->right);    
    } /* end of if (this != NULL) */
	
} /* end of void Tree_PreorderTraversal(Node) */

static void Tree_InorderTraversal(_Node this) {

    if (this != NULL) {
        Tree_InorderTraversal(this->left);    
        printf(" %d \t%s \t%d\n", *((Integer) this->info), (this->colour == RED) ? "RED" : "BLACK", (this->parent != NULL) ? *((Integer) this->parent->info) : 0);
        Tree_InorderTraversal(this->right);    
    } /* end of if (this != NULL) */
	
} /* end of void Tree_InorderTraversal(Node) */

static void Tree_PostorderTraversal(_Node this) {

    if (this != NULL) {    
        Tree_PostorderTraversal(this->left);    
        Tree_PostorderTraversal(this->right); 
        printf(" %d \t%s \t%d\n", *((Integer) this->info), (this->colour == RED) ? "RED" : "BLACK", (this->parent != NULL) ? *((Integer) this->parent->info) : 0);
    } /* end of if (this != NULL) */

} /* end of void Tree_PostorderTraversal(Node) */

void RedBlack_Tree_PreorderTraversal(const RedBlack_Root this) {

    printf("\n Pre-Order Traversal");
	printf("\n----------------------");
    printf("\n Node\tColour\tParent\n");
	printf("------\t------\t------\n");

    if (this->childs != NULL) 
        Tree_PreorderTraversal(this->childs); 
   
} /* end of void void RedBlack_Tree_PreorderTraversal(const RedBlack_Root) */

void RedBlack_Tree_InorderTraversal(const RedBlack_Root this) {

    printf("\n In-Order Traversal");
	printf("\n----------------------");
    printf("\n Node\tColour\tParent\n");
	printf("------\t------\t------\n");

    if (this->childs != NULL) 
        Tree_InorderTraversal(this->childs); 
   
} /* end of void RedBlack_Tree_InorderTraversal(const RedBlack_Root) */

void RedBlack_Tree_PostorderTraversal(const RedBlack_Root this) {

    printf("\n Post-Order Traversal");
	printf("\n----------------------");
    printf("\n Node\tColour\tParent\n");
	printf("------\t------\t------\n");

    if (this->childs != NULL) 
        Tree_PostorderTraversal(this->childs); 
   
} /* end of void RedBlack_Tree_PostorderTraversal(const RedBlack_Root) */

#endif


