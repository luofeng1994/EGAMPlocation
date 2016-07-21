#include "noiMatGen.h"


void noiMatGen(param *Param) {
	int i,j;

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(i!=j){
				Param->noise_agent[i][j]=NOISEVAR0*pow(Param->rDist_real[i][j],(float)NP)*NOISEVAR0*pow(Param->rDist_real[i][j],(float)NP);
			}
		}
	}

	for(i=0;i<N;i++){
		for(j=0;j<ANCHORS;j++){
			Param->noise_anc[i][j]=NOISEVAR0*pow(Param->dist_anc_real[i][j],(float)NP)*NOISEVAR0*pow(Param->dist_anc_real[i][j],(float)NP);
		}
	}
}