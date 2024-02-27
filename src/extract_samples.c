#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024
#define MAX_WORDS 50  // Maximum number of words to find, adjust as needed

// Function to perform case-insensitive string search
char* strcasestr(const char* haystack, const char* needle) {
    int i, j;
    for (i = 0; haystack[i] != '\0'; i++) {
        j = 0;
        while (needle[j] != '\0' && tolower(haystack[i + j]) == tolower(needle[j])) {
            j++;
        }
        if (needle[j] == '\0') {
            return (char*)&haystack[i];
        }
    }
    return NULL;
}

// Function to remove the file extension from a file name
void removeFileExtension(char* fileName) {
    char *dot = strrchr(fileName, '.');
    if (dot != NULL) {
        *dot = '\0';
    }
}

int main() {
    FILE *inputFile, *outputFile;
    char inputFileName[MAX_LINE_LENGTH];  // buffer to store the input file name
    char outputFileName[] = "keep.txt";  // replace with your output file name
    char wordsToFind[MAX_WORDS][MAX_LINE_LENGTH];  // buffer to store multiple words
    int numWords;

    // Get user input for the input file name
    printf("Enter the input file name: ");
    scanf("%s", inputFileName);

    // Get user input for multiple words separated by commas
    printf("Enter words to find (separated by commas): ");
    getchar();  // Consume the newline character left in the input buffer
    char inputWords[MAX_LINE_LENGTH * MAX_WORDS];  // Increased the size to accommodate more words
    fgets(inputWords, sizeof(inputWords), stdin);

    // Tokenize the input string to extract individual words
    char *token = strtok(inputWords, ", \n");
    numWords = 0;

    while (token != NULL && numWords < MAX_WORDS) {
        strcpy(wordsToFind[numWords], token);
        token = strtok(NULL, ", \n");
        numWords++;
    }

    // Open the input file for reading
    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // Open the output file for writing
    outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile);
        return 1;
    }

    char line[MAX_LINE_LENGTH];

    // Read lines from the input file
    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {
        // Check if any part of the words is present in the line (case-insensitive)
        for (int i = 0; i < numWords; i++) {
            if (strcasestr(line, wordsToFind[i]) != NULL) {
                // Write the line to the output file
                fputs(line, outputFile);
                break;  // Move to the next line after finding a match
            }
        }
    }

    // Close the files
    fclose(inputFile);
    fclose(outputFile);

    printf("Lines containing the specified words have been written to '%s'.\n", outputFileName);

    // Remove file extension from inputFileName
    removeFileExtension(inputFileName);

    // Run plink.exe command
    char plinkCommand[MAX_LINE_LENGTH * 2];
    #ifdef _WIN32
    snprintf(plinkCommand, sizeof(plinkCommand), "plink.exe --bfile %s --keep %s --allow-no-sex --make-bed --out extracted > NUL 2>&1", inputFileName, outputFileName);
    #else
    snprintf(plinkCommand, sizeof(plinkCommand), "plink.exe --bfile %s --keep %s --allow-no-sex --make-bed --out extracted > /dev/null 2>&1", inputFileName, outputFileName);
    #endif
    system(plinkCommand);

    return 0;
}