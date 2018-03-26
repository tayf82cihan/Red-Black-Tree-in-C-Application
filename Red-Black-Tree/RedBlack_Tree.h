#ifndef RedBlack_TREE_H
#define RedBlack_TREE_H

#define DEBUG

#include "General.h"

struct REDBLACK_ROOT;

typedef struct REDBLACK_ROOT* RedBlack_Root;

typedef struct _Object_func {
    COMPARISON_FUNC compare_component;
    COPY_FUNC copy_component;
    DESTRUCTION_FUNC destroy_component;
} RedBlack_Tree_Component_Funcs;

extern RedBlack_Root RedBlack_Tree_Create(RedBlack_Tree_Component_Funcs);
extern void RedBlack_Tree_Destroy(RedBlack_Root*);
extern void RedBlack_Tree_InsertElement(const RedBlack_Root, const Object);
extern Object RedBlack_Tree_DeleteElement(const RedBlack_Root, Object);
extern int RedBlack_Tree_GetNumberOfElements(const RedBlack_Root);
extern Object RedBlack_Tree_GetElement(const RedBlack_Root, const Object);
extern Object RedBlack_Tree_FindLargestElement(const RedBlack_Root);
extern Object RedBlack_Tree_FindSmallestElement(const RedBlack_Root);

#ifdef DEBUG
extern void RedBlack_Tree_PreorderTraversal(const RedBlack_Root);
extern void RedBlack_Tree_InorderTraversal(const RedBlack_Root);
extern void RedBlack_Tree_PostorderTraversal(const RedBlack_Root);
#endif

extern Object RedBlack_Tree_BFS_Search(const RedBlack_Root this, const Object info);

#endif
