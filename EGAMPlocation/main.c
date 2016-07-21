#include "stdio.h"
#include "stdlib.h"
//#include "general.h"
#include "dataInit.h"
#include "wsnNetGen.h"
#include "noiMatGen.h"
#include "toaMeasureGen.h"
#include "unqTayGAMPloc.h"


extern float pos[N][DIM];
extern float pos_anc[ANCHORS][DIM];
extern float pos_init[N][DIM];

void main() {

	int i,j;
	param sysParam;
	float pos_gamp[N][DIM];
	char filename[70]="G:\\c_code\\EGAMPlocation\\EGAMPlocation\\data\\plotdata.txt";
	FILE *file=fopen(filename,"w+");

	paramInit(&sysParam);
	wsnNetGen(&sysParam);
	noiMatGen(&sysParam);
	toaMeasureGen(&sysParam);
	unqTayGAMPloc(&sysParam,pos_gamp);

	for(i=0;i<N;i++) {
		fprintf(file,"%f  %f\n",pos[i][0],pos[i][1]);
	}
	fprintf(file,"\n");
	for(i=0;i<ANCHORS;i++) {
		fprintf(file,"%f  %f\n",pos_anc[i][0],pos_anc[i][1]);
	}
	fprintf(file,"\n");
	for(i=0;i<N;i++) {
		fprintf(file,"%f  %f\n",pos_init[i][0],pos_init[i][1]);
	}
	fprintf(file,"\n");
	for(i=0;i<N;i++) {
		fprintf(file,"%f  %f\n",pos_gamp[i][0],pos_gamp[i][1]);
	}



	//for(i=0;i<N;i++) {
	//	printf("%f   %f\n",pos_gamp[i][0],pos_gamp[i][1]);
	//}


	//for(i=0;i<N;i++){
	//	for(j=0;j<N;j++){
	//		printf("%f  ",sysParam.rDist_real[i][j]);		
	//	}
	//	printf("\n");
	//}
	//printf("****************\n");

	//for(i=0;i<N;i++){
	//	for(j=0;j<N;j++){
	//		printf("%f  ",sysParam.rDist[i][j]);		
	//	}
	//	printf("\n");
	//}
	//printf("****************\n");
	//for(i=0;i<N;i++){
	//	for(j=0;j<sysParam.net_num[i];j++){
	//		printf("%d  ",sysParam.net[i][j]);		
	//	}
	//	printf("\n");
	//}
	//printf("****************\n");
	//for(i=0;i<N;i++) {
	//	printf("%d\n",sysParam.net_num[i]);
	//}
	//printf("****************\n");
	//for(i=0;i<N;i++){
	//	for(j=0;j<ANCHORS;j++){
	//		printf("%d  ",sysParam.net_anc[i][j]);		
	//	}
	//	printf("\n");
	//}
	//printf("****************\n");
	//for(i=0;i<N;i++) {
	//	printf("%d\n",sysParam.net_anc_num[i]);
	//}
	//printf("****************\n");
	//for(i=0;i<N;i++){
	//	for(j=0;j<N;j++){
	//		printf("%f  ",sysParam.noise_agent[i][j]);		
	//	}
	//	printf("\n");
	//}
	//printf("****************\n");
	//for(i=0;i<N;i++){
	//	for(j=0;j<ANCHORS;j++){
	//		printf("%f  ",sysParam.noise_anc[i][j]);		
	//	}
	//	printf("\n");
	//}

	//for(i=0;i<2*N;i++){
	//	printf("%f\n",sysParam.gamParam.d_init[i]);
	//}
	//printf("****************\n");

	//for(i=0;i<2*N;i++){
	//	printf("%f\n",sysParam.priorParam.mu[i]);
	//}
	//printf("****************\n");

}