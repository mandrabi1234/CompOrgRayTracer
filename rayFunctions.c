#include <stdio.h>
#include <math.h>
#include "rayHeader.h"
// Created by: Mohi 

/*---------Ray Tracer Functions File---------*/


int surfCount = 5;
// (curvature, axial thickness, refraction, ..., ..., ...)
Surface surf[] = {{0.0, 45, 1.0, 30, 90, 90}, // Represent sphere surfaces as arrays
                  {0.03,9.0,1.67, 20, 90, 30},
                  {-0.05,4.5,1.728, 233, 121, 233},
                  {-0.5,43.55158,1.0, 40, 60, 120},
                  {0.0, 0.0, 1.0, 190, 180, 30}};


/*----Function: tracing a ray through an optical system----*/

int rayTrace(RAY *start, RAY data[]){

//  Declare and initialize necessary variables and pointers 
    int i, image, inView;
    RAY *in, *out;
    Surface *sp;
    sp = surf+1;
    start->distanceToPoint = 0.0;
    in = start;
    out = data+1;
    image = surfCount-1;
 
    for (i=1; i<=image; i++) {
        inView = trace(in, out, sp++);
        if (inView<0) return inView;
        in = out++;
    }
    return 0;
}

/*----Function: calculating the surface intersection and direction of refraction----*/
/*  1. Declare necessary variables, pointers, etc.
        *
    2. Transfer to the current surface's coordinates
        * in->start is axial distance to the present surface upon --INPUT--
        * in->start is the oblique distance to the present surface upon --OUTPUT--
    3. Intersect the present surface
        *
    4. Calculate the point of intersection
        *
    5. Calculate the surface normalized
        *
    6. Refract the ray into the surface
        *
*/

int trace(RAY *in, RAY *out, Surface *surf){

// 1. Declare necessary variables, pointers, etc.  
    int i;
    double rNiether, rNot, curv_Vec, start;
    double A,B,C,D;
    double root, refractionStrength;
    rNiether = in->refractionIndex;
    rNot = surf->refractionIndex;
    curv_Vec = surf->curvature;

// 2. Transfer to the current surface's coordinates
    out->start[0] = in->start[0];
    out->start[1] = in->start[1];
    out->start[2] = in->start[2] - in->distanceToPoint;

// 3. Intersect the present surface
    if (curv_Vec) {
        A = curv_Vec*rNiether*rNiether;
        B = in->rayDirection[2]-curv_Vec*dotProduct(in->rayDirection,out->start);
        C = curv_Vec*dotProduct(out->start,out->start)-2.0*out->start[2];
        D = B*B-A*C;
        if (D<0.0) {
            out->error = -1;
            return out->error;
        }
        D = sqrt(D);
        start = C/(B+(B>0? D: -D));
    }
    else {
        if (in->rayDirection[2]==0.0) {
            out->error =  -1;
            return out->error;
        }
        start = -out->start[2]/in->rayDirection[2];
    }


    in->distanceToPoint = start;
    out->distanceToPoint = surf->axialThickness;

// 4. Calculate the point of intersection
    for (i=0; i<3; i++) out->start[i] += start * in->rayDirection[i]; 

// 5. Calculate the surface normalized
    for (i=0; i<3; i++) out->norm_surf[i] = -curv_Vec*out->start[i];
    out->norm_surf[2] += 1.0;

// 6. Refract the ray into the surface
    out->norm_cosi = dotProduct(in->rayDirection,out->norm_surf);
    root = out->norm_cosi*out->norm_cosi + (rNot+rNiether)*(rNot - rNiether);
    if (root<0.0) {
        out->error = -2;
        return out->error;
    }
    root = sqrt(root);
    out->refrac_cosr = (out->norm_cosi>0.0? root: -root);
    refractionStrength = out->refrac_cosr - out->norm_cosi;
    for (i=0; i<3; i++) out->rayDirection[i] = in->rayDirection[i] + refractionStrength*out->norm_surf[i];
    out->refractionIndex = rNot;
    out->error = 0;
    return out->error;
}

/*----Function: printing all calculated ray data----*/
/*
    1. Missed Output
        *If... indicate the inteded surface was not hit
    2. Surface Interaction Output
        * Regardless of all other outcomes, output information pertaining to surface interaction
    3. ODC Output
        *If... output information regarding the ray's optical direction cosines
    4. Surf Norm Output
        * Output information pertaining to...
    5. Distance Output
        * Output information pertaining to the ray's distance to point
    6. TIR Output
        * If... output information regarding the total internal reflection

*/
int print_Ray(RAY *ray)
{
// 1. Surface Missed Output
    if (ray->error==-1) {
        printf("Ray Did Not Hit\n");
       // printf("No Color");
        return ray->error;
    }

// 2. Surface Interaction Output    
    printf("Surface Intersection: ");
    print_Vector(ray->start);

// 3. ODC Output
    if (ray->error==0) {
        printf("Optical Direction Cosines: ");
        print_Vector(ray->rayDirection);
    }

// 4. Surf Norm Output
    printf("Surface Normalized: ");
    print_Vector(ray->norm_surf);

// 5. Distance Output
    printf("Distance: %12.6f G_COSI %12.6f G_COSR %12.6f\n",
           ray->distanceToPoint,ray->norm_cosi,ray->refrac_cosr);

// 6. TIR Output    
    if (ray->error==-2) {
        printf("TOTAL INTERNAL REFLECTION\n");
        return ray->error;
    }
    return ray->error;
}

/*----Function: printing vector data based on input----*/
void print_Vector(double v[3])
{
    printf("%12.6f %12.6f %12.6f\n",v[0],v[1],v[2]);
}


/*----Function: calculating vector dot product----*/
double dotProduct(double a[3], double b[3])
{
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}


/*----Function: normalizing vector length to a given input value----*/

void vectorNormalize(double vector[3], double normalize)
{
    int i;
    double vector_normalized;

    vector_normalized = vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2];
    
    if (vector_normalized==0.0) return;
    
    vector_normalized = normalize/sqrt(vector_normalized);
    
    for (i=0; i<3; i++) vector[i] *= vector_normalized;
}