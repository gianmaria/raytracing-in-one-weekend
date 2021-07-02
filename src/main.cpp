#include <stdio.h>
#include <stdlib.h>

#include "math.h"

int main(void)
{
    Vec3 a = {};
    Vec3 b = {};

    Vec3 res = a + b;

    return 0;
}

int main1()
{
    
    FILE* fp = fopen("output.ppm", "wb");

    int image_width = 256;
    int image_height = 256;

    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", image_width, image_height);
    fprintf(fp, "255\n");

    for (int row = image_height - 1;
        row >= 0;
        --row)
    {
        //printf("Scanlines remaining: %d\n", row);

        for (int col = 0;
            col < image_width;
            ++col)
        {
            float r = (float)col / (float)image_width;
            float g = (float)row / (float)image_height;
            float b = 0.25f;

            int ir = (int)(255.999 * r);
            int ig = (int)(255.999 * g);
            int ib = (int)(255.999 * b);

            fprintf(fp, "%d %d %d\n",
                ir, ig, ib);

        }
    }


    fclose(fp);

    return 0;
}