#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float prom;
    for (int i = 0 ; i < height ; i ++) // For every row
    {
        for (int j = 0 ; j < width ; j ++)  // For every pixel
        {
            prom = roundf((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0f);
            image[i][j].rgbtRed = prom;
            image[i][j].rgbtGreen = prom;
            image[i][j].rgbtBlue = prom;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0 ; i < height ; i ++) // For every row
    {
        for (int j = 0 ; j < width ; j ++)  // For every pixel
        {
            // Apply sepia formula
            float sepiaRed = roundf(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            float sepiaGreen = roundf(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            float sepiaBlue = roundf(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);
            // For each amount of color, set to 255 if it is greater than 255
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int mid = width / 2;
    int temp;
    for (int i = 0 ; i < height ; i ++) // For every row
    {
        for (int j = 0 ; j < mid ; j ++)  // For every pixel
        {
            temp = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][(width - 1) - j].rgbtRed;
            image[i][(width - 1) - j].rgbtRed = temp;

            temp = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][(width - 1) - j].rgbtGreen;
            image[i][(width - 1) - j].rgbtGreen = temp;

            temp = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][(width - 1) - j].rgbtBlue;
            image[i][(width - 1) - j].rgbtBlue = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imagetemp[height][width];

    // I'll divide the problem into solving these parts separately
    // The corners, the border lines and the center part
    // +   + + +   +
    //
    // +   + + +   +
    // +   + + +   +
    // +   + + +   +
    //
    // +   + + +   +

// Center part
    for (int i = 1 ; i < (height - 1) ; i ++) // For every row, except for the borders
    {
        for (int j = 1 ; j < (width - 1) ; j ++)  // For every pixel, exept from the borders
        {
            float promR = roundf((image[i - 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed +
                                  image[i - 1][j].rgbtRed + image[i][j].rgbtRed + image[i + 1][j].rgbtRed +
                                  image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed) / 9.0f);

            float promG = roundf((image[i - 1][j - 1].rgbtGreen + image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen +
                                  image[i - 1][j].rgbtGreen + image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen +
                                  image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 9.0f);

            float promB = roundf((image[i - 1][j - 1].rgbtBlue + image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue +
                                  image[i - 1][j].rgbtBlue + image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue +
                                  image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 9.0f);

            imagetemp[i][j].rgbtRed = promR;
            imagetemp[i][j].rgbtGreen = promG;
            imagetemp[i][j].rgbtBlue = promB;
        }
    }

// Upper horizontal border line
    int i = 0;
    for (int j = 1 ; j < (width - 1) ; j ++)  // For every pixel, exept from the corners
    {
        float promR = roundf((image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed +
                              image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 6.0f);

        float promG = roundf((image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                              image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 6.0f);

        float promB = roundf((image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                              image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 6.0f);

        imagetemp[i][j].rgbtRed = promR;
        imagetemp[i][j].rgbtGreen = promG;
        imagetemp[i][j].rgbtBlue = promB;
    }
// Lower horizontal border line
    i = height - 1;
    for (int j = 1 ; j < (width - 1) ; j ++)  // For every pixel, exept from the corners
    {
        float promR = roundf((image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed +
                              image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed) / 6.0f);

        float promG = roundf((image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                              image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen) / 6.0f);

        float promB = roundf((image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                              image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue) / 6.0f);

        imagetemp[i][j].rgbtRed = promR;
        imagetemp[i][j].rgbtGreen = promG;
        imagetemp[i][j].rgbtBlue = promB;
    }

// Left vertical border line
    int j = 0;
    for (i = 1 ; i < (height - 1) ; i ++)   // For every pixel, exept from the corners
    {
        float promR = roundf((image[i - 1][j].rgbtRed + image[i][j].rgbtRed + image[i + 1][j].rgbtRed +
                              image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed) / 6.0f);

        float promG = roundf((image[i - 1][j].rgbtGreen + image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen +
                              image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 6.0f);

        float promB = roundf((image[i - 1][j].rgbtBlue + image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue +
                              image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 6.0f);

        imagetemp[i][j].rgbtRed = promR;
        imagetemp[i][j].rgbtGreen = promG;
        imagetemp[i][j].rgbtBlue = promB;
    }

// Right vertical border line
    j = width - 1;
    for (i = 1 ; i < (height - 1) ; i ++)  // For every pixel, exept from the corners
    {
        float promR = roundf((image[i - 1][j].rgbtRed + image[i][j].rgbtRed + image[i + 1][j].rgbtRed +
                              image[i - 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed) / 6.0f);

        float promG = roundf((image[i - 1][j].rgbtGreen + image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen +
                              image[i - 1][j - 1].rgbtGreen + image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen) / 6.0f);

        float promB = roundf((image[i - 1][j].rgbtBlue + image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue +
                              image[i - 1][j - 1].rgbtBlue + image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue) / 6.0f);

        imagetemp[i][j].rgbtRed = promR;
        imagetemp[i][j].rgbtGreen = promG;
        imagetemp[i][j].rgbtBlue = promB;
    }

// Top left corner
    i = 0;
    j = 0;

    float promR = roundf((image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i][j + 1].rgbtRed +
                          image[i + 1][j + 1].rgbtRed) / 4.0f);

    float promG = roundf((image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                          image[i + 1][j + 1].rgbtGreen) / 4.0f);

    float promB = roundf((image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                          image[i + 1][j + 1].rgbtBlue) / 4.0f);

    imagetemp[i][j].rgbtRed = promR;
    imagetemp[i][j].rgbtGreen = promG;
    imagetemp[i][j].rgbtBlue = promB;

// Bottom left corner
    i = height - 1;
    j = 0;

    promR = roundf((image[i][j].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +
                    image[i][j + 1].rgbtRed) / 4.0f);

    promG = roundf((image[i][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +
                    image[i][j + 1].rgbtGreen) / 4.0f);

    promB = roundf((image[i][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +
                    image[i][j + 1].rgbtBlue) / 4.0f);

    imagetemp[i][j].rgbtRed = promR;
    imagetemp[i][j].rgbtGreen = promG;
    imagetemp[i][j].rgbtBlue = promB;

// Top right corner
    i = 0;
    j = width - 1;

    promR = roundf((image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i][j - 1].rgbtRed +
                    image[i + 1][j - 1].rgbtRed) / 4.0f);

    promG = roundf((image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j - 1].rgbtGreen +
                    image[i + 1][j - 1].rgbtGreen) / 4.0f);

    promB = roundf((image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j - 1].rgbtBlue +
                    image[i + 1][j - 1].rgbtBlue) / 4.0f);

    imagetemp[i][j].rgbtRed = promR;
    imagetemp[i][j].rgbtGreen = promG;
    imagetemp[i][j].rgbtBlue = promB;

    // Bottom right corner
    i = height - 1;
    j = width - 1;

    promR = roundf((image[i][j].rgbtRed + image[i - 1][j].rgbtRed + image[i][j - 1].rgbtRed +
                    image[i - 1][j - 1].rgbtRed) / 4.0f);

    promG = roundf((image[i][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j - 1].rgbtGreen +
                    image[i - 1][j - 1].rgbtGreen) / 4.0f);

    promB = roundf((image[i][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j - 1].rgbtBlue +
                    image[i - 1][j - 1].rgbtBlue) / 4.0f);

    imagetemp[i][j].rgbtRed = promR;
    imagetemp[i][j].rgbtGreen = promG;
    imagetemp[i][j].rgbtBlue = promB;

    // Transfer the temporal image to the image
    for (i = 0 ; i < height ; i ++) // For every row
    {
        for (j = 0 ; j < width ; j ++)  // For every pixel
        {
            image[i][j].rgbtRed = imagetemp[i][j].rgbtRed;
            image[i][j].rgbtGreen = imagetemp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = imagetemp[i][j].rgbtBlue;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imagetemp[height][width];
    int GxR;
    int GxG;
    int GxB;
    int GyR;
    int GyG;
    int GyB;
    float GxyR;
    float GxyG;
    float GxyB;
    int i;
    int j;

    // I'll divide the problem into solving these parts separately
    // The corners, the border lines and the center part
    // +   + + +   +
    //
    // +   + + +   +
    // +   + + +   +
    // +   + + +   +
    //
    // +   + + +   +

// Center part
    for (i = 1 ; i < (height - 1) ; i ++) // For every row, except for the borders
    {
        for (j = 1 ; j < (width - 1) ; j ++)  // For every pixel, exept from the borders
        {
            GxR = ((-1 * (image[i - 1][j - 1].rgbtRed)) - (2 * (image[i][j - 1].rgbtRed)) - (image[i + 1][j - 1].rgbtRed) +
                   image[i - 1][j + 1].rgbtRed + (2 * (image[i][j + 1].rgbtRed)) + image[i + 1][j + 1].rgbtRed);

            GxG = ((-1 * (image[i - 1][j - 1].rgbtGreen)) - (2 * (image[i][j - 1].rgbtGreen)) - (image[i + 1][j - 1].rgbtGreen) +
                   image[i - 1][j + 1].rgbtGreen + (2 * (image[i][j + 1].rgbtGreen)) + image[i + 1][j + 1].rgbtGreen);

            GxB = ((-1 * (image[i - 1][j - 1].rgbtBlue)) - (2 * (image[i][j - 1].rgbtBlue)) - (image[i + 1][j - 1].rgbtBlue) +
                   image[i - 1][j + 1].rgbtBlue + (2 * (image[i][j + 1].rgbtBlue)) + image[i + 1][j + 1].rgbtBlue);

            GyR = ((-1 * (image[i - 1][j - 1].rgbtRed)) - (2 * (image[i - 1][j].rgbtRed)) - (image[i - 1][j + 1].rgbtRed) +
                   image[i + 1][j - 1].rgbtRed + (2 * (image[i + 1][j].rgbtRed)) + image[i + 1][j + 1].rgbtRed);

            GyG = ((-1 * (image[i - 1][j - 1].rgbtGreen)) - (2 * (image[i - 1][j].rgbtGreen)) - (image[i - 1][j + 1].rgbtGreen) +
                   image[i + 1][j - 1].rgbtGreen + (2 * (image[i + 1][j].rgbtGreen)) + image[i + 1][j + 1].rgbtGreen);

            GyB = ((-1 * (image[i - 1][j - 1].rgbtBlue)) - (2 * (image[i - 1][j].rgbtBlue)) - (image[i - 1][j + 1].rgbtBlue) +
                   image[i + 1][j - 1].rgbtBlue + (2 * (image[i + 1][j].rgbtBlue)) + image[i + 1][j + 1].rgbtBlue);

            GxyR = roundf(sqrt(pow(GxR, 2) + pow(GyR, 2)));
            GxyG = roundf(sqrt(pow(GxG, 2) + pow(GyG, 2)));
            GxyB = roundf(sqrt(pow(GxB, 2) + pow(GyB, 2)));

            if (GxyR > 255)
            {
                GxyR = 255;
            }
            if (GxyG > 255)
            {
                GxyG = 255;
            }
            if (GxyB > 255)
            {
                GxyB = 255;
            }

            imagetemp[i][j].rgbtRed = GxyR;
            imagetemp[i][j].rgbtGreen = GxyG;
            imagetemp[i][j].rgbtBlue = GxyB;

        }
    }

// Upper horizontal border line
    i = 0;

    for (j = 1 ; j < (width - 1) ; j ++)  // For every pixel, exept from the corners
    {
        GxR = (- 2 * (image[i][j - 1].rgbtRed) - 1 * (image[i + 1][j - 1].rgbtRed) + 2 * (image[i][j + 1].rgbtRed) +
               image[i + 1][j + 1].rgbtRed);

        GxG = (- 2 * (image[i][j - 1].rgbtGreen) - 1 * (image[i + 1][j - 1].rgbtGreen) + 2 * (image[i][j + 1].rgbtGreen) +
               image[i + 1][j + 1].rgbtGreen);

        GxB = (- 2 * (image[i][j - 1].rgbtBlue) - 1 * (image[i + 1][j - 1].rgbtBlue) + 2 * (image[i][j + 1].rgbtBlue) +
               image[i + 1][j + 1].rgbtBlue);

        GyR = (image[i + 1][j - 1].rgbtRed + 2 * (image[i + 1][j].rgbtRed) + image[i + 1][j + 1].rgbtRed);

        GyG = (image[i + 1][j - 1].rgbtGreen + 2 * (image[i + 1][j].rgbtGreen) + image[i + 1][j + 1].rgbtGreen);

        GyB = (image[i + 1][j - 1].rgbtBlue + 2 * (image[i + 1][j].rgbtBlue) + image[i + 1][j + 1].rgbtBlue);

        GxyR = roundf(sqrt(pow(GxR, 2) + pow(GyR, 2)));
        GxyG = roundf(sqrt(pow(GxG, 2) + pow(GyG, 2)));
        GxyB = roundf(sqrt(pow(GxB, 2) + pow(GyB, 2)));

        if (GxyR > 255)
        {
            GxyR = 255;
        }
        if (GxyG > 255)
        {
            GxyG = 255;
        }
        if (GxyB > 255)
        {
            GxyB = 255;
        }

        imagetemp[i][j].rgbtRed = GxyR;
        imagetemp[i][j].rgbtGreen = GxyG;
        imagetemp[i][j].rgbtBlue = GxyB;
    }

// Lower horizontal border line
    i = height - 1;
    for (j = 1 ; j < (width - 1) ; j ++)  // For every pixel, exept from the corners
    {
        GxR = (- 2 * (image[i][j - 1].rgbtRed) - 1 * (image[i - 1][j - 1].rgbtRed) + 2 * (image[i][j + 1].rgbtRed) +
               image[i - 1][j + 1].rgbtRed);

        GxG = (- 2 * (image[i][j - 1].rgbtGreen) - 1 * (image[i - 1][j - 1].rgbtGreen) + 2 * (image[i][j + 1].rgbtGreen) +
               image[i - 1][j + 1].rgbtGreen);

        GxB = (- 2 * (image[i][j - 1].rgbtBlue) - 1 * (image[i - 1][j - 1].rgbtBlue) + 2 * (image[i][j + 1].rgbtBlue) +
               image[i - 1][j + 1].rgbtBlue);

        GyR = (- 1 * image[i - 1][j - 1].rgbtRed - 2 * (image[i - 1][j].rgbtRed) - 1 *  image[i - 1][j + 1].rgbtRed);

        GyG = (- 1 * image[i - 1][j - 1].rgbtGreen - 2 * (image[i - 1][j].rgbtGreen) - 1 * image[i - 1][j + 1].rgbtGreen);

        GyB = (- 1 * image[i - 1][j - 1].rgbtBlue - 2 * (image[i - 1][j].rgbtBlue) - 1 * image[i - 1][j + 1].rgbtBlue);

        GxyR = roundf(sqrt(pow(GxR, 2) + pow(GyR, 2)));
        GxyG = roundf(sqrt(pow(GxG, 2) + pow(GyG, 2)));
        GxyB = roundf(sqrt(pow(GxB, 2) + pow(GyB, 2)));

        if (GxyR > 255)
        {
            GxyR = 255;
        }
        if (GxyG > 255)
        {
            GxyG = 255;
        }
        if (GxyB > 255)
        {
            GxyB = 255;
        }

        imagetemp[i][j].rgbtRed = GxyR;
        imagetemp[i][j].rgbtGreen = GxyG;
        imagetemp[i][j].rgbtBlue = GxyB;
    }

// Left vertical border line
    j = 0;
    for (i = 1 ; i < (height - 1) ; i ++)   // For every pixel, exept from the corners
    {
        GxR = ((image[i - 1][j + 1].rgbtRed) + 2 * (image[i][j + 1].rgbtRed) + image[i + 1][j + 1].rgbtRed);

        GxG = ((image[i - 1][j + 1].rgbtGreen) + 2 * (image[i][j + 1].rgbtGreen) + image[i + 1][j + 1].rgbtGreen);

        GxB = ((image[i - 1][j + 1].rgbtBlue) + 2 * (image[i][j + 1].rgbtBlue) + image[i + 1][j + 1].rgbtBlue);

        GyR = (- 1 * image[i - 1][j + 1].rgbtRed - 2 * (image[i - 1][j].rgbtRed) + image[i + 1][j + 1].rgbtRed +
               2 * (image[i + 1][j].rgbtRed));

        GyG = (- 1 * image[i - 1][j + 1].rgbtGreen - 2 * (image[i - 1][j].rgbtGreen) + image[i + 1][j + 1].rgbtGreen +
               2 * (image[i + 1][j].rgbtGreen));

        GyB = (- 1 * image[i - 1][j + 1].rgbtBlue - 2 * (image[i - 1][j].rgbtBlue) + image[i + 1][j + 1].rgbtBlue +
               2 * (image[i + 1][j].rgbtBlue));

        GxyR = roundf(sqrt(pow(GxR, 2) + pow(GyR, 2)));
        GxyG = roundf(sqrt(pow(GxG, 2) + pow(GyG, 2)));
        GxyB = roundf(sqrt(pow(GxB, 2) + pow(GyB, 2)));

        if (GxyR > 255)
        {
            GxyR = 255;
        }
        if (GxyG > 255)
        {
            GxyG = 255;
        }
        if (GxyB > 255)
        {
            GxyB = 255;
        }

        imagetemp[i][j].rgbtRed = GxyR;
        imagetemp[i][j].rgbtGreen = GxyG;
        imagetemp[i][j].rgbtBlue = GxyB;
    }

// Right vertical border line
    j = width - 1;
    for (i = 1 ; i < (height - 1) ; i ++)  // For every pixel, exept from the corners
    {
        GxR = (- 1 * (image[i - 1][j - 1].rgbtRed) - 2 * (image[i][j - 1].rgbtRed) - 1 * (image[i + 1][j - 1].rgbtRed));

        GxG = (- 1 * (image[i - 1][j - 1].rgbtGreen) - 2 * (image[i][j - 1].rgbtGreen) - 1 * (image[i + 1][j - 1].rgbtGreen));

        GxB = (- 1 * (image[i - 1][j - 1].rgbtBlue) - 2 * (image[i][j - 1].rgbtBlue) - 1 * (image[i + 1][j - 1].rgbtBlue));

        GyR = (- 1 * image[i - 1][j - 1].rgbtRed - 2 * (image[i - 1][j].rgbtRed) + image[i + 1][j - 1].rgbtRed +
               2 * (image[i + 1][j].rgbtRed));

        GyG = (- 1 * image[i - 1][j - 1].rgbtGreen - 2 * (image[i - 1][j].rgbtGreen) + image[i + 1][j - 1].rgbtGreen +
               2 * (image[i + 1][j].rgbtGreen));

        GyB = (- 1 * image[i - 1][j - 1].rgbtBlue - 2 * (image[i - 1][j].rgbtBlue) + image[i + 1][j - 1].rgbtBlue +
               2 * (image[i + 1][j].rgbtBlue));

        GxyR = roundf(sqrt(pow(GxR, 2) + pow(GyR, 2)));
        GxyG = roundf(sqrt(pow(GxG, 2) + pow(GyG, 2)));
        GxyB = roundf(sqrt(pow(GxB, 2) + pow(GyB, 2)));

        if (GxyR > 255)
        {
            GxyR = 255;
        }
        if (GxyG > 255)
        {
            GxyG = 255;
        }
        if (GxyB > 255)
        {
            GxyB = 255;
        }

        imagetemp[i][j].rgbtRed = GxyR;
        imagetemp[i][j].rgbtGreen = GxyG;
        imagetemp[i][j].rgbtBlue = GxyB;
    }

// Top left corner
    i = 0;
    j = 0;

    GxR = (2 * (image[i][j + 1].rgbtRed) + image[i + 1][j + 1].rgbtRed);

    GxG = (2 * (image[i][j + 1].rgbtGreen) + image[i + 1][j + 1].rgbtGreen);

    GxB = (2 * (image[i][j + 1].rgbtBlue) + image[i + 1][j + 1].rgbtBlue);

    GyR = (image[i + 1][j + 1].rgbtRed + 2 * (image[i + 1][j].rgbtRed));

    GyG = (image[i + 1][j + 1].rgbtGreen + 2 * (image[i + 1][j].rgbtGreen));

    GyB = (image[i + 1][j + 1].rgbtBlue + 2 * (image[i + 1][j].rgbtBlue));

    GxyR = roundf(sqrt(pow(GxR, 2) + pow(GyR, 2)));
    GxyG = roundf(sqrt(pow(GxG, 2) + pow(GyG, 2)));
    GxyB = roundf(sqrt(pow(GxB, 2) + pow(GyB, 2)));

    if (GxyR > 255)
    {
        GxyR = 255;
    }
    if (GxyG > 255)
    {
        GxyG = 255;
    }
    if (GxyB > 255)
    {
        GxyB = 255;
    }

    imagetemp[i][j].rgbtRed = GxyR;
    imagetemp[i][j].rgbtGreen = GxyG;
    imagetemp[i][j].rgbtBlue = GxyB;

// Bottom left corner
    i = height - 1;
    j = 0;

    GxR = (2 * (image[i][j + 1].rgbtRed) + image[i - 1][j + 1].rgbtRed);

    GxG = (2 * (image[i][j + 1].rgbtGreen) + image[i - 1][j + 1].rgbtGreen);

    GxB = (2 * (image[i][j + 1].rgbtBlue) + image[i - 1][j + 1].rgbtBlue);

    GyR = (- 1 * (image[i - 1][j + 1].rgbtRed) - 2 * (image[i - 1][j].rgbtRed));

    GyG = (- 1 * (image[i - 1][j + 1].rgbtGreen) - 2 * (image[i - 1][j].rgbtGreen));

    GyB = (- 1 * (image[i - 1][j + 1].rgbtBlue) - 2 * (image[i - 1][j].rgbtBlue));

    GxyR = roundf(sqrt(pow(GxR, 2) + pow(GyR, 2)));
    GxyG = roundf(sqrt(pow(GxG, 2) + pow(GyG, 2)));
    GxyB = roundf(sqrt(pow(GxB, 2) + pow(GyB, 2)));

    if (GxyR > 255)
    {
        GxyR = 255;
    }
    if (GxyG > 255)
    {
        GxyG = 255;
    }
    if (GxyB > 255)
    {
        GxyB = 255;
    }

    imagetemp[i][j].rgbtRed = GxyR;
    imagetemp[i][j].rgbtGreen = GxyG;
    imagetemp[i][j].rgbtBlue = GxyB;

// Top right corner
    i = 0;
    j = width - 1;

    GxR = (- 2 * (image[i][j - 1].rgbtRed) - 1 * (image[i + 1][j - 1].rgbtRed));

    GxG = (- 2 * (image[i][j - 1].rgbtGreen) - 1 * (image[i + 1][j - 1].rgbtGreen));

    GxB = (- 2 * (image[i][j - 1].rgbtBlue) - 1 * (image[i + 1][j - 1].rgbtBlue));

    GyR = ((image[i + 1][j - 1].rgbtRed) + 2 * (image[i + 1][j].rgbtRed));

    GyG = ((image[i + 1][j - 1].rgbtGreen) + 2 * (image[i + 1][j].rgbtGreen));

    GyB = ((image[i + 1][j - 1].rgbtBlue) + 2 * (image[i + 1][j].rgbtBlue));

    GxyR = roundf(sqrt(pow(GxR, 2) + pow(GyR, 2)));
    GxyG = roundf(sqrt(pow(GxG, 2) + pow(GyG, 2)));
    GxyB = roundf(sqrt(pow(GxB, 2) + pow(GyB, 2)));

    if (GxyR > 255)
    {
        GxyR = 255;
    }
    if (GxyG > 255)
    {
        GxyG = 255;
    }
    if (GxyB > 255)
    {
        GxyB = 255;
    }

    imagetemp[i][j].rgbtRed = GxyR;
    imagetemp[i][j].rgbtGreen = GxyG;
    imagetemp[i][j].rgbtBlue = GxyB;

    // Bottom right corner
    i = height - 1;
    j = width - 1;

    GxR = (- 2 * (image[i][j - 1].rgbtRed) - 1 * (image[i - 1][j - 1].rgbtRed));

    GxG = (- 2 * (image[i][j - 1].rgbtGreen) - 1 * (image[i - 1][j - 1].rgbtGreen));

    GxB = (- 2 * (image[i][j - 1].rgbtBlue) - 1 * (image[i - 1][j - 1].rgbtBlue));

    GyR = (-(image[i - 1][j - 1].rgbtRed) - 2 * (image[i - 1][j].rgbtRed));

    GyG = (-(image[i - 1][j - 1].rgbtGreen) - 2 * (image[i - 1][j].rgbtGreen));

    GyB = (-(image[i - 1][j - 1].rgbtBlue) - 2 * (image[i - 1][j].rgbtBlue));

    GxyR = roundf(sqrt(pow(GxR, 2) + pow(GyR, 2)));
    GxyG = roundf(sqrt(pow(GxG, 2) + pow(GyG, 2)));
    GxyB = roundf(sqrt(pow(GxB, 2) + pow(GyB, 2)));

    if (GxyR > 255)
    {
        GxyR = 255;
    }
    if (GxyG > 255)
    {
        GxyG = 255;
    }
    if (GxyB > 255)
    {
        GxyB = 255;
    }

    imagetemp[i][j].rgbtRed = GxyR;
    imagetemp[i][j].rgbtGreen = GxyG;
    imagetemp[i][j].rgbtBlue = GxyB;

    // Transfer the temporal image to the image
    for (i = 0 ; i < height ; i ++) // For every row
    {
        for (j = 0 ; j < width ; j ++)  // For every pixel
        {
            image[i][j].rgbtRed = imagetemp[i][j].rgbtRed;
            image[i][j].rgbtGreen = imagetemp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = imagetemp[i][j].rgbtBlue;

        }
    }

    return;
}