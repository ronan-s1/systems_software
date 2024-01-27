#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "areaFunctions.h"

int main(int argc, char **argv) 
{
    char *shape = argv[1];

    if (strcmp(shape, "Rectangle") == 0 && argc == 4) 
    {
        int side1 = atoi(argv[2]);
        int side2 = atoi(argv[3]);
        areaOfRectangle(side1, side2);
    }

    else if (strcmp(shape, "Circle") == 0 && argc == 3)
    {
        float radius = atof(argv[2]);
        areaOfCircle(radius);
    } 

    else if (strcmp(shape, "Triangle") == 0 && argc == 4)
    {
        float base = atof(argv[2]);
        float height = atof(argv[3]);
        areaOfTriangle(base, height);
    } 

    else 
    {
        printf("Invalid arguments or shape.\n");
    }

    return 0;
}

