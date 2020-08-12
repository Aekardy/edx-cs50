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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Create a copy for calculation (IMP) 
    //NOTE the height and width is incremented by 2 to adjust out of bounds pixels
    RGBTRIPLE copy[height + 2][width + 2];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i + 1][j + 1] = image[i][j];
        }
    }
    
    //Create the Gx and Gy matrices
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    
    //Loop over the entire 2d image array
    //Here it starts from 1 just to make sure that surrounding pixels of corners and edges are indexed correctly
    for (int i = 1; i <= height; i++)
    {
        for (int j = 1; j <= width; j++)
        {
            //These values store the respective values found from kernels
            int R = 0, R_x = 0, R_y = 0, G = 0, G_x = 0, G_y = 0, B = 0, B_x = 0, B_y = 0;
            //We go over all bounding pixels 
            for (int r = -1; r < 2; r++)
            {
                for (int c = -1; c < 2; c++)
                {
                    if (((r + i) <= 0 || (c + j) <= 0) || ((r + i) > (height) || (c + j) > (width)))
                    {
                        //If corner or edge pixel, set the outer pixels to 0, then go on to calculate the Gx and Gy
                        copy[r + i][j + c].rgbtRed = 0;
                        copy[r + i][j + c].rgbtGreen = 0;
                        copy[r + i][j + c].rgbtBlue = 0;
                        
                        R_x += copy[r + i][j + c].rgbtRed * Gx[r + 1][c + 1];
                        G_x += copy[r + i][j + c].rgbtGreen * Gx[r + 1][c + 1];
                        B_x += copy[r + i][j + c].rgbtBlue * Gx[r + 1][c + 1];
                        
                        R_y += copy[r + i][j + c].rgbtRed * Gy[r + 1][c + 1];
                        G_y += copy[r + i][j + c].rgbtGreen * Gy[r + 1][c + 1];
                        B_y += copy[r + i][j + c].rgbtBlue * Gy[r + 1][c + 1];
                    }
                    //This calculates the Gx and Gy for the rest pixels in a similar fashion
                    //IMP: USE COPY AS USING THE ORIGINAL CHANGES THE VALUES BEFORE ASSIGNING (spent a lot of time here)
                    else
                    {
                        R_x += copy[i + r][j + c].rgbtRed * Gx[r + 1][c + 1];;
                        G_x += copy[i + r][j + c].rgbtGreen * Gx[r + 1][c + 1];
                        B_x += copy[i + r][j + c].rgbtBlue * Gx[r + 1][c + 1];
                        
                        R_y += copy[r + i][j + c].rgbtRed * Gy[r + 1][c + 1];
                        G_y += copy[r + i][j + c].rgbtGreen * Gy[r + 1][c + 1];
                        B_y += copy[r + i][j + c].rgbtBlue * Gy[r + 1][c + 1];
                    }
                }
            }
            //Finally, calculate the final value (Sobel filter algorithm)
            R = round(sqrt((R_x * R_x) + (R_y * R_y)));
            G = round(sqrt((G_x * G_x) + (G_y * G_y)));
            B = round(sqrt((B_x * B_x) + (B_y * B_y)));
            
            //Store the value in original file, capping at a max value of 255
            image[i - 1][j - 1].rgbtRed = R > 255 ? 255 : R;
            image[i - 1][j - 1].rgbtGreen = G > 255 ? 255 : G;
            image[i - 1][j - 1].rgbtBlue = B > 255 ? 255 : B;
        }
    }
    return;
}
