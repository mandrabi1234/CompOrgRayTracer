#include <stdio.h>
#include <math.h>
#include "rayHeader.h"
//#include "rayFunctions.c"

int nsurf = 5;
Surface surf[] = {{0.0, 45, 1.0},
                  {0.03,9.0,1.67},
                  {-0.05,4.5,1.728},
                  {-0.5,43.55158,1.0},
                  {0.0, 0.0, 1.0}};
//sphere surfaces as an array
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

int trace(RAY *in, RAY *out, Surface *surf){
    int i;
    double rni, rno, curv_Vec, start;
    double A,B,C,D;
    double root, power;
    rni = in->refractionIndex;
    rno = surf->refractionIndex;
    curv_Vec = surf->curvature;
    out->start[0] = in->start[0];
    out->start[1] = in->start[1];
    out->start[2] = in->start[2] - in->distanceToPoint;
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
    for (i=0; i<3; i++) out->start[i] += start * in->rayDirection[i];
    for (i=0; i<3; i++) out->norm[i] = -curv_Vec*out->start[i];
    out->norm[2] += 1.0;
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


int print_ray(RAY *ray)
{
    if (ray->error==-1) {
        printf("Surface Not Hit\n");
        return ray->error;
    }
    printf("surface intersection: ");
    print_vector(ray->start);
    if (ray->error==0) {
        printf("optical direction cosines: ");
        print_vector(ray->rayDirection);
    }
    printf("surface normal: ");
    print_vector(ray->norm);
    printf("Distance: %12.6f G_COSI %12.6f G_COSR %12.6f\n",
           ray->distanceToPoint,ray->gcosi,ray->gcosr);
    if (ray->error==-2) {
        printf("TOTAL INTERNAL REFLECTION\n");
        return ray->error;
    }
    return ray->error;
}


void print_vector(double v[3])
{
    printf("%12.6f %12.6f %12.6f\n",v[0],v[1],v[2]);
}


double dotProduct(double a[3], double b[3])
{
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

void vectorNormalize(double vector[3], double normalize)
{
    int i;
    double vector_normalized;
    vector_normalized = vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2];
    if (vector_normalized==0.0) return;
    vector_normalized = normalize/sqrt(vector_normalized);
    for (i=0; i<3; i++) vector[i] *= vector_normalized;
}

int main()
{
    int i, iret;
    RAY ray[5], enp[1], *obj;
    obj = ray;
    obj->start[0] = 0.0;
    obj->start[1] = 0.0;
    obj->start[2] = 0.0;
    enp->start[0] = 0.0;
    enp->start[1] = 12.5;
    enp->start[2] = 0.0;
    /* calculate ray optical direction cosines */
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


