/*****************************************************************************
 * @file tree.c
 * @author Connor Dobson:   Student_ID: 1043665
 * @author Hayden Knox:     Student_ID: 2485875
 * @author Jakub Sawicki:   Student_ID:  
 * 
 * This program is provides all of the function which are required to produce 
 * a new instance of a BST (Binary search tree) or a RBT (Red Black Tree) 
 * and perform the required functions on a created tree data structure. 
 * This provides methods such as tree_new(), tree_insert(), right_rotate(), 
 * left_rotate(), tree_search(), tree_depth(), tree_fix(), tree_preorder(). 

 * 
 * This file is also provides functions for creating dot representations of
 * created BST or RBT data structures.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "mylib.h"

/*****************************************************************************
 * IS_BLACK & IS_RED indicate state of tree data strucutre node colour.
 * These statments evaluate the colour individual tree node retains,
 * the use of these statments are  in the tree_fix function.
 *****************************************************************************/

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

static tree_t tree_type;

struct tree_node{
    char *key; /* a string of characters that represents the node */
    tree_colour colour; /* the colour of the node in the tree */
    tree left; 
    tree right;
    int frequency; /*update whenever duplicate item is added*/
};
/*****************************************************************************
 * Function: tree_new()
 * @param: int input
 * This is a variable of an int type is used to indicate if the to be created data 
 * structure is a BST or an RBT.
 * Output: 
 * A tree data structure, ethier a RBT or a BST.
 * Procedure: Allocates memory to the tree data structure; 
 * @return A null tree data structure (nod nodes or associated key values).
 *****************************************************************************/
tree tree_new(int input) { 
    if (input == 0) { 
        tree_type = BST;
    } else if (input == 1) {
        tree_type = RBT;
    }
    return NULL; /*creates an empty tree*/
}

tree tree_insert(tree t, char *str) {
    int s;
    if(t == NULL){ /* if we are starting with an empty tree */
        t = emalloc(sizeof *t); /* allocates memory for the tree */
        t->key = emalloc((strlen(str)+1) * sizeof t->key[0]); /* allocates memory for the length of the key */
        t->right = NULL;
        t->left = NULL;
        t->frequency = 1;
        if (tree_type == RBT) { 
            t->colour = RED; /* set default tree colour to red, before we call tree_fix */
        }
        strcpy(t->key,str); /* creates the first key by copying str to t->key */
    } else { /* if our tree has more than zero elements */
        s = strcmp(str, t->key); /* compares the two strings to see whether they are identical */
        if(s == 0){ /* if we have an identical string */
            t->frequency++; /*updates when duplicate item is added*/
        } else if(s < 0){  
            t->left = tree_insert(t->left, str); /* adds left */
        } else if(s > 0){
            t->right = tree_insert(t->right,str); /* adds right */
        }
    }
    
    if (tree_type == RBT) {
        t = tree_fix(t);  /* will allocate the correct colour to a tree node */
        return t; 
    } else {
    return t;
    }
}

/*****************************************************************************
 * Function: left_rotate()
 * @param: tree t
 * This is a variable of data structure either a BST or an RBT.
 * Output: 
 * A tree data structure, ethier a RBT or a BST.
 * Procedure: This method left rotates the tree data structure 
 * @return: A node arrangment modified version of the input tree data structure. 
 *****************************************************************************/
static tree left_rotate(tree t) {
    tree temp;
    temp = t;
    t = t->right;
    temp->right = t->left;
    t->left = temp;
    return t;
}

/*****************************************************************************
 * Function: right_rotate()
 * @param: tree t
 * This is a variable of data structure either a BST or an RBT.
 * Output: 
 * A tree data structure, ethier a RBT or a BST.
 * Procedure: This method right rotates the tree data structure 
 * @return: A node arrangment modified version of the input tree data structure. 
 *****************************************************************************/
static tree right_rotate(tree t) {
    /*same as left but swap left for right*/
    tree temp; 
    temp = t; 
    t = t->left; 
    temp->left = t->right;
    t->right = temp;
    return t;
}

/*****************************************************************************
 * Function: tree_search()
 * @param: tree t
 * This is a variable of data structure either a BST or an RBT.
 * @param: str* 
 * The varialbe *str is a pointer to the char value which is passed as a parameter
 * for the data structure traversal. 
 * Output: and int data type to a recursive function call.
 * A tree data structure, ethier a RBT or a BST.
 * Procedure: 
 * The tree_search() method is used to traverse the input tree data structure and
 * reading input by evaluating key char variable values to the keys strings present 
 * withing the tree data structures current tree node. There are three cases which 
 * can executed depending on the various if statements. If the if() comparison
 * returns true for any if staement. The if() statement returns a recursive method call
 * for either left or right tree key node left or right subtree traversal.
 * If the traversal search() function encounters a NUll tree or subtree it will 
 * return 0 as this value is indicative of an intraversable path. If the functions 
 * traverses to a node and its key matches the value provided as input the function 
 * returns 1. If the nodes key exceeds the character asci character input
 * key we return the result of searching the left subtree. If the key is to 
 * small the method returns the result of searching the right subtree.
 * @return A recursive tree_search() call to either the left or right subtree of a
 * data structure.
 *****************************************************************************/
