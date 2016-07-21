#include "Erf.h"

#define ERF_N 100


float erf(float x)////erf(x) = the cumulation of { 2/sqrt(pi)*exp(-z*z) } from 0 to x; //error function
{	// 2/sqrt(pi) * { ¡Æ[(-1)^n / n! * x^(2n+1)/(2n+1)] + x }
	float res = x;
	float factorial = 1;	//n!
	double x_pow = x;
	int one = 1, n;
	if(x>3)
		return 1.0;
	else if(x<-3)
		return -1.0;
	else{
		for( n=1; n<100; n++ ){
			factorial *= n;
			one *= -1;
			x_pow *= x*x;
			res += one / factorial * x_pow / ( 2*n+1 );
		}
		res *= 2 / sqrt(PI);
		return res;
	}
}
