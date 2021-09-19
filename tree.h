/*****************************************************************************
 * @file tree.h
 * @author Connor Dobson:   Student_ID: 
 * @author Hayden Knox:     Student_ID: 2485875
 * @author Jakub Sawicki:   Student_ID:  
 * 
 * This program is provides access to all of the functions which are required 
 * to produce a new instance of a BST (Binary search tree) or a RBT 
 * (Red Black Tree) data structure to be used in the programs main.c file. 
 * This provides access to methods such as tree_new(), tree_insert(), 
 * right_rotate(), left_rotate(), tree_search(), tree_depth(), tree_fix(),
 * tree_preorder(). 
 *****************************************************************************/

#ifndef TREE_H_
#define TREE_H_

typedef struct tree_node *tree;
typedef enum tree_e { BST, RBT } tree_t;
typedef enum { RED, BLACK } tree_colour;
extern tree tree_free(tree t);
extern void tree_inorder(tree t, void f(char *str));
extern void tree_preorder(tree t, void f(int freq, char *str));
extern tree tree_insert(tree t, char *str);
extern tree tree_new();
extern tree tree_fix();
extern int tree_depth(tree t);
extern int tree_search(tree t, char *str);
extern void tree_output_dot(tree t, FILE *out);

#endif