int tree_search(tree t, char *str){
    if (t == NULL) { /* there will be nothing to search */
        return 0;
    }

    if (strcmp(str, t->key) == 0) { /* if our comparison is the same, return success */
        return 1;
    } else if (strcmp(str, t->key) < 0) {
        return tree_search(t->left, str); /* search to the left */
    } else { /* if all else fails, we must search right */
        return tree_search(t->right, str);
    }
}

tree rbt_insert(tree t, char *str) {
    t = tree_insert(t, str);
    t->colour = BLACK;
    return t;
}

/*****************************************************************************
 * Function: tree_depth()
 * @param: tree t
 * This is a variable of data structure either a BST or an RBT.
 * Output  : int
 * Procedure: This function traverses the left and right subtrees of the tree 
 * data structure and using recursive calls to the tree_depth function 
 * increments the total node sum for both the left and right subtrees. Then 
 * afterwars compars the total values for the depth of the left and right 
 * subtrees returns the maximum int value of either the left or right subtree
 * data structure.
 * @return the maximum tree depth value of ether the left or right subtree
 * data structure. 
 *****************************************************************************/
int tree_depth(tree t) { /* finds the maximum depth of the tree */
    int leftDepth, rightDepth;
    if (t == NULL) {
        return 0;
    }
        leftDepth =  tree_depth(t->left); /* finds all the nodes left and makes them equal to an int */
        rightDepth = tree_depth(t->right); /* finds all the nodes right and makes them equal to an int */
    if (leftDepth > rightDepth) { /* if the left side of the tree is deeper */
        return leftDepth + 1;
    } else { /* if the right side of the tree is deeper */
        return rightDepth + 1;
    }
}

tree tree_fix (tree t) {
    if (IS_RED(t->left) && IS_RED(t->left->left)) {
        if (IS_RED(t->right)) {
            t->colour = RED;
            t->left->colour = BLACK;
            t->right->colour = BLACK;
        } else if(IS_BLACK(t->right)) {
            t = right_rotate(t);
            t->colour = BLACK;
            t->right->colour = RED;
        }
    } else if (IS_RED(t->left) && IS_RED(t->left->right)) {
        if (IS_RED(t->right)) {
            t->colour = RED;
            t->left->colour = BLACK;
            t->right->colour = BLACK;
        } else if (IS_BLACK(t->right)) {
            t->left = left_rotate(t->left);
            t = right_rotate(t);
            t->colour = BLACK;
            t->right->colour = RED;
        }
    } else if (IS_RED(t->right) && IS_RED(t->right->left)) {
        if (IS_RED(t->left)) {
            t->colour = RED;
            t->left->colour = BLACK;
            t->right->colour = BLACK;
        } else if (IS_BLACK(t->left)) {
            t->right = right_rotate(t->right);
            t = left_rotate(t);
            t->colour = BLACK;
            t->left->colour = RED;
        }
    } else if (IS_RED(t->right) && IS_RED(t->right->right)) {
        if (IS_RED(t->left)) {
            t->colour = RED;
            t->left->colour = BLACK;
            t->right->colour = BLACK;
        } else if (IS_BLACK(t->left)) {
            t = left_rotate(t);
            t->colour = BLACK;
            t->left->colour = RED;
        }
    }

    /* Here after all the tree errors and cases have been resolved.
    * The alterations to the root red node can be resolved here. 
    */
    return t;
}

void tree_preorder(tree t, void f(int freq, char *str)){
    if(t == NULL){
        return;
    }
    f(t->frequency, t->key);
    tree_preorder(t->left, f);
    tree_preorder(t->right, f);
}

/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
static void tree_output_dot_aux(tree t, FILE *out) {
    if(t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
                t->key, t->key, t->frequency,
                (RBT == tree_type && RED == t->colour) ? "red":"black");
    }
    if(t->left != NULL) {
        tree_output_dot_aux(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if(t->right != NULL) {
        tree_output_dot_aux(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}

/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
    fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
    tree_output_dot_aux(t, out);
    fprintf(out, "}\n");
}

/*****************************************************************************
 * Function: tree_free()
 * @param: tree t
 * This is variable of an instance of a tree data structure which has already 
 * been created.
 * Output: 
 * A tree data structure. with no data.
 * Procedure: This function traverses the created tree and dallocates memory to
 * each tree node in the data structure.
 *****************************************************************************/
tree tree_free(tree t){
    if (t == NULL){ /* if there's nothing to free, do nothing */
        return NULL;
    }
    tree_free(t->right);
    tree_free(t->left);
    free(t->key);
    free(t);

    return NULL;
}
