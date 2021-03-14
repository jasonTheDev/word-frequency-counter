/*
 * linkedlist.h
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define _GNU_SOURCE
#define SUCCESS_CODE 0
#define ARG_ERROR_CODE 1
#define FILE_ERROR_CODE 2
#define MALLOC_ERROR_CODE 3
#define SORT_ARG "--sort"
#define INFILE_ARG "--infile"
#define ARG_ERROR "program: missing '--infile <filename> [--sort]'\n"
#define DELIM " .,:;()\n\t"

// Word_Node struct
typedef struct Word_Node {
    char *word;
    struct Word_Node *next;
}word_node;

// Length_Node struct
typedef struct Length_Node {
    int length;
    int count;
    word_node *words;
    struct Length_Node *next;
}length_node;


size_t getline(char **lineptr, size_t *n, FILE *stream);    // doesn't compile -std=c99 without this

void *emalloc(size_t n);
length_node *new_length(char *word);
length_node *add_length_inorder(length_node *list, char *word, int desc);
word_node *new_word(char *word);
word_node *add_word_inorder(word_node *words, char *word, int desc);
void cleanup_words(word_node *list);
void cleanup_lengths(length_node *list);
void print_counts(length_node *list);

// void print_words(word_node *list);   // for testing
// void print_all(length_node *list);


/* Dynamically allocate memory of size_t n and return the 
 * pointer to the memory address
 *
 * exit with return code on allocation error 
 */
void *emalloc(size_t n) {
    void *p;

    p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "program: malloc of %zu bytes failed", n);
        exit(MALLOC_ERROR_CODE);
    }

    return p;
}

/* Given a word, initialize a new length node
 *
 * return the new length node
 */
length_node *new_length(char *word) {

    length_node *new_len = (length_node *) emalloc(sizeof(length_node));
    new_len->length = (int) strlen(word);
    new_len->count = 1;
    new_len->next = NULL;
    // initialize words list
    new_len->words = new_word(word);

    return new_len;
}

/* Given a word, add it to the list of lengths
 *
 * return the lengths list
 */
length_node *add_length_inorder(length_node *list, char *word, int desc) {
    assert(word != NULL);

    int len = (int) strlen(word);
    length_node *curr = NULL;
    length_node *prev = NULL;

    if (list == NULL) {
        return new_length(word);
    }
    for (curr = list; curr != NULL; curr = curr->next) {
        if (curr->length < len) {
            prev = curr;
        }
        else if (curr->length == len) {
            curr->count++;
            curr->words = add_word_inorder(curr->words, word, desc);
            return list;
        }
        else {
            break;
        }
    }

    length_node *new_len = new_length(word);
    new_len->next = curr;
    if (prev == NULL) {
        return new_len;
    } else {
        prev->next = new_len;
        return list;
    }
}

/* Given a word, initialize a new word node
 *
 * return the new word node
 */
word_node *new_word(char *word) {

    word_node *new_word = (word_node *) emalloc(sizeof(word_node));
    new_word->word = (char *) emalloc(strlen(word) + 1);
    strcpy(new_word->word, word);
    new_word->next = NULL;

    return new_word;
}

/* Add a word to the word_node list in order
 * If desc is set to 1 add in descending order
 *
 * return the words list
 */
word_node *add_word_inorder(word_node *words, char *word, int desc) {

    word_node *curr = NULL;
    word_node *prev = NULL;

    if (words == NULL) {
        return new_word(word);
    }
    for (curr = words; curr != NULL; curr = curr->next) {
        if (strcmp(curr->word, word) < 0 && desc==0) {
            prev = curr;
        }
        else if (strcmp(curr->word, word) > 0 && desc==1) {
            prev = curr;
        }
        else if (strcmp(curr->word, word) == 0) {
            return words;
        }
        else {
            break;
        }
    }

    word_node *new_w = new_word(word);
    new_w->next = curr;
    if (prev == NULL) {
        return new_w;
    } else {
        prev->next = new_w;
        return words;
    }
}

/* Free malloc'ed word_nodes
 *
 */
void cleanup_words(word_node *list) {

    if (list == NULL) {
        return;
    }
    cleanup_words(list->next);
    free(list->word);
    free(list);
}

/* Free malloc'ed lengths_nodes
 *
 */
void cleanup_lengths(length_node *list) {

    if (list == NULL) {
        return;
    }
    cleanup_lengths(list->next);
    cleanup_words(list->words);
    free(list);
}

/* Print counts of word lengths and unique words
 *
 */
void print_counts(length_node *list) {

    if (list == NULL) {
        return;
    }
    printf("Count[%02d]=%02d; ", list->length, list->count);
    printf("(words: ");

    word_node *curr = NULL;
    for (curr = list->words; curr != NULL; curr = curr->next) {
        if (curr->next == NULL) {
            printf("\"%s\"", curr->word);
        }
        else if (curr->next->next == NULL) {
            printf("\"%s\" and ", curr->word);
        }
        else {
            printf("\"%s\", ", curr->word);
        }
    }
    printf(")\n");
    print_counts(list->next);
}


/***********************************************************
 ***********************************************************
 * The following functions are not used in the final program
 * They are usefull for testing purposes
 */

// print all words in a word_node list
void print_words(word_node *list) {

    if (list == NULL) {
        return;
    }
    printf("word in node: %s\n", list->word);
    print_words(list->next);
}

// print all members in a length_node list
void print_all(length_node *list) {

    if (list == NULL) {
        return;
    }

    printf("length: %d\n", list->length);
    printf("count: %d\n", list->count);
    print_words(list->words);
    print_all(list->next);
}