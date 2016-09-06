#include "general.h"

extern float pos_anc[ANCHORS][DIM];
extern float pos[N][DIM];
extern float pos_init[N][DIM];

void paramInit(param *Param) {

	int i,j;
	
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			Param->rDist_real[i][j]=0;
		}
	}

	for(i=0;i<N;i++){
		for(j=0;j<ANCHORS;j++){
			Param->dist_anc_real[i][j]=0;
		}
	}

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			Param->rDist[i][j]=0;
		}
	}

	for(i=0;i<N;i++){
		for(j=0;j<ANCHORS;j++){
			Param->dist_anc[i][j]=0;
		}
	}

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			Param->net[i][j]=0;
		}
	}

	for(i=0;i<N;i++){
		for(j=0;j<ANCHORS;j++){
			Param->net_anc[i][j]=0;
		}
	}

	for(i=0;i<N;i++) {
		Param->net_num[i]=0;
	}

	for(i=0;i<N;i++) {
		Param->net_anc_num[i]=0;
	}

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			Param->noise_agent[i][j]=0;
		}
	}

	for(i=0;i<N;i++){
		for(j=0;j<ANCHORS;j++){
			Param->noise_anc[i][j]=0;
		}
	}

	Param->measureNum_anc=0;
	Param->measureNum_rel=0;
	Param->initMode=DVdist;
	Param->measMode=TOA;

	Param->gamParam.noiseUpdate=0;
	Param->gamParam.iter_noise=12;
	Param->gamParam.tol=pow((double)10,-4);
	Param->gamParam.l_max[0]=LMAX_X;
	Param->gamParam.l_max[1]=LMAX_Y;

	for(i=0;i<N;i++){
		Param->gamParam.d_init[i*DIM]=pos_init[i][0];
		Param->gamParam.d_init[i*DIM+1]=pos_init[i][1];
	}

	for(i=0;i<N;i++){
		Param->priorParam.mu[i*DIM]=pos_init[i][0];
		Param->priorParam.mu[i*DIM+1]=pos_init[i][1];
	}

	Param->priorParam.kesai=15;
	Param->priorParam.priorMode=UNIFORM;

}

