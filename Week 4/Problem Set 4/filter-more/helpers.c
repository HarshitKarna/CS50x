#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg = 0;
    for (int i=0; i < height; i++)
    {
        for (int j=0; j < width; j++)
        {
            avg = (int)round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{    
    for (int i=0; i < height; i++)
    {
        for (int j=0; j < width; j++)
        {
            int SepiaRed = (int)round(0.393 * image[i][j].rgbtRed
                                + 0.769 * image[i][j].rgbtGreen
                                + 0.189 * image[i][j].rgbtBlue);
            int SepiaGreen = (int)round(0.349 * image[i][j].rgbtRed
                                + 0.686 * image[i][j].rgbtGreen
                                + 0.168 * image[i][j].rgbtBlue);
            int SepiaBlue = (int)round(0.272 * image[i][j].rgbtRed
                                + 0.534 * image[i][j].rgbtGreen
                                + 0.131 * image[i][j].rgbtBlue);

            if (SepiaRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = SepiaRed;
            }

            if (SepiaGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen= SepiaGreen;
            }

            if (SepiaBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = SepiaBlue;
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];

            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp; 
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    // Make a copy so that changing one pixel doesn't affect others
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            copy[row][col] = image[row][col];
        }
    }

    // Go through every pixel
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int red = 0;
            int green = 0;
            int blue = 0;
            int count = 0;

            // Look at the pixel and its neighbors
            for (int r = row - 1; r <= row + 1; r++)
            {
                for (int c = col - 1; c <= col + 1; c++)
                {
                    // Skip pixels outside the image
                    if (r < 0 || r >= height || c < 0 || c >= width)
                        continue;

                    red += copy[r][c].rgbtRed;
                    green += copy[r][c].rgbtGreen;
                    blue += copy[r][c].rgbtBlue;

                    count++;
                }
            }

            // Calculate average
            image[row][col].rgbtRed = round((float) red / count);
            image[row][col].rgbtGreen = round((float) green / count);
            image[row][col].rgbtBlue = round((float) blue / count);
        }
    }

    return;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    // Copy the original image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            copy[row][col] = image[row][col];
        }
    }

    int Gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] =
    {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int gxRed = 0, gxGreen = 0, gxBlue = 0;
            int gyRed = 0, gyGreen = 0, gyBlue = 0;

            // Look at the 3x3 area around the current pixel
            for (int r = -1; r <= 1; r++)
            {
                for (int c = -1; c <= 1; c++)
                {
                    int nr = row + r;
                    int nc = col + c;

                    // Ignore pixels outside the image
                    if (nr < 0 || nr >= height || nc < 0 || nc >= width)
                        continue;

                    gxRed += copy[nr][nc].rgbtRed * Gx[r + 1][c + 1];
                    gxGreen += copy[nr][nc].rgbtGreen * Gx[r + 1][c + 1];
                    gxBlue += copy[nr][nc].rgbtBlue * Gx[r + 1][c + 1];

                    gyRed += copy[nr][nc].rgbtRed * Gy[r + 1][c + 1];
                    gyGreen += copy[nr][nc].rgbtGreen * Gy[r + 1][c + 1];
                    gyBlue += copy[nr][nc].rgbtBlue * Gy[r + 1][c + 1];
                }
            }

            // Calculate final RGB values
            int red = round(sqrt(gxRed * gxRed + gyRed * gyRed));
            int green = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            int blue = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));

            // Maximum RGB value is 255
            if (red > 255)
                red = 255;
            if (green > 255)
                green = 255;
            if (blue > 255)
                blue = 255;

            image[row][col].rgbtRed = red;
            image[row][col].rgbtGreen = green;
            image[row][col].rgbtBlue = blue;
        }
    }

    return;
}

