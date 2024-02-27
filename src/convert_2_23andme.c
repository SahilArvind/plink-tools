#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000

// Function prototypes
void convertAncestryDNA();
void convertFTDNA_MyHeritage(char *file_type);

int main() {
    char file_type[200];

    // Prompting user for file type
    printf("Enter the file type (ancestrydna, ftdna, myheritage): ");
    scanf("%s", file_type);

    // Convert file_type to lowercase
    for (int i = 0; file_type[i]; i++) {
        file_type[i] = tolower(file_type[i]);
    }

    // Checking the file type and calling the appropriate function
    if (strcmp(file_type, "ancestrydna") == 0) {
        convertAncestryDNA();
    } else if (strcmp(file_type, "ftdna") == 0) {
        convertFTDNA_MyHeritage(file_type);
        // Run the python script after converting ftdna file
        system("python sort.py");
    } else if (strcmp(file_type, "myheritage") == 0) {
        convertFTDNA_MyHeritage(file_type);
    } else {
        printf("Invalid file type.\n");
        return 1;
    }

    return 0;
}

void convertAncestryDNA() {
    char input_file_name[MAX_LINE_LENGTH], output_file_name[MAX_LINE_LENGTH];

    // Prompting user for input file name
    printf("Enter the name of the input file: ");
    scanf("%s", input_file_name);

    FILE *input_file = fopen(input_file_name, "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        exit(1);
    }

    // Prompting user for output file name
    printf("Enter the name of the output file: ");
    scanf("%s", output_file_name);

    FILE *output_file = fopen(output_file_name, "w");
    if (output_file == NULL) {
        perror("Error creating output file");
        fclose(input_file);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];

    // Read each line from the input file
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
        // Make a copy of the line for case-insensitive comparison
        char lowercase_line[MAX_LINE_LENGTH];
        strcpy(lowercase_line, line);
        // Convert the copy to lowercase for case-insensitive comparison
        for (int i = 0; lowercase_line[i]; i++) {
            lowercase_line[i] = tolower(lowercase_line[i]);
        }

        // Check if the line contains strings like "rsid" or "#"
        if (strstr(lowercase_line, "rsid") != NULL || strstr(lowercase_line, "#") != NULL) {
            // If the line contains such strings, skip writing it to the output file
            continue; // Skip processing for this line
        }

        char *token = strtok(line, "\t");
        int column_count = 1;
        int both_elements_zero = 0; // Flag to indicate if both 4th and 5th elements are 0

        while (token != NULL) {
            // Check if the current token is the 4th column
            if (column_count == 4) {
                // Check if the 4th element is 0
                both_elements_zero = (strcmp(token, "0") == 0);
            } else if (column_count == 5) {
                // Check if the 5th element is 0 and update flag only if 4th element was also 0
                both_elements_zero = both_elements_zero && (strcmp(token, "0") == 0);
            }

            // Write the appropriate output based on conditions
            if (column_count == 4 || column_count == 5) {
                if (both_elements_zero) {
                    fprintf(output_file, "--\n"); // Write "--" if both elements are 0
                    break;
                } else {
                    fprintf(output_file, "%s", token); // Write the token otherwise
                }
            } else {
                fprintf(output_file, "%s\t", token); // Write other columns as they are
            }

            token = strtok(NULL, "\t");
            column_count++;
        }
    }

    fclose(input_file);
    fclose(output_file);

    printf("Output file '%s' generated successfully.\n", output_file_name);
}

void convertFTDNA_MyHeritage(char *file_type) {
    char inputFileName[MAX_LINE_LENGTH], outputFileName[MAX_LINE_LENGTH];
    FILE *inputFile, *outputFile;
    char line[MAX_LINE_LENGTH];

    // Prompting user for input file name
    printf("Enter the input file name: ");
    scanf("%s", inputFileName);

    // Opening input file
    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Error opening input file.\n");
        exit(1);
    }

    // Default output file name for FTDNA
    strcpy(outputFileName, "data.txt");

    // For MyHeritage, prompt user for output file name
    if (strcmp(file_type, "myheritage") == 0) {
        printf("Enter the name of the output file: ");
        scanf("%s", outputFileName);
    }

    // Opening output file
    outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("Error creating output file.\n");
        fclose(inputFile);
        exit(1);
    }

    // Read each line from the input file
    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {
        // Check if the line contains strings like "RSID" or "#"
        if (strstr(line, "RSID") != NULL || strstr(line, "#") != NULL) {
            // If the line contains such strings, skip writing it to the output file
            continue; // Skip processing for this line
        }

        // Replace commas with tabs
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == ',') {
                line[i] = '\t';
            }
        }

        // Eliminate double quotes
        char *src = line, *dst = line;
        do {
            *dst = *src++;
            dst += (*dst != '"');
        } while (*dst != '\0');

        // Write the processed line to the output file
        fputs(line, outputFile);
    }

    // Closing files
    fclose(inputFile);
    fclose(outputFile);

    if (strcmp(file_type, "myheritage") == 0) {
        printf("File conversion done successfully!\n");
    }
}