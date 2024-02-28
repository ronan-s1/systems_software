#include <stdio.h>

float areaOfTriangle(float base, float height) 
{
    float area = 0.5 * base * height;
    printf("The area of the triangle is %.2f\n", area);
    return area;
}
