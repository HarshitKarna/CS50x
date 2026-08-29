// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "wb");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]); // atof converts string into double precision

    // Copy header from input file to output file
    uint8_t header[44];

    if (fread(header, sizeof(uint8_t), 44, input) != 44) // & not needed because header is an arr
    {
        printf("Couldn't read the input.");
        return 1;
    }
    if (fwrite(header, sizeof(uint8_t), 44, output) != 44)
    {
        printf("Couldn't write header to the output.");
        return 1;
    }

    // Read samples from input file and write updated data to output file
    int16_t sample;

    while (fread(&sample, sizeof(sample), 1, input) == 1)  // fread returns the number of items read. so if its 0, means that fread couldnt read anything, so we end loop
    {
        sample *= factor;
        fwrite(&sample, sizeof(sample), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);

    return 0;
}
