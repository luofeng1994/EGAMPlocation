#ifndef UNQTAYGAMPLOC_H
#define UNQTAYGAMPLOC_H 1

#include "general.h"
#include "wsnNetGen.h"
#include "measureNumGen.h"
#include "Malloc.h"
#include "math.h"
#include "Erf.h"

void unqTayGAMPloc(param *Param,float pos_gamp[N][DIM]);
void gpuI_locAncMat(float pos[N][DIM],param *Param,float *y_anc,float **A_anc_elements,int *A_anc_index,float *noise_anc);
void sub_gampVecForm(float pos_a[DIM],float pos_anc[DIM],param *Param,float *d_obs,float a[DIM]);
void gpuI_formObsMat(float pos[N][DIM],param *Param,float *y_rel,float **A_rel_elements,int **A_rel_index,float *noise_rel);
void TaylorRowForm(float pos_a[DIM],float pos_b[DIM],param *Param,float *d_obs,float a[2*DIM]);
void matvecMult(float **A,int **A_index,float *v,float *p);
void shat_vs_update(float *y,float *phat,float *vp,float *noiseVar,float *shat,float *vs);
void vmatvecMult(float **av_elements,int **av_index,int *av_num,float *vv,float *rr);
void fa_fv(float *rhat,float *vr,float *Lmax,PriorParam *priorParm,float *xhat,float *vx);
void uniform_sub_fa_fv(float rhat,float vr,float Lmax,float *xhat,float *vx);

//void GampMatParamGen(float pos[N][DIM],param *Param,float *y,float **A_elements,int **A_index,float **Av_elements,int **Av_index,int Av_num[D],float **A2_elements,float **Av2_elements,float *noiseVar);


#endif