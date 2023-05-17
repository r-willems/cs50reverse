/*This is a working solution to the Problem Set Reverse from Harvard's CS50x course:
https://cs50.harvard.edu/x/2023/psets/4/
This is for showcase and eductional purposes only. 
Students taking CS50, I stronlgy encourage you to attempt your own code and use this as inspiration 
only. Keep in mind the Academic Honesty Policy!*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("Improper usage.\n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    char *infile = argv[1];//Name of the input file
    char *outfile = argv[2];//Name of the output file

    //Open input file in "reading"-mode
    FILE *input = fopen(infile, "r");
    if (input == NULL)
    {
        printf("Incorrect input.\n");
        fclose(input);
        return 1;
    }

    // Read header
    // TODO #3
    //Read file-header from input
    WAVHEADER fheader;
    fread(&fheader, sizeof(WAVHEADER), 1, input);

    // Use check_format to ensure WAV format
    // TODO #4
    if (check_format(fheader) != 0)
    {
        fclose(input);
        return 1;
    }

    // Open output file for writing
    // TODO #5
    FILE *output = fopen(outfile, "w");
    //Ensure a valid output file
    if (output == NULL)
    {
        printf("Output file unsuccesful");
        fclose(output);
        fclose(input);
        return 1;
    }

    // Write header to output-file
    // TODO #6
    fwrite(&fheader, sizeof(WAVHEADER), 1, output);

    // Use get_block_size to calculate size of block
    // TODO #7
    int size = get_block_size(fheader);

    // Write reversed audio to file
    // TODO #8
        /*
    -create a buffer array of 8 bits * <size of block>; 
    initialise loop at <size of block> before EOF; 
    continue until <pointer current location>-<size of block> is at start of audio data chunk;
    place fseek() pointer back twice <size of block>, once to the previous location, once more to go 
    to the new location
        */
    BYTE buffer[size];
    for (fseek(input, 0 - size, SEEK_END); ftell(input) > sizeof(fheader)-size; fseek(input, 0 - (size * 2), SEEK_CUR))
    { 
        fread(&buffer, size, 1, input);
        fwrite(&buffer, size, 1, output);
    }
  

    fclose(input);
    fclose(output);
    return 0;
}

int check_format(WAVHEADER header)
{
    // TODO #4
    //Array of characters
    int wav[] = {0x57, 0x41, 0x56, 0x45};
    int i;
    for (i = 0; i < 4; i++)
    {
        //check if chars match those in file-format-descriptor
        if (header.format[i] != wav[i])
        {
            printf("Not a valid WAV file!\n");
            return 1;
        }
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    //block size in bytes = 4;
    int n;
    n = header.numChannels * (header.bitsPerSample / 8);

    return n;
}
