#include "measureNumGen.h"

void measureNumGen(param *Param) {
	int i,j;

	Param->measureNum_anc=0;
	Param->measureNum_rel=0;

	for(i=0;i<N;i++) {
		Param->measureNum_anc+=Param->net_anc_num[i];
	}

	for(i=0;i<N;i++) {
		for(j=0;j<Param->net_num[i];j++) {
			if(Param->net[i][j]>i) {
				Param->measureNum_rel+=1;
			}
		}
	}

}