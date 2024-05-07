#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define MAX_LENGTH 200

void processFamFiles(const char *file1_path, const char *file2_path, const char *output_path) {
    FILE *file1, *file2, *output_file;
    char line1[MAX_LENGTH], line2[MAX_LENGTH], label[MAX_LENGTH], restOfLine[MAX_LENGTH];
    char secondString[MAX_LENGTH], thirdString[MAX_LENGTH], fourthString[MAX_LENGTH], fifthString[MAX_LENGTH], sixthString[MAX_LENGTH];

    file1 = fopen(file1_path, "r");
    file2 = fopen(file2_path, "r");
    output_file = fopen(output_path, "w");

    if (file1 == NULL || file2 == NULL || output_file == NULL) {
        printf("Error opening files.\n");
        return;
    }

    while (fgets(line1, sizeof(line1), file1) && fgets(line2, sizeof(line2), file2)) {
        // Extract the third string from file1
        sscanf(line1, "%*s %*s %s", label);

        // Extract the second, third, fourth, fifth, and sixth strings from file2
        sscanf(line2, "%*s %s %s %s %s %s", secondString, thirdString, fourthString, fifthString, sixthString);

        // Write to output file
        fprintf(output_file, "%s\t%s %s %s %s %s\n", label, secondString, thirdString, fourthString, fifthString, sixthString);
    }

    fclose(file1);
    fclose(file2);
    fclose(output_file);

    printf("Output file created successfully.\n");
}

void processIndFiles(const char *file1_path, const char *file2_path, const char *output_path) {
    FILE *file1, *file2, *output_file;
    char line1[MAX_LENGTH], line2[MAX_LENGTH], firstString1[MAX_LENGTH], secondString1[MAX_LENGTH], firstString2[MAX_LENGTH], secondString2[MAX_LENGTH];

    file1 = fopen(file1_path, "r");
    file2 = fopen(file2_path, "r");
    output_file = fopen(output_path, "w");

    if (file1 == NULL || file2 == NULL || output_file == NULL) {
        printf("Error opening files.\n");
        return;
    }

    while (fgets(line1, sizeof(line1), file1) && fgets(line2, sizeof(line2), file2)) {
        // Extract the first and second strings from file1
        sscanf(line1, "%s %s", firstString1, secondString1);

        // Extract the first and second strings from file2
        sscanf(line2, "%s %s", firstString2, secondString2);

        // Write to output file
        fprintf(output_file, "%s %s %s\n", firstString2, secondString2, firstString1);
    }

    fclose(file1);
    fclose(file2);
    fclose(output_file);

    printf("Output file created successfully.\n");
}

int main(int argc, char *argv[]) {
    int opt;
    char *file1_path = NULL, *file2_path = NULL, *output_path = NULL;
    int indFlag = 0, famFlag = 0;

    while ((opt = getopt(argc, argv, "i:d:o:-:")) != -1) {
        switch (opt) {
            case 'i':
                file1_path = optarg;
                break;
            case 'd':
                file2_path = optarg;
                break;
            case 'o':
                output_path = optarg;
                break;
            case '-':
                if (strcmp(optarg, "ind") == 0) {
                    indFlag = 1;
                } else if (strcmp(optarg, "fam") == 0) {
                    famFlag = 1;
                } else {
                    printf("Unknown option: %s\n", optarg);
                    return 1;
                }
                break;
            default:
                printf("Usage: %s -i <file1> -d <file2> -o <outputfile> --ind/--fam\n", argv[0]);
                return 1;
        }
    }

    if ((indFlag && famFlag) || (!indFlag && !famFlag)) {
        printf("Please specify either --ind or --fam option.\n");
        return 1;
    }

    if (file1_path == NULL || file2_path == NULL || output_path == NULL) {
        printf("Error: Please provide input and output file paths.\n");
        printf("Usage: %s -i <file1> -d <file2> -o <outputfile> --ind/--fam\n", argv[0]);
        return 1;
    }

    if (indFlag) {
        processIndFiles(file1_path, file2_path, output_path);
    } else if (famFlag) {
        processFamFiles(file1_path, file2_path, output_path);
    }

    return 0;
}