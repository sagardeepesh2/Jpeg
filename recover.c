#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef uint8_t BYTE;


int main(int argc, char *argv[])
{
    //ensure proper useage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }


    //getting the recovery file image
    char *jfile = argv[1];


    //open input file
    FILE *inptr = fopen(jfile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", jfile);
        return 2;
    }

    //read jpg file

    BYTE buffer[512];
    int imageCount = 0;
    char filename[8];
    FILE *outptr = NULL;

    while (1)
    {
        // read a block of the memory card image
        size_t bytesRead = fread(buffer, 1, 512, inptr);

        // break out of the loop when we reach the end of the card image
        if (bytesRead == 0 && feof(inptr))
        {
            break;
        }

        // check if we found a JPEG
        int containsJpegHeader = buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;

        // if we found a yet another JPEG, we must close the previous file
        if (containsJpegHeader && outptr != NULL)
        {
            fclose(outptr);
            imageCount++;
        }

        // if we found a JPEG, we need to open the file for writing
        if (containsJpegHeader)
        {
            sprintf(filename, "%03i.jpg", imageCount);
            outptr = fopen(filename, "w");
        }

        // write anytime we have an open file
        if (outptr != NULL)
        {
            fwrite(buffer, 1, bytesRead, outptr);
        }
    }

    // close last jpeg file
    fclose(outptr);

    // close infile
    fclose(inptr);

    // success
    return 0;
}

