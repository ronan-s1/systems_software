#include <stdio.h>

float areaOfCircle(float radius) 
{
    float area = 3.141 * radius * radius;
    printf("The area of the circle is %.2f\n", area);
    return area;
}
