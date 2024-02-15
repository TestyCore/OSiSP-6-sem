#include <stdio.h>
#include <string.h>
#include "reverse.h"

int main(int argc, char *argv[]) {
    char line[100];
    FILE *output_file = fopen(argv[1], "w"); 
    
    if (output_file == NULL) {
        perror("Error opening output file");
        return 1; 
    }

    while (fgets(line, sizeof(line), stdin) != NULL) {
        char *newline = strchr(line, '\n');
        if (newline != NULL) {
            *newline = '\0';
        }

        reverseString(line);
        fprintf(output_file, "%s\n", line); 
    }

    fclose(output_file); 

    return 0; 
}
