/*
Use cases

./my-zip <file> <file> ... Writes all file compressions to standard output
./my-zip <file> <file> ... > <input_file> Writes all file compressions to file

Additional help from:
Github CoPilot

For testing purposes copy pasta:
valgrind --leak-check=full ./my-zip test.txt

Author:
@AleksiLe
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2)
    {
        fprintf(stderr, "my-zip: file1 [file2 ...]\n");
        exit(1);
    }
    for (int i = 1; i < argc; i++) {
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            fprintf(stderr, "Error opening input file");
            exit(1);
        }

        int current_char = fgetc(file);
        int count = 1;

        while (current_char != EOF) {
            int next_char = fgetc(file);

            if (next_char == current_char) {
                count++;
            } else {
                fwrite(&count, sizeof(count), 1, stdout); // Write count in binary format
                fputc(current_char, stdout); // Write character in ASCII format
                count = 1;
                current_char = next_char;
            }
        }
        fclose(file);
    }
    return 0;
}