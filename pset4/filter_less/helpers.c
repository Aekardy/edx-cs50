#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //Loop over the whole 2d image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Take the average of RGB value, set the average value to each pixel
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //Create a copy for calculation (IMP)
    //The copy is required to make sure not to change the original while calculations
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    
    //Loop over the whole 2d image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Calculate sepiaRed and cap the value to 255
            int sepiaRed = round(.393 * copy[i][j].rgbtRed + .769 * copy[i][j].rgbtGreen + .189 * copy[i][j].rgbtBlue);
            image[i][j].rgbtRed = sepiaRed > 255 ? 255 : sepiaRed;
            
            //Same for sepiaGreen and sepiaBlue
            int sepiaGreen = round(.349 * copy[i][j].rgbtRed + .686 * copy[i][j].rgbtGreen + .168 * copy[i][j].rgbtBlue);
            image[i][j].rgbtGreen = sepiaGreen > 255 ? 255 : sepiaGreen;
            
            int sepiaBlue = round(.272 * copy[i][j].rgbtRed + .534 * copy[i][j].rgbtGreen + .131 * copy[i][j].rgbtBlue);
            image[i][j].rgbtBlue = sepiaBlue > 255 ? 255 : sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Loop over the 2d image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            //Swap jth pixel in ith row, the row order remains same, the columns flip
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Create a copy for calculation (IMP)
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    
    //Loop over the entire 2d image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //These values store the rgb values for one pixel, cnt is for finding the average
            int R = 0, G = 0, B = 0, cnt = 0;
            //We go over all bounding pixels 
            for (int r = -1; r < 2; r++)
            {
                for (int c = -1; c < 2; c++)
                {
                    if ((r + i) < 0 || (c + j) < 0 || (r + i) > (height - 1) || (c + j) > (width - 1))
                    {
                        //Do Nothing, as out of bounds
                    }
                    //This calculates the rgb values for all bounding pixels along with itself
                    //IMP: USE COPY AS USING THE ORIGINAL CHANGES THE VALUES BEFORE ASSIGNING (spent a lot of time here)
                    else
                    {
                        R += copy[i + r][j + c].rgbtRed;
                        G += copy[i + r][j + c].rgbtGreen;
                        B += copy[i + r][j + c].rgbtBlue;
                        cnt++;
                    }
                }
            }
            //Finally, assign the rgb average to the individual pixel
            image[i][j].rgbtRed = round(R / (float) cnt);
            image[i][j].rgbtGreen = round(G / (float) cnt);
            image[i][j].rgbtBlue = round(B / (float) cnt);
        }
    }
    return;
}
