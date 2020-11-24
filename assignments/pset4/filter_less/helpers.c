#include "helpers.h"
#include <math.h>

int min(int a, int b);
int max(int a, int b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop over rows
    for (int i = 0; i < height; i++)
    {
        // loop over columns
        for (int j = 0; j < width; j++)
        {
            // take average and round
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen
                             + image[i][j].rgbtRed) / 3.0);
            // assign average to all colors
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
    // loop over rows
    for (int i = 0; i < height; i++)
    {
        // loop over columns
        for (int j = 0; j < width; j++)
        {
            // assign a pointer to pixel
            RGBTRIPLE *p = &image[i][j];
            // get new colors as per formula
            int sepiaRed = round(0.393 * p->rgbtRed +
                                 0.769 * p->rgbtGreen +
                                 0.189 * p->rgbtBlue);
            int sepiaGreen = round(0.349 * p->rgbtRed +
                                   0.686 * p->rgbtGreen +
                                   0.168 * p->rgbtBlue);
            int sepiaBlue = round(0.272 * p->rgbtRed +
                                  0.534 * p->rgbtGreen +
                                  0.131 * p->rgbtBlue);
            // assign the minimum of 255 and new color
            p->rgbtRed = min(255, sepiaRed);
            p->rgbtGreen = min(255, sepiaGreen);
            p->rgbtBlue = min(255, sepiaBlue);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // loop over rows
    for (int i = 0; i < height; i++)
    {
        // get half of column width
        int half = width / 2;
        // loop over half the columns
        for (int j = 0; j < half; j++)
        {
            // get index of swapping partner
            int k = width - (j + 1);
            // swap cells (j) and (k)
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][k];
            image[i][k] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create new image
    RGBTRIPLE img[height][width];

    // loop over rows
    for (int i = 0; i < height; i++)
    {
        // loop over columns
        for (int j = 0; j < width; j++)
        {
            // calculate colors for this pixel
            int red = 0, blue = 0, green = 0, count = 0;
            // loop over rows above and below if exist
            int rMin = max(0, i - 1);
            int rMax = min(i + 2, height);
            int cMin = max(0, j - 1);
            int cMax = min(j + 2, width);
            for (int r = rMin; r < rMax; r++)
            {
                // loop over columns left and right
                for (int c = cMin; c < cMax; c++)
                {
                    red += image[r][c].rgbtRed;
                    green += image[r][c].rgbtGreen;
                    blue += image[r][c].rgbtBlue;
                    count++;
                }
            }
            // take average and assign
            img[i][j].rgbtRed = round((red * 1.0) / count);
            img[i][j].rgbtGreen = round((green * 1.0) / count);
            img[i][j].rgbtBlue = round((blue * 1.0) / count);
        }
    }

    // replace image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // copy all colors
            image[i][j].rgbtRed = img[i][j].rgbtRed;
            image[i][j].rgbtGreen = img[i][j].rgbtGreen;
            image[i][j].rgbtBlue = img[i][j].rgbtBlue;
        }
    }

    return;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}