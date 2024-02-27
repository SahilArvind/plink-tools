#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define DEV_NULL "NUL"
#else
#define DEV_NULL "/dev/null"
#endif

#define MAX_FILENAME_LENGTH 256
#define MAX_COMMAND_LENGTH 512

void run_command(const char *command) {
    int status = system(command);
    if (status != 0) {
        fprintf(stderr, "Error executing command: %s\n", command);
        exit(EXIT_FAILURE);
    }
}

void delete_files() {
    remove("add.bed");
    remove("add.bim");
    remove("add.fam");
    remove("add.hh");
    remove("add.log");
    remove("add.nosex");
    remove("basic.bed");
    remove("basic.bim");
    remove("basic.fam");
    remove("basic.hh");
    remove("basic.log");
    remove("basic.nosex");
    remove("basic_tmp.bed");
    remove("basic_tmp.bim");
    remove("basic_tmp.fam");
    remove("basic_tmp.hh");
    remove("basic_tmp.log");
    remove("basic_tmp.nosex");
}

int main() {
    char raw_file[MAX_FILENAME_LENGTH];
    char base_dataset[MAX_FILENAME_LENGTH];
    char output_dataset[MAX_FILENAME_LENGTH];

    // Step 1: Take user input for raw file name
    printf("Enter the raw file name (without extension): ");
    scanf("%s", raw_file);

    // Construct and run command a)
    char command_a[MAX_COMMAND_LENGTH];
    snprintf(command_a, MAX_COMMAND_LENGTH, "plink.exe --allow-no-sex --23file %s.txt --make-bed --out add > " DEV_NULL " 2>&1", raw_file);
    run_command(command_a);

    // Construct and run command b)
    run_command("plink.exe --bfile add --extract plink.snplist --make-bed --allow-no-sex --out basic > " DEV_NULL " 2>&1");

    // Step 2: Take user input for base dataset and output dataset name
    printf("Enter the base dataset name: ");
    scanf("%s", base_dataset);
    printf("Enter the output dataset name: ");
    scanf("%s", output_dataset);

    // Construct and run command for step 2
    char command_step2[MAX_COMMAND_LENGTH];
    snprintf(command_step2, MAX_COMMAND_LENGTH, "plink.exe --bfile %s --bmerge basic.bed basic.bim basic.fam --indiv-sort 0 --make-bed --allow-no-sex --out %s > " DEV_NULL " 2>&1", base_dataset, output_dataset);
    int status_step2 = system(command_step2);

    // Step 3: Retry with additional commands if step 2 fails
    if (status_step2 != 0) {
        // Construct and run command for step 3a
        char command_step3a[MAX_COMMAND_LENGTH];
        snprintf(command_step3a, MAX_COMMAND_LENGTH, "plink.exe --bfile basic --flip %s-merge.missnp --make-bed --allow-no-sex --out basic_tmp > " DEV_NULL " 2>&1", output_dataset);
        run_command(command_step3a);

        // Construct and run command for step 3b
        char command_step3b[MAX_COMMAND_LENGTH];
        snprintf(command_step3b, MAX_COMMAND_LENGTH, "plink.exe --bfile %s --bmerge basic_tmp.bed basic_tmp.bim basic_tmp.fam --indiv-sort 0 --make-bed --allow-no-sex --out %s > " DEV_NULL " 2>&1", base_dataset, output_dataset);
        int status_step3 = system(command_step3b);

        // Step 4: Retry with additional commands if step 3 fails
        if (status_step3 != 0) {
            // Construct and run command for step 4a
            char command_step4a[MAX_COMMAND_LENGTH];
            snprintf(command_step4a, MAX_COMMAND_LENGTH, "plink.exe --bfile basic_tmp --exclude %s-merge.missnp --make-bed --allow-no-sex --out basic > " DEV_NULL " 2>&1", output_dataset);
            run_command(command_step4a);

            // Retry Step 2
            run_command(command_step2);
        }
    }

    // Delete temporary files
    delete_files();

    return 0;
}