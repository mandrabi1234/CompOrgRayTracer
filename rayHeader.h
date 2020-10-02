//
// Created by Jack on 9/30/2020.
//

#ifndef RAY_TRACER_ATTEMPT_3_RAYHEADER_H
#define RAY_TRACER_ATTEMPT_3_RAYHEADER_H
typedef struct {
    double p[3];
    double k[3];
    double n;
    double q;
    double gcosi;
    double gcosr;
    double norm[3];
    int error;
} RAY;

typedef struct {
    double cv;
    double th;
    double n;
} SURF;

int raytrace(RAY *start, RAY data[]);
int trace(RAY *in, RAY *out, SURF *surf);
int print_ray(RAY *ray);
void print_vector(double v[3]);
double dot(double a[3], double b[3]);
void vectorNormalize(double v[3], double norm);

#endif //RAY_TRACER_ATTEMPT_3_RAYHEADER_H
