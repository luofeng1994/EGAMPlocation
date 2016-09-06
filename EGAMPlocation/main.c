#include "stdio.h"
#include "stdlib.h"
#include "general.h"
#include "wsnNetGen.h"
#include "noiMatGen.h"
#include "toaMeasureGen.h"
#include "unqTayGAMPloc.h"
#include "posData.h"
#include "posInitObtain.h"

extern float pos[N][DIM];
extern float pos_anc[ANCHORS][DIM];
extern float pos_init[N][DIM];

void main() {

	int i,j;
	param sysParam;
	float pos_gamp[N][DIM];
	char filename1[70]="G:\\c_code\\EGAMPlocation\\EGAMPlocation\\data\\plotdata.txt";
	char filename2[70]="G:\\c_code\\EGAMPlocation\\EGAMPlocation\\data\\paramdata.txt";
	FILE *file1=fopen(filename1,"w+");
	FILE *file2=fopen(filename2,"w+");

	paramInit(&sysParam);
	posGen(&sysParam);
	wsnNetGen(&sysParam);
	noiMatGen(&sysParam);
	toaMeasureGen(&sysParam);
	posInitObtain(&sysParam);
	unqTayGAMPloc(&sysParam,pos_gamp);


	//for(i=0;i<N;i++) {
	//	printf("%f   %f\n",pos[i][0],pos[i][1]);
	//}
	//printf("\n");
	//for(i=0;i<ANCHORS;i++) {
	//	printf("%f  %f\n",pos_anc[i][0],pos_anc[i][1]);
	//}

	//for(i=0;i<N;i++) {
	//	for(j=0;j<N;j++) {
	//		printf("%f ",sysParam.rDist[i][j]);
	//	}
	//	printf("\n");
	//}
	//printf("***********\n");
	//for(i=0;i<N;i++) {
	//	printf("%d   ",sysParam.net_num[i]);
	//	for(j=0;j<sysParam.net_num[i];j++) {
	//		printf("%d  ",sysParam.net[i][j]);
	//	}
	//	printf("\n");
	//}
	//printf("***********");
	//for(i=0;i<N;i++) {
	//	for(j=0;j<ANCHORS;j++) {
	//		printf("%f ",sysParam.dist_anc[i][j]);
	//	}
	//	printf("\n");
	//}
	//printf("***********\n");
	//for(i=0;i<N;i++) {
	//	printf("%d   ",sysParam.net_anc_num[i]);
	//	for(j=0;j<sysParam.net_anc_num[i];j++) {
	//		printf("%d  ",sysParam.net_anc[i][j]);
	//	}
	//	printf("\n");
	//}
	//printf("******************\n");
	//for(i=0;i<N;i++) {
	//	printf("%f   %f\n",pos[i][0],pos[i][1]);
	//}
	//printf("******************\n");
	//for(i=0;i<N;i++) {
	//	printf("%f   %f\n",pos_init[i][0],pos_init[i][1]);
	//}
	printf("******************\n");
	for(i=0;i<N;i++) {
		printf("%f   %f\n",pos_gamp[i][0],pos_gamp[i][1]);
	}
	
	fprintf(file2,"%d\n",N);
	fprintf(file2,"%d\n",ANCHORS);
	for(i=0;i<N;i++) {
		fprintf(file1,"%f  %f\n",pos[i][0],pos[i][1]);
	}
	fprintf(file1,"\n");
	for(i=0;i<ANCHORS;i++) {
		fprintf(file1,"%f  %f\n",pos_anc[i][0],pos_anc[i][1]);
	}
	fprintf(file1,"\n");
	for(i=0;i<N;i++) {
		fprintf(file1,"%f  %f\n",pos_init[i][0],pos_init[i][1]);
	}
	fprintf(file1,"\n");
	for(i=0;i<N;i++) {
		fprintf(file1,"%f  %f\n",pos_gamp[i][0],pos_gamp[i][1]);
	}


}