#include "dataInit.h"

extern float pos_anc[ANCHORS][DIM];
extern float pos[N][DIM];
extern float pos_init[N][DIM];

extern float rDist[N][N];
extern float dist_anc[N][ANCHORS];
extern int net[N][N];
extern int net_anc[N][ANCHORS];
extern int net_num[N];
extern int net_anc_num[N];
extern float noise_agent[N][N];
extern float noise_anc[N][ANCHORS];

void dataInit() {
	//memset(rDist,1.2,N*N*sizeof(float));
	//memset(dist_anc,2,sizeof(dist_anc));
	//memset(net,1,N*N*sizeof(int));
	//memset(net_anc,1,N*N*sizeof(int));
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			rDist[i][j]=1.2;
		}
	}

	for(i=0;i<N;i++){
		for(j=0;j<ANCHORS;j++){
			dist_anc[i][j]=0;
		}
	}

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			net[i][j]=0;
		}
	}

	for(i=0;i<N;i++){
		for(j=0;j<ANCHORS;j++){
			net_anc[i][j]=0;
		}
	}

	for(i=0;i<N;i++) {
		net_num[i]=0;
	}

	for(i=0;i<N;i++) {
		net_anc_num[i]=0;
	}

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			noise_agent[i][j]=0;
		}
	}

	for(i=0;i<N;i++){
		for(j=0;j<ANCHORS;j++){
			noise_anc[i][j]=0;
		}
	}

}