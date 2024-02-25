# plink-tools
A package for automating mundane tasks using [PLINK 1.9](https://www.cog-genomics.org/plink/). <br />

### Installation
Unzip the repositoy and place all the files inside the ./src and ./data directories in the PLINK directory. <br />

### Usage
**merge.c**  <br />

This code is a C program that performs data manipulation using the PLINK software. Here's a concise explanation:  <br />

 - It defines maximum lengths for filenames and commands.  <br />
 - run_command() function executes a given command and handles errors.  <br />
 - In the main() function:  <br />
 * User inputs the name of a raw file.  <br />
 * It constructs and runs a command (command_a) using plink.exe to process the raw file.  <br />
 * It constructs and runs another command (command_b) using plink.exe.  <br />
 * User inputs the names of a base dataset and an output dataset.  <br />
 * It constructs and runs a command for merging datasets (command_step2) using plink.exe.  <br />
 * If merging fails, it retries with additional commands (command_step3a, command_step3b), then again (command_step4a, command_step2).  <br />
 - Finally, the program returns 0.  <br />

Overall, the program automates several steps of data processing with PLINK, allowing for user input and handling failures with retry mechanisms. <br />

*Execute the C program by running the following in the terminal:* <br />
``` r
./merge
```

**extract_samples.c**  <br />

This C program performs the following tasks:   <br />

 - Takes user input for an input file name and a list of words to find.   <br />
 - Reads lines from the input file.   <br />
 - Searches for each word in the list within the lines (case-insensitive).   <br />
 - Writes lines containing any of the specified words to an output file named "keep.txt".   <br />
 - Removes the file extension from the input file name.   <br />
 - Executes a PLINK command using the input file and the generated "keep.txt" file.   <br />
 - Outputs a message indicating the completion of the operation.   <br />

Overall, it filters lines containing specified words from the input file, writes them to an output file, and then uses this filtered data as input for a PLINK command.   <br />

*Execute the C program by running the following in the terminal:* <br />
``` r
./extract_samples
```