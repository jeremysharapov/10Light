#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;

  color a = calculate_ambient(alight, areflect);
  color d = calculate_diffuse(light, dreflect, normal);
  color s = calculate_specular(light, sreflect, view, normal);

  i.red = a.red + d.red + s.red;
  i.green = a.green + d.green + s.green;
  i.blue = a.blue + d.blue + s.blue;

  limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a = alight;
  a.red = a.red * areflect[0];
  a.green = a.green * areflect[1];
  a.blue = a.blue * areflect[2];
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  d.red = light[1][0];
  d.green = light[1][1];
  d.blue = light[1][2];
  
  double * l = light[0];
  double * n = normal;
  normalize(l);
  normalize(n);
  
  double dot = dot_product(n, l);
  d.red = d.red * dreflect[0] * dot;
  d.green = d.green * dreflect[1] * dot;
  d.blue = d.blue * dreflect[2] * dot;

  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  s.red = light[1][0];
  s.green = light[1][1];
  s.blue = light[1][2];

  double * l = light[0];
  double * n = normal;
  double * v = view;
  normalize(l);
  normalize(n);
  normalize(v);

  double dot = dot_product(n, l);

  int i;
  for (i = 0; i < 3; i++){
    n[i] = n[i] * 2 * dot;
  }

  for (i = 0; i < 3; i++){
    n[i] = n[i] - l[i];
  }

  dot = dot_product(n, v);
  dot = pow(dot, SPECULAR_EXP);

  s.red = s.red * sreflect[0] * dot;
  s.green = s.green * sreflect[1] * dot;
  s.blue = s.blue * sreflect[2] * dot;
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  if (c -> red > 255){
    c -> red = 255;
  }
  if (c -> red < 0){
    c -> red = 0;
  }
  if (c -> green > 255){
    c -> green = 255;
  }
  if (c -> green < 0){
    c -> green = 0;
  }
  if (c -> blue > 255){
    c -> blue = 255;
  }
  if (c -> blue < 0){
    c -> blue = 0;
  }
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  int i;
  double x, y, z, magnitude;
  x = vector[0];
  y = vector[1];
  z = vector[2];
  magnitude = sqrt(x * x + y * y + z * z);
  for (i = 0; i < 3; i++){
    vector[i] = vector[i] / magnitude;
  }
}

//Return the dot product of a . b
double dot_product( double *a, double *b ) {
  int i;
  double t = 0;
  for (i = 0; i < 3; i++){
    t += (a[i] * b[i]);
  }
  if (t < 0){
    t = 0;
  }
  return t;
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
