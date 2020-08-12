//The header files
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//define an 8-bit unsigned int, as specified by uint8_t (stdint.h)
typedef uint8_t BYTE;

//Main block
int main(int argc, char *argv[])
{
    //Check correct usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    //Open file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Can't open file\n");
        return 1;
    }

    //c is to keep count of new files, start is to check when the image file begins
    int c = 0;
    int start = 0;
    //buffer takes up 512 chunks of data, filename stores the name of the file
    BYTE *buffer = malloc(512 * sizeof(BYTE));
    char *filename = malloc(8 * sizeof(char));
    //create a file pointer for writing to new files
    FILE *f = NULL;
    
    //run loop until end of file
    while (fread(buffer, sizeof(BYTE), 512, file) == 512)
    {
        //If it is the start header for a new jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //make start to 1, so that new images start 
            start = 1;
            //Check if its the first image
            if (c == 0)
            {
                sprintf(filename, "%03i.jpg", c);
                f = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), 512, f);
                c++;
            }
            //Start this for subsequent images
            else
            {
                fclose(f);
                sprintf(filename, "%03i.jpg", c);
                f = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), 512, f);
                c++;
            }
        }
        //Do this for all consequent block until a new image header is found
        else if (start == 1)
        {
            //f = fopen(*filename, "w"); // DO NOT OPEN HERE, spent most time to debug this!!
            fwrite(buffer, sizeof(BYTE), 512, f);
        }
        else
        {
            //Do nothing
        }

    }
    //free space for buffer and filename created using malloc
    free(buffer);
    free(filename);
    //Close the last file
    fclose(f);
}
