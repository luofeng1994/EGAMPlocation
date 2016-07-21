#include "wsnNetGen.h"

extern float pos_anc[ANCHORS][DIM];
extern float pos[N][DIM];
extern float pos_init[N][DIM];


void wsnNetGen(param *Param) {
	int i,j;


	for(i=0;i<N;i++) {
		int num=0;
		for(j=0;j<N;j++) {
			if(i!=j) {
				float dist=norm(pos[i][0],pos[i][1],pos[j][0],pos[j][1]);
				Param->rDist_real[i][j]=dist;
				if(dist<R){
					Param->net[i][num]=j;
					num++;
				}
			}
		}
		Param->net_num[i]=num;
	}

	for(i=0;i<N;i++) {
		int num=0;
		for(j=0;j<ANCHORS;j++) {
			float dist=norm(pos[i][0],pos[i][1],pos_anc[j][0],pos_anc[j][1]);
			Param->dist_anc_real[i][j]=dist;
			if(dist<R){
				Param->net_anc[i][num]=j;
				num++;
			}
		}
		Param->net_anc_num[i]=num;
	}
}

float norm(float x1,float y1,float x2,float y2) {
	float dist;
	float dist_2=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
	dist = sqrt(dist_2);
	return dist; 
}
