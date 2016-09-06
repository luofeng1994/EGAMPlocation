#ifndef POSDATA_H
#define POSDATA_H 1

#include "general.h"
#include "gaussNoiseGen.h"
#include "time.h"

void posGen(param *Param);
void pos_Gen(float pos[N][DIM],float x_max,float x_min,float y_max,float y_min);
void pos_anc_Gen(float pos_anc[ANCHORS][DIM],float x_max,float x_min,float y_max,float y_min);

#endif