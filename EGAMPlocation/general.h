#ifndef GENERAL_H
#define GENERAL_H 1

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
//#include "data.h"

#define ANCHORS 15
#define R 65
#define DAMPGAMP 0.6
#define N 200
#define DIM 2
#define D N*DIM
#define LMAX 100
#define LMAX_X 100
#define LMAX_Y 100
#define TOA 0
#define RSS 1
#define DVdist 0
#define UNIFORM 0
#define GAUSS 1
#define MAXITERATION 60;

#define NOISEVAR0 0.05
#define  NP 1.2

#define PI 3.1415926

typedef struct PriorParam {
	float mu[2*N];
	float kesai;
	int priorMode;
}PriorParam;

typedef struct GamParam {
	int noiseUpdate;
	int iter_noise;
	float tol;
	float l_max[2];
	float d_init[2*N];


}GamParam;

typedef struct param {
	
	float rDist_real[N][N];
	float dist_anc_real[N][ANCHORS];
	float rDist[N][N];
	float dist_anc[N][ANCHORS];
	int net[N][N];
	int net_anc[N][ANCHORS];
	int net_num[N];
	int net_anc_num[N];
	float noise_agent[N][N];
	float noise_anc[N][ANCHORS];
	int measureNum_anc;
	int measureNum_rel;
	int initMode;
	int measMode;
	GamParam gamParam; 
	PriorParam priorParam;
}param;

void paramInit(param *Param);

#endif