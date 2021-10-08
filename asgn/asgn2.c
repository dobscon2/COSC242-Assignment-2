/**
 * @file asgn2.c
 * @author Connor Dobson:   Student_ID: 1043665
 * @author Hayden Knox:     Student_ID: 2485875
 * @author Jakub Sawicki:   Student_ID: 5580766
 * 
 * This program is used to create a new instance of a BST (Binary search tree)
 * or an RBT (Red Black Tree). This program accepts input from the Linux
 * command line terminal. The user provides a text file which is arranged into
 * a tree data structure and assigned the properties of a red-black tree
 * according to the properties of maintaining a correct RBT.
 * 
 * This file is also responsible for printing/displaying important properties
 * of the instantiated BST or RBT. Including but not limited to tree search 
 * time, fill completion time, and the number if word returned as
 * unidentifiable. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include "tree.h"
#include "mylib.h"

/**
 * This constant sets the maximum size of the text file words which can be read 
 * in from the command line as input.
 */
#define WORD_SIZE 256

/**
* Function:
* This function is passed as a parameter to the tree_preorder method and 
* prints the result of the program.
* @param int frequency to detect frequency of a word
* @param char *word, a string for the word to print 
*/

static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

/**
 * Function: 
 * This function uses a tree data structure to perform several tasks. Text
 * words provided as input from a text file are read from stdin and included 
 * in the tree before being displayed adjacent to their respective word input 
 * and their frequency of occurrence. Words are read as input in this function 
 * using the getword() function in the mylib.h file and memory for reading 
 * in as input is allocated once this function is executed and deallocated 
 * once this function finishes its execution. 
 * 
 * The values read in as input are accepted by the optstring variable. each
 * character listed in the opstring variable.
 * 
 * @param argc: 
 * This variable holds the name of the text file that is being read as input 
 * from the terminal command line. This file will be input for the data 
 * structure of either a binary search tree or a red-black tree. The variable 
 * argc represents the total number of command-line arguments
 * @param argv:
 * This variable is the character which the program user enters to perform a 
 * specific operation in the newly created data structure. The variable argv
 * is and an array of strings that contain the command line process
 * arguments.
 * @return:
 * This function returns EXIT_SUCCESS if the program is successful.
 */

int main(int argc, char* argv[]) {
    const char *optstring = "c:df:orh";
    FILE *infile; 
    FILE *outfile;
    char option;
    char word[WORD_SIZE];
    char *searchFile = NULL;
    char *outputFile = NULL;
    int output_to_dot = 0;
    int enable_rbt = 0;
    int print_depth = 0; 
    clock_t fillStart, fillEnd;
    clock_t searchStart, searchEnd;
    int unknown_words = 0;
    tree t;

    while((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'c':
                searchFile = optarg;
                break;
            case 'd':
                print_depth = 1;
                break; 
            case 'f':
                outputFile = optarg;
                break;
            case 'o':
                output_to_dot = 1;
                break;
            case 'r':
                enable_rbt = 1;
                break;
            case 'h':
                fprintf(stderr, "Usage: %s [OPTION]... <STDIN>\n", argv[0]);
                fprintf(stderr, "\n");
                fprintf(stderr, "Perform various operations using a binary tree. By default, words\nare read from stdin and added to the tree, before being printed out\nalongside their frequencies to stdout.\n\n");
                fprintf(stderr, "-c FILENAME\tCheck the spelling of words in FILENAME using words\n\t\tread from stdin as the dictionary. Print timing\n\t\tinfo & unknown words to stderr (ignore -d & -o)\n");
                fprintf(stderr, "-d\t\tOnly print the tree depth (ignore -o)\n");
                fprintf(stderr, "-f FILENAME\tWrite DOT output to FILENAME (if -o given)\n");
                fprintf(stderr, "-o\t\tOutput the tree in DOT form to file 'tree-view.dot'\n");
                fprintf(stderr, "-r\t\tMake the tree an RBT (the default is a BST)\n");
                fprintf(stderr, "\n");
                fprintf(stderr, "-h\t\tPrint this message\n");
                return EXIT_FAILURE;
                break;
            default:
                fprintf(stderr, "Usage: %s [OPTION]... <STDIN>\n", argv[0]);
                fprintf(stderr, "\n");
                fprintf(stderr, "Perform various operations using a binary tree. By default, words\nare read from stdin and added to the tree, before being printed out\nalongside their frequencies to stdout.\n\n");
                fprintf(stderr, "-c FILENAME\tCheck the spelling of words in FILENAME using words\n\t\tread from stdin as the dictionary. Print timing\n\t\tinfo & unknown words to stderr (ignore -d & -o)\n");
                fprintf(stderr, "-d\t\tOnly print the tree depth (ignore -o)\n");
                fprintf(stderr, "-f FILENAME\tWrite DOT output to FILENAME (if -o given)\n");
                fprintf(stderr, "-o\t\tOutput the tree in DOT form to file 'tree-view.dot'\n");
                fprintf(stderr, "-r\t\tMake the tree an RBT (the default is a BST)\n");
                fprintf(stderr, "\n");
                fprintf(stderr, "-h\t\tPrint this message\n");
                return EXIT_FAILURE;
        } 
        
    }
        
    if (enable_rbt == 1) {
        t = tree_new(enable_rbt);
    } else {
        t = tree_new(enable_rbt);
    }

    if (searchFile != NULL) {
        fillStart = clock();
    }
    
    while (getword(word, sizeof word, stdin) != EOF) {
        if (enable_rbt == 1) {
            t = rbt_insert(t, word);
        } else {
            t = tree_insert(t, word);
        }
    }

    if (searchFile != NULL) {
        fillEnd = clock();
    }

    if (searchFile == NULL && print_depth == 0 && output_to_dot == 0 && outputFile == NULL) {
        tree_preorder(t, print_info);
    }
    
    fflush(stdin);

    if (searchFile != NULL) {
        if (NULL == (infile = fopen(searchFile, "r"))) {
            fprintf(stderr, "Can't find file %s\n", optarg); 
            return EXIT_FAILURE;
        } else {
            searchStart = clock();
            while (getword(word, sizeof word, infile) != EOF) {
                if (tree_search(t, word) == 0) {
                    fprintf(stdout, "%s\n", word);
                    unknown_words++;
                }
            }
            searchEnd = clock();
            fclose(infile);

            fprintf(stderr, "Fill time     : %f\n", (fillEnd - fillStart) / (double)CLOCKS_PER_SEC);
            fprintf(stderr, "Search time   : %f\n", (searchEnd - searchStart) / (double)CLOCKS_PER_SEC);
            fprintf(stderr, "Unknown words = %d\n", unknown_words);
        }
    }
   
    if (searchFile == NULL) {
        if (print_depth == 1) {
            int depth = tree_depth(t) - 1;
            printf("%d\n", depth);
        }
    }

    if (searchFile == NULL) {
        if (outputFile != NULL) {
            outfile = fopen(outputFile, "w");
            tree_output_dot(t, outfile);
            fclose(outfile);
            fprintf(stdout, "Successfully created '%s'\n", outputFile);
        } else if (output_to_dot == 1) {
            outfile = fopen("tree-view.dot", "w");
            tree_output_dot(t, outfile);
            fclose(outfile);
            fprintf(stdout, "Successfully created 'tree-view.dot'\n");
        }
    }

    t = tree_free(t);

    return EXIT_SUCCESS;
}
