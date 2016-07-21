#include "gaussNoiseGen.h"

float rd_MN1(float m,float n)
{
    float r;
	//srand((unsigned)time(NULL)); 
    r = (   (float)rand() / ((float)(RAND_MAX)+(float)(1)) );   
    r = m + r * (n-m);
    return r;
}

float genRealGauss(float var)
{
	float u1=rd_MN1(0.001,1);
	float u2=rd_MN1(0.001,1);
	float gauss;
	return gauss=sqrt(-2*log((double)u1)*var)*sin(2*PI*(double)u2);  //用(-2lnu1)^0.5*sin(2*pi*u2)产生N(0,1)高斯分布随机数。
}