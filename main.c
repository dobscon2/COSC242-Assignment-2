#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include "tree.h"
#include "mylib.h"

/* prints the end result of the program.
   @param int frequency to detect frequency of a word
   @param char *word, a string for the word to print 
*/
static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

int main(int argc, char* argv[]) {
    const char *optstring = "c:df:orh"; /* a string determining the different character choices you can make. */
    char option;
    char word[256];
    FILE *infile; 
    FILE *outfile;
    char *searchFile = NULL;
    int enable_rbt = 0;
    int print_depth = 0;
    char *outputFile = NULL;
    int output_to_dot = 0;

    clock_t fillStart, fillEnd;
    clock_t searchStart, searchEnd;
    int unknown_words = 0;

    tree t;

    while((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'c': /* for activating the timer */
                searchFile = optarg;
                break;
            case 'd': /* print the depth of the tree */
                print_depth = 1;
                break; 
            case 'f': /* write the dot output to filename */
                outputFile = optarg;
                break;
            case 'o': /* output the representation of the tree in dot form */
                output_to_dot = 1;
                break;
            case 'r': /* create an RBT instead of a BST */
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
        /* Create a new tree with RBT characteristics */
        t = tree_new(enable_rbt);
    } else {
        /* Create a new tree with BST characteristics */
        t = tree_new(enable_rbt);
    }

    if (searchFile != NULL) { /* start the timer! */
        fillStart = clock();
    }
    
    while (getword(word, sizeof word, stdin) != EOF) {
        t = tree_insert(t, word);
    }

    if (searchFile != NULL) {
        fillEnd = clock();
    }

    if (searchFile == NULL && print_depth == 0 && output_to_dot == 0 && outputFile == NULL) {
        tree_preorder(t, print_info);
    }
    
    fflush(stdin);

    if (searchFile != NULL) {
        if (NULL == (infile = fopen(searchFile, "r"))) { /* if we can't find the file to search, end program with a fail */
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
            fprintf(stderr, "Unknwon words = %d\n", unknown_words);
        }
    }

    if (searchFile == NULL) {
        if (print_depth == 1) {
            int depth = maxDepth(t) - 1;
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
