#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static int strlengther(char *theString)
{
	int i;
	for (i = 0; theString[i] != '\0'; i++)
		;
	return i;
}

int main(int argc, char *argv[])
{
	if (argc > 3 || argc < 2) {
		fprintf(stderr, "usage: reverse <input> <output>\n");
		return 1;
	}

	if (argc == 3) {
		int L1 = strlengther(argv[1]);
		int L2 = strlengther(argv[2]);

		if (L1 == L2)
		{
			if (!strncmp(argv[1], argv[2], L1))
			{
				fprintf(stderr, "error: input and output file must differ\n");
				return 1;
			}
		}
	}

	// declare any variable we'll need
	FILE *theFile;
	theFile = fopen(argv[1], "r");

	if (theFile == NULL) {
		fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
		return 1;
	}

	// process file information
	char* trash_buff = NULL;
	char* buff = NULL;
	size_t buffer_size = 0;
	char **lines;
	// need to know how much memory to malloc (No. lines x sizeof(ptr))
	int numLines = 0;
	while (getline(&trash_buff, &buffer_size, theFile) != -1) {
		numLines++;
	}

	free(trash_buff);

	// reset file pointer to beginning of file
	rewind(theFile);
	
	// close then re-open file to reset file pointer
	fclose(theFile);
	theFile = fopen(argv[1], "r");

	// allocate memory for the lines of the file
	lines = (char**) malloc(numLines * sizeof(char*));
	
	if (lines == NULL) {
		fprintf(stderr, "error: malloc failed\n");
		return 1;
	}

	int i = 0;

	while (getline(&buff, &buffer_size, theFile) != -1)
	{
		// save each line into an array
		// lines[i] = (char*) malloc(sizeof(buff));
		lines[i] = strdup(buff);
		if (lines[i] == NULL) {
			fprintf(stderr, "error: malloc failed\n");
			return 1;
		}
		i++;
	}

	free(buff);
	fclose(theFile);

	if (argc == 3) {
		// write the lines (reversed) to the outfile file stream
		FILE * oFile;
		oFile = fopen(argv[2], "w");
		if (oFile == NULL) {
			fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
			return 1;
		}

		for (int j = numLines - 1; j > -1; j--)
		{	
			fprintf(oFile, "%s", lines[j]);
		}
		fclose(oFile);
	} else {
		// print the lines out, but reversed
		for (int j = numLines - 1; j > -1; j--) {
			printf("%s", lines[j]);
		}
	}
	
	for (int k = 0; k < numLines; k++) {
		free(lines[k]);
	}

	free(lines);
	return 0;
}