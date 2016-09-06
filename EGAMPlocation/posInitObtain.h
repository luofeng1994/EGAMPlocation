#ifndef POSINITOBTAIN_H
#define POSINITOBTAIN_H 1

#include "general.h"
#include "Malloc.h"
#include "matrixTool.h"
#include "math.h"
#include "stdio.h"

typedef struct dvdparam {
	float TTLmax;
	int maxIteration;
}dvdparam;

typedef struct dvparam {
	float TTL[ANCHORS];
	float succ[ANCHORS];
	float dist[ANCHORS];
}dvparam;

typedef struct apsunit {
	int aps_num;
	float *aps_dist;
	float **aps_pos;
}apsunit;

void posInitObtain(param *Param);
void distDVlocalize(param *Param,dvdparam *DVDparam,float pos_init[ANCHORS][DIM]);
void DVparamUpdate(float dist,dvparam *DVparamUp,dvparam *DVparamRef,dvdparam *DVDparam,param *Param);
void finalAPSloc(apsunit aps_unit[N],float pos_init[N][DIM]);
void apsLSlocalize(int anc_num,float **anc_pos,float *dist,float pos_est[2]);

#endif