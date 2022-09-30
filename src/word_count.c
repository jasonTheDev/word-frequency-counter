#include "word_count.h"

int 
main(int argc, char *argv[]) {

	// variables for checking command line arguments
	int sort_flag = 0;
	int infile_flag = 0;
	char *filepath;

	// variables for reading file and creating lengths list
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	size_t nread;
	length_node *lengths = NULL;

	/*
	 * check for valid command line arguments
	 */ 
	if (argc < 3) {
		fprintf(stderr, ARG_ERROR);
		exit(ARG_ERROR_CODE);
	}
	for (int i=1; i<argc; i++) {
		char *arg = argv[i];
		if (strcmp(arg, SORT_ARG) == 0) {
			sort_flag = 1;
		}
		else if (strcmp(arg, INFILE_ARG) == 0) {
			infile_flag = 1;
			if (i+1 < argc) {
				filepath = argv[i+1];
			} else {
				fprintf(stderr, ARG_ERROR);
				exit(ARG_ERROR_CODE);
			}
		}
	}
	if (infile_flag == 0) {
		fprintf(stderr, ARG_ERROR);
		exit(ARG_ERROR_CODE);
	}
	
	/*
	 * read file and create lengths list
	 */
	fp = fopen(filepath, "r");
	if (fp == NULL) {
		fprintf(stderr, "program: cannot open file %s\n", filepath);
		exit(FILE_ERROR_CODE);
	}
	while ((nread = getline(&line, &len, fp)) != -1) {
		char *token = strtok(line, DELIM);
		while (token != NULL) {
			lengths = add_length_inorder(lengths, token, sort_flag);
			token = strtok(NULL, DELIM);
		}
	}
	if (line) {
		free(line);
	}
	fclose(fp);

	/*
	 * print histogram
	 */
	print_counts(lengths);

	/*
	 * free memory used in linked list
	 */
	cleanup_lengths(lengths);

	return SUCCESS_CODE;
}

