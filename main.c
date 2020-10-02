#include <stdio.h>
#include <math.h>
#include "rayHeader.h"
//#include "rayFunctions.c"
//TODO: INPUT FURTHER INFORMATION FOR EACH FUNCTION IN THE COMMENT STRUCTURE

/*------------PROJECT INFORMATION------------*/
/*
    Authors: Mohi Andrabi, Dan Scal, Jack Stark
    Course Info: Comp 249(?), Professor Thomas Yeh
    Title: Software Ray Tracer in C
    Description: //TODO: Add a detailed description of what the ray tracer does

*/


int nsurf = 5;

Surface surf[] = {{0.0, 45, 1.0}, // Represent sphere surfaces as arrays
                  {0.03,9.0,1.67},
                  {-0.05,4.5,1.728},
                  {-0.5,43.55158,1.0},
                  {0.0, 0.0, 1.0}};


/*----Function: tracing a ray through an optical system----*/
/*
    1. 
    2. 
    3. 


*/
int raytrace(RAY *start, RAY data[]){
    int k, image, iret;
    RAY *in, *out;
    Surface *sp;
    sp = surf+1;
    start->distanceToPoint = 0.0;
    in = start;
    out = data+1;
    image = nsurf-1;
    for (k=1; k<=image; k++) {
        iret = trace(in, out, sp++);
        if (iret<0) return iret;
        in = out++;
    }
    return 0;
}

/*----Function: calculating the surface intersection and direction of refraction----*/
/*  1. Declare necessary variables, pointers, etc.
        *
    2. Transfer to the current surface's coordinates
        * in->start is axial distance to the present surface upon INPUT
        * in->start is the oblique distance to the present surface upon OUTPUT
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
// 1.     
    int i;
    double rni, rno, curv_Vec, start;
    double A,B,C,D;
    double root, power;
    rni = in->refractionIndex;
    rno = surf->refractionIndex;
    curv_Vec = surf->curvature;

// 2. 
    out->start[0] = in->start[0];
    out->start[1] = in->start[1];
    out->start[2] = in->start[2] - in->distanceToPoint;

// 3. 
    if (curv_Vec) {
        A = curv_Vec*rni*rni;
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

// 4. 
    for (i=0; i<3; i++) out->start[i] += start * in->rayDirection[i]; 

// 5. 
    for (i=0; i<3; i++) out->norm[i] = -curv_Vec*out->start[i]; 
    out->norm[2] += 1.0;

// 6. 
    out->gcosi = dotProduct(in->rayDirection,out->norm);
    root = out->gcosi*out->gcosi + (rno+rni)*(rno - rni);
    if (root<0.0) {
        out->error = -2;
        return out->error;
    }
    root = sqrt(root);
    out->gcosr = (out->gcosi>0.0? root: -root);
    power = out->gcosr - out->gcosi;
    for (i=0; i<3; i++) out->rayDirection[i] = in->rayDirection[i] + power*out->norm[i];
    out->refractionIndex = rno;
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
int print_ray(RAY *ray)
{
// 1. Surface Missed Output
    if (ray->error==-1) {
        printf("Surface Not Hit\n");
        return ray->error;
    }

// 2. Surface Interaction Output    
    printf("Surface Intersection: ");
    print_vector(ray->start);

// 3. ODC Output
    if (ray->error==0) {
        printf("Optical Direction Cosines: ");
        print_vector(ray->rayDirection);
    }

// 4. Surf Norm Output
    printf("Surface Normalized: ");
    print_vector(ray->norm);

// 5. Distance Output
    printf("Distance: %12.6f G_COSI %12.6f G_COSR %12.6f\n",
           ray->distanceToPoint,ray->gcosi,ray->gcosr);
// 6. TIR Output    
    if (ray->error==-2) {
        printf("TOTAL INTERNAL REFLECTION\n");
        return ray->error;
    }
    return ray->error;
}

/*----Function: printing vector data based on input----*/
void print_vector(double v[3])
{
    printf("%12.6f %12.6f %12.6f\n",v[0],v[1],v[2]);
}


/*----Function: calculating vector dot product----*/
double dotProduct(double a[3], double b[3])
{
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}


/*----Function: normalizing vector length to a given input value----*/
/*
    1. Declare necessary variables, pointers, etc.
    2. 
    3. 

*/
void vectorNormalize(double vector[3], double normalize)
{
    int i;
    double vector_normalized;
    vector_normalized = vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2];
    if (vector_normalized==0.0) return;
    vector_normalized = normalize/sqrt(vector_normalized);
    for (i=0; i<3; i++) vector[i] *= vector_normalized;
}

/*----Main Method Execution Flow----*/
/*
    1. Define necessary variables, pointers, etc.
        * ray ()
        * enp ()
        * *obj (object pointer)
    2. Define the object point
        * //TODO: SAY WHAT THIS DOES... ADDITIONAL CONTEXT
    3. Define the entrance pupil aim point
        *
    4. Calculate the ray's optical direction cosines
        *
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
        if (iret<0) break;
    }
    return 0;
}


