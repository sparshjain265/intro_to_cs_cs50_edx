#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // accept exactly 1 argument
    if (argc != 2)
    {
        printf("Usage: recover raw_file.\n");
        return 1;
    }

    // remember filename
    char *fileName = argv[1];

    // open file
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", fileName);
        return 1;
    }

    // initialize image count
    int imgCount = 0;
    // string to store name of new image
    char imgName[10];
    // file to store image
    FILE *imgFile = NULL;

    // define a block as an array of 512 bytes
    BYTE block[512];

    // read a block
    int readCount = fread(block, sizeof(BYTE), 512, file);
    // while successful
    while (readCount == 512)
    {
        // check for JPEG header
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff &&
            (block[3] & 0xf0) == 0xe0)
        {
            // name of new image
            sprintf(imgName, "%03i.jpg", imgCount);
            // increment image count
            imgCount++;

            // close previous file if open
            if (imgFile)
            {
                fclose(imgFile);
            }

            // open new file
            imgFile = fopen(imgName, "w");
        }

        // if an image file is open
        if (imgFile != NULL)
        {
            // write the block to file
            fwrite(block, sizeof(BYTE), 512, imgFile);
        }

        // read again for next iteration
        readCount = fread(block, sizeof(BYTE), 512, file);
    }

    // close files
    if (imgFile)
    {
        fclose(imgFile);
    }

    if (file)
    {
        fclose(file);
    }

    // success
    return 0;
}
