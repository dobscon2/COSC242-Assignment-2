#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "mylib.h"

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))
static tree_t tree_type;

struct treenode{
    char *key; /* a string of characters that represents the node */
    tree_colour colour; /* the colour of the node in the tree */
    tree left; 
    tree right;
    int frequency; /*update whenever duplicate item is added*/
};

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

static tree left_rotate(tree t) {
    tree temp;
    temp = t;
    t = t->right;
    temp->right = t->left;
    t->left = temp;
    return t;
}

static tree right_rotate(tree t) {
    /*same as left but swap left for right*/
    tree temp; 
    temp = t; 
    t = t->left; 
    temp->left = t->right;
    t->right = temp;
    return t;
}

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

int maxDepth(tree t) { /* finds the maximum depth of the tree */
    int leftDepth, rightDepth;
    if (t == NULL) {
        return 0;
    }
        leftDepth = maxDepth(t->left); /* finds all the nodes left and makes them equal to an int */
        rightDepth = maxDepth(t->right); /* finds all the nodes right and makes them equal to an int */
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
