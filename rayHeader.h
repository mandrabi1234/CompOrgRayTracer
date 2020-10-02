//
// Created by Jack on 9/30/2020.
//

#ifndef RAY_TRACER_ATTEMPT_3_RAYHEADER_H
#define RAY_TRACER_ATTEMPT_3_RAYHEADER_H
typedef struct {
    double start[3];     //starting point in 3D space, ie x,y,z
    double rayDirection[3];     //direction of ray propagation and spread
    double refractionIndex;       //refraction index ie how shiny something is and how much it will scatter light
    double distanceToPoint;       //distance from camera to point information
    double gcosi;    //normalize dot of k
    double gcosr;   //refracted dot of k
    double norm[3]; //surface normalized
    int error;  //error
} RAY;

typedef struct {
    double curvature;
    double axialThickness;
    double refractionIndex;
} Surface;

typedef struct Color{
    float r;
    float g;
    float b;
} Color;



int raytrace(RAY *start, RAY data[]);
int trace(RAY *in, RAY *out, Surface *surf);
int print_ray(RAY *ray);
void print_vector(double v[3]);
double dotProduct(double a[3], double b[3]);
void vectorNormalize(double v[3], double norm);

#endif //RAY_TRACER_ATTEMPT_3_RAYHEADER_H
