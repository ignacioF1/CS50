#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // Where uint8_t is defined

// Each JPEG is stored back to back in the memory card

typedef uint8_t BYTE; // Define a new type called BYTE, which stores 8 bits
int i = 0; // JPEG count
char filename[8]; // To store the file name: 000.jpg + Null terminator = 8 chars

int main(int argc, char *argv[])
{
// The program works with: ./recover card.raw

    // Check for invalid usage
    if (argc < 2 || argc > 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open the memory card
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }

    unsigned char *buffer = malloc(sizeof(char) * 512); // Allocate 512 Bytes for reading the card

    sprintf(filename, "%03i.jpg", 0); // Print the first file name in char* "filename"
    FILE *img = fopen(filename, "w"); // Open the first JPEG file for writting

    while (fread(buffer, 512, 1, file) > 0) // While the read bytes are more than 0 (The function returns the number of read bytes)
    {

        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
            // If it is the beginning of a JPEG (JPEG starts with 0xff 0xd8 0xff 0xeX), start a new file and close if another has been opened
        {

            if (i != 0) // Once the first JPEG appeared
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", i); // Print the new file name in char* "filename"
                img = fopen(filename, "w"); // Open new JPEG file for writting       
            }
            i ++; // increment the file count
        }
        
        if (i != 0) // Once the first JPEG appeared
        {
            fwrite(buffer, 512, 1, img);  // if it is not a new file, continue writing it
        }
    } // End while, end of file
    
    fclose(img); // Close the last image
    
    free(buffer);

}