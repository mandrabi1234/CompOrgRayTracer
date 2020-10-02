#include <stdio.h>
#include <math.h>
#include "rayHeader.h"
//#include "rayFunctions.c"



int nsurf = 5;

SURF surf[] = {{0.0, -2e10, 1.0},{0.03,9.0,1.67},{-0.05,4.5,1.728},{-0.5,43.55158,1.0},{0.0, 0.0, 1.0}};

int raytrace(RAY *start, RAY data[])
{
    int k, image, iret;
    RAY *in, *out;
    SURF *sp;
    sp = surf+1;
    start->q = 0.0;
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

int trace(RAY *in, RAY *out, SURF *surf)
{
    int i;
    double rni, rno, cv, q;
    double A,B,C,D;
    double root, power;
    rni = in->n;
    rno = surf->n;
    cv = surf->cv;
    out->p[0] = in->p[0];
    out->p[1] = in->p[1];
    out->p[2] = in->p[2] - in->q;

    if (cv) {
        A = cv*rni*rni;
        B = in->k[2]-cv*dot(in->k,out->p);
        C = cv*dot(out->p,out->p)-2.0*out->p[2];
        D = B*B-A*C;
        if (D<0.0) {
            out->error = -1;
            return out->error;
        }
        D = sqrt(D);
        q = C/(B+(B>0? D: -D));
    }
    else {
        if (in->k[2]==0.0) {
            out->error =  -1; /* ray parallel to plane */
            return out->error;
        }
        q = -out->p[2]/in->k[2];
    }
    in->q = q;
    out->q = surf->th;
    for (i=0; i<3; i++) out->p[i] += q * in->k[i];
    for (i=0; i<3; i++) out->norm[i] = -cv*out->p[i];
    out->norm[2] += 1.0;
    out->gcosi = dot(in->k,out->norm);
    root = out->gcosi*out->gcosi + (rno+rni)*(rno - rni);
    if (root<0.0) {
        out->error = -2;
        return out->error;
    }
    root = sqrt(root);
    out->gcosr = (out->gcosi>0.0? root: -root);
    power = out->gcosr - out->gcosi;
    for (i=0; i<3; i++) out->k[i] = in->k[i] + power*out->norm[i];
    out->n = rno;
    out->error = 0;
    return out->error;
}


int print_ray(RAY *ray)
{
    if (ray->error==-1) {
        printf("Surface Not Hit\n");
        return ray->error;
    }
    printf("surface intersection ");
    print_vector(ray->p);
    if (ray->error==0) {
        printf("optical direction cosines ");
        print_vector(ray->k);
    }
    printf("surface normal ");
    print_vector(ray->norm);
    printf("q %12.6f G_COSI %12.6f G_COSR %12.6f\n",
           ray->q,ray->gcosi,ray->gcosr);
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


double dot(double a[3], double b[3])
{
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

void vectorNormalize(double v[3], double norm)
{
    int i;
    double vn;
    vn = v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
    if (vn==0.0) return;
    vn = norm/sqrt(vn);
    for (i=0; i<3; i++) v[i] *= vn;
}

int main()
{
    int i, iret;
    RAY ray[5], enp[1], *obj;
    obj = ray;
    obj->p[0] = 0.0;
    obj->p[1] = 0.0;
    obj->p[2] = 0.0;
    enp->p[0] = 0.0;
    enp->p[1] = 12.5;
    enp->p[2] = 0.0;
    /* calculate ray optical direction cosines */
    for (i=0; i<3; i++) obj->k[i] = enp->p[i] - obj->p[i];
    obj->k[2] += surf->th;
    vectorNormalize(obj->k,surf->n);
    for (i=0; i<3; i++) enp->k[i] = obj->k[i];
    enp->n = surf->n;
    printf("object point: ");
    print_vector(obj->p);
    raytrace(enp,ray);
    for (i=1; i<nsurf; i++) {
        printf("\nSurface %d\n",i);
        iret = print_ray(ray+i);
        if (iret<0) break;
    }
    return 0;
}


