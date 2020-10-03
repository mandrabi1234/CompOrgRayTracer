#include <stdio.h>
#include <math.h>
#include "rayHeader.h"
#include "rayFunctions.c"

/*------------PROJECT INFORMATION------------*/
/*
    Authors: Mohi Andrabi, Dan Scal, Jack Stark
    Course Info: Computer Organization, Professor Thomas Yeh, Fall 2020
    Title: Software Ray Tracer in C
*/


/*----Main Method Execution Flow----*/
/*
    1. Define necessary variables, pointers, etc.
        * ray ()
        * enp ()
        * *obj (object pointer)
    2. Define the object point
        *
    3. Define the entrance pupil aim point
        *
    4. Calculate the ray's optical direction cosines
        *
*/

int main()
{
// 1. Define necessary variables, pointers, etc.
    int i, inView;
    RAY ray[5], end_point[1], *obj;
    obj = ray;

// 2. Define the object point
    obj->start[0] = 0.0;
    obj->start[1] = 0.0;
    obj->start[2] = 0.0;

// 3. Define the entrance pupil aim point
    end_point->start[0] = 0.0;
    end_point->start[1] = 12.5;
    end_point->start[2] = 0.0;

// 4. Calculate ray optical direction cosines
    for (i=0; i<3; i++) obj->rayDirection[i] = end_point->start[i] - obj->start[i];
    obj->rayDirection[2] += surf->axialThickness;
    vectorNormalize(obj->rayDirection,surf->refractionIndex);
    for (i=0; i<3; i++) end_point->rayDirection[i] = obj->rayDirection[i];
    end_point->refractionIndex = surf->refractionIndex;
    printf("object point: ");
    print_Vector(obj->start);
    rayTrace(end_point,ray);
    i =1;
    while (i<surfCount) {
        printf("\nSurface %d\n",i);
        inView = print_Ray(ray+i);
        printf("Predefined Surface Color: R:%12.6f G:%12.6f B:%12.6f\n", surf[i].r, surf[i].g, surf[i].b);

        i++;
        if (inView<0) break;
    }
    return 0;
}


