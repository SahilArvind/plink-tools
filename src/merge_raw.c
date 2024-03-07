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
    remove("add1.bed");
    remove("add1.bim");
    remove("add1.fam");
    remove("add1.hh");
    remove("add1.log");
    remove("add1.nosex");
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

int main(int argc, char *argv[]) {
    if (argc != 7) {
        fprintf(stderr, "Usage: %s -i raw_file -b base -o output\n", argv[0]);
        return EXIT_FAILURE;
    }

    char raw_file[MAX_FILENAME_LENGTH];
    char base_dataset[MAX_FILENAME_LENGTH];
    char output_dataset[MAX_FILENAME_LENGTH];

    // Parse command line arguments
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-i") == 0) {
            strncpy(raw_file, argv[i + 1], MAX_FILENAME_LENGTH - 1);
        } else if (strcmp(argv[i], "-b") == 0) {
            strncpy(base_dataset, argv[i + 1], MAX_FILENAME_LENGTH - 1);
        } else if (strcmp(argv[i], "-o") == 0) {
            strncpy(output_dataset, argv[i + 1], MAX_FILENAME_LENGTH - 1);
        } else {
            fprintf(stderr, "Invalid option: %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    char command_a[MAX_COMMAND_LENGTH];
    snprintf(command_a, MAX_COMMAND_LENGTH, "plink.exe --allow-no-sex --23file %s.txt --make-bed --out add > " DEV_NULL " 2>&1", raw_file);
    run_command(command_a);

    run_command("plink.exe --bfile add --extract plink.snplist --make-bed --allow-no-sex --out basic > " DEV_NULL " 2>&1");

    char command_step2[MAX_COMMAND_LENGTH];
    snprintf(command_step2, MAX_COMMAND_LENGTH, "plink.exe --bfile %s --bmerge basic.bed basic.bim basic.fam --indiv-sort 0 --make-bed --allow-no-sex --out %s > " DEV_NULL " 2>&1", base_dataset, output_dataset);
    int status_step2 = system(command_step2);

    if (status_step2 != 0) {
        char command_step3a[MAX_COMMAND_LENGTH];
        snprintf(command_step3a, MAX_COMMAND_LENGTH, "plink.exe --bfile basic --flip %s-merge.missnp --make-bed --allow-no-sex --out basic_tmp > " DEV_NULL " 2>&1", output_dataset);
        int status_step3a = system(command_step3a);

        if (status_step3a != 0) {
            run_command("plink2.exe --bfile add --rm-dup force-first  --make-bed --out add1 > " DEV_NULL " 2>&1");
            run_command("plink.exe --bfile add1 --extract plink.snplist --make-bed --allow-no-sex --out basic > " DEV_NULL " 2>&1");

            int status_step2 = system(command_step2);

            if (status_step2 != 0) {
                char command_step3a[MAX_COMMAND_LENGTH];
                snprintf(command_step3a, MAX_COMMAND_LENGTH, "plink.exe --bfile basic --flip %s-merge.missnp --make-bed --allow-no-sex --out basic_tmp > " DEV_NULL " 2>&1", output_dataset);
                int status_step3a = system(command_step3a);

                char command_step3b[MAX_COMMAND_LENGTH];
                snprintf(command_step3b, MAX_COMMAND_LENGTH, "plink.exe --bfile %s --bmerge basic_tmp.bed basic_tmp.bim basic_tmp.fam --indiv-sort 0 --make-bed --allow-no-sex --out %s > " DEV_NULL " 2>&1", base_dataset, output_dataset);
                int status_step3b = system(command_step3b);

                if (status_step3b != 0) {
                    char command_step4a[MAX_COMMAND_LENGTH];
                    snprintf(command_step4a, MAX_COMMAND_LENGTH, "plink.exe --bfile basic_tmp --exclude %s-merge.missnp --make-bed --allow-no-sex --out basic > " DEV_NULL " 2>&1", output_dataset);
                    run_command(command_step4a);
                    run_command(command_step2);
                }
            }

            char command_step3b[MAX_COMMAND_LENGTH];
            snprintf(command_step3b, MAX_COMMAND_LENGTH, "plink.exe --bfile %s --bmerge basic_tmp.bed basic_tmp.bim basic_tmp.fam --indiv-sort 0 --make-bed --allow-no-sex --out %s > " DEV_NULL " 2>&1", base_dataset, output_dataset);
            int status_step3b = system(command_step3b);

            if (status_step3b != 0) {
                char command_step4a[MAX_COMMAND_LENGTH];
                snprintf(command_step4a, MAX_COMMAND_LENGTH, "plink.exe --bfile basic_tmp --exclude %s-merge.missnp --make-bed --allow-no-sex --out basic > " DEV_NULL " 2>&1", output_dataset);
                run_command(command_step4a);

                run_command(command_step2);
            }
        }
    }
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

    delete_files();

    return EXIT_SUCCESS;
}