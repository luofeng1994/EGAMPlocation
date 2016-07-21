#include "toaMeasureGen.h"

void toaMeasureGen(param *Param) {
	int i,j;

	for(i=0;i<N;i++){
		for(j=0;j<N;j++) {
			Param->rDist[i][j]=Param->rDist_real[i][j]+genRealGauss(Param->noise_agent[i][j]);
		}
	}

	for(i=0;i<N;i++){
		for(j=0;j<ANCHORS;j++) {
			Param->dist_anc[i][j]=Param->dist_anc_real[i][j]+genRealGauss(Param->noise_anc[i][j]);
		}
	}

}