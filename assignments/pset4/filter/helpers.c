#include "helpers.h"
#include <math.h>

int min(int a, int b);
int max(int a, int b);

int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // create new image
    RGBTRIPLE img[height][width];

    // loop over rows
    for (int i = 0; i < height; i++)
    {
        // loop over columns
        for (int j = 0; j < width; j++)
        {
            // store colors here
            int xRed = 0, xGreen = 0, xBlue = 0;
            int yRed = 0, yGreen = 0, yBlue = 0;

            // loop over 3x3 kernel
            for (int p = 0; p < 3; p++)
            {
                for (int q = 0; q < 3; q++)
                {
                    // get index
                    int r = i + (p - 1);
                    int c = j + (q - 1);
                    // if valid index
                    if (r > -1 && r < height && c > -1 && c < width)
                    {
                        // apply Gx
                        xRed += Gx[p][q] * image[r][c].rgbtRed;
                        xGreen += Gx[p][q] * image[r][c].rgbtGreen;
                        xBlue += Gx[p][q] * image[r][c].rgbtBlue;
                        // apply Gy
                        yRed += Gy[p][q] * image[r][c].rgbtRed;
                        yGreen += Gy[p][q] * image[r][c].rgbtGreen;
                        yBlue += Gy[p][q] * image[r][c].rgbtBlue;
                    }
                }
            }

            // calculate colors
            int red = round(sqrt(xRed * xRed + yRed * yRed));
            int green = round(sqrt(xGreen * xGreen + yGreen * yGreen));
            int blue = round(sqrt(xBlue * xBlue + yBlue * yBlue));
            img[i][j].rgbtRed = min(255, red);
            img[i][j].rgbtGreen = min(255, green);
            img[i][j].rgbtBlue = min(255, blue);
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