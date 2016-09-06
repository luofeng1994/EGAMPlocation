#ifndef SVD_H
#define SVD_H 1

#define MAX_ITERA 60
#define MIN_DOUBLE (1e-30)

#include "stdlib.h"
#include "math.h"

int dluav(float *a,int m,int n,float eps,int ka);
void damul(float a[],float b[],int m,int n,int k,float c[]);
void ppp(float a[],float e[],float s[],float v[],int m,int n);
void sss(float fg[2],float cs[2]);

#endif