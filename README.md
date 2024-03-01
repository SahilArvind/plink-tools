# plink-tools
A package for automating mundane tasks using [PLINK 1.9](https://www.cog-genomics.org/plink/). <br />

### Installation
Unzip the repository and place all the files inside the ./src and ./data directories in the PLINK directory. <br />

### Usage
**merge_raw.c**  <br />

This C program utilizes the PLINK tool for converting the raw file to plink format and merging it with another dataset. It first prompts the user to input a raw file name, executes initial PLINK commands based on this file, and then prompts for a base dataset name and an output dataset name. It merges datasets using another PLINK command, incorporating error-handling logic to retry and adjust commands if necessary. The program aims to streamline genetic data manipulation tasks while managing potential errors during the dataset merging process. <br />

Overall, the program automates several steps of data processing with PLINK, allowing for user input and handling failures with retry mechanisms. <br />

*Execute the C program by running the following in the terminal:* <br />
``` r
./merge_raw
```

**extract_samples.c**  <br />

This C program prompts the user for an input file name and a list of words to find. It reads the input file, searching for any occurrence of the provided words in each line (case-insensitive search). Lines containing these words are written to an output file named "keep.txt". It then removes the file extension from the input file name and constructs a command to run PLINK, a tool for genetic analysis, using the input file and "keep.txt". Finally, it executes this command using the system() function. <br />

Overall, it filters lines containing specified words from the input file, writes them to an output file, and then uses this filtered data as input for a PLINK command.   <br />

*Execute the C program by running the following in the terminal:* <br />
``` r
./extract_samples
```

**convert_2_23andme.c**  <br />

This C program prompts the user to specify the type of genetic data file they want to convert to a 23andme format. It supports file types such as AncestryDNA, FTDNA, MyHeritage, Mapmygenome and LivingDNA. After converting the user's input to lowercase for uniform comparison, it calls the corresponding conversion function based on the specified file type. Each conversion function processes the input file, skipping certain lines based on predefined conditions, and writes the processed data to an output file. For FTDNA and Mapmygenome file types, the program executes a Python script named "sort.py" after conversion. <br />

*Execute the C program by running the following in the terminal:* <br />
``` r
./convert_2_23andme
```