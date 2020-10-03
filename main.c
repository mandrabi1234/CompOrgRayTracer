#include <stdio.h>
#include <math.h>
#include "rayHeader.h"
#include "rayFunctions.c"
// Created by
//TODO: INPUT FURTHER INFORMATION FOR EACH FUNCTION IN THE COMMENT STRUCTURE

/*------------PROJECT INFORMATION------------*/
/*
    Authors: Mohi Andrabi, Dan Scal, Jack Stark
    Course Info: Comp 249(?), Professor Thomas Yeh
    Title: Software Ray Tracer in C
    Description: //TODO: Add a detailed description of what the ray tracer does

*/

int main()
{
// 1. Define necessary variables, pointers, etc.
    int i, iret;
    RAY ray[5], enp[1], *obj;
    obj = ray;

// 2. Define the object point
    obj->start[0] = 0.0;
    obj->start[1] = 0.0;
    obj->start[2] = 0.0;

// 3. Define the entrance pupil aim point
    enp->start[0] = 0.0;
    enp->start[1] = 12.5;
    enp->start[2] = 0.0;

// 4. Calculate ray optical direction cosines
    for (i=0; i<3; i++) obj->rayDirection[i] = enp->start[i] - obj->start[i];
    obj->rayDirection[2] += surf->axialThickness;
    vectorNormalize(obj->rayDirection,surf->refractionIndex);
    for (i=0; i<3; i++) enp->rayDirection[i] = obj->rayDirection[i];
    enp->refractionIndex = surf->refractionIndex;
    printf("object point: ");
    print_vector(obj->start);
    raytrace(enp,ray);
    for (i=1; i<nsurf; i++) {
        printf("\nSurface %d\n",i);
        iret = print_ray(ray+i);
        printf("Color of Surface: R:%12.6f G:%12.6f B:%12.6f\n", surf[i].r, surf[i].g, surf[i].b);
        if (iret<0) break;
    }
    return 0;
}


