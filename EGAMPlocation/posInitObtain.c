#include "posInitObtain.h"

#define Max(a,b) a>b?a:b
#define Min(a,b) a>b?b:a

extern float pos[N][DIM];
extern float pos_anc[ANCHORS][DIM];

float pos_init[N][DIM];
int writeNum=1;

void posInitObtain(param *Param) {
	int i,j;
	int initMode=Param->initMode,measMode=Param->measMode;
	dvdparam DVDparam;
	if(initMode==DVdist) {
		DVDparam.TTLmax=8;
		DVDparam.maxIteration=15;
		distDVlocalize(Param,&DVDparam,pos_init);
	}

	for(i=0;i<N;i++) {
		pos_init[i][0]=Max(Min(pos_init[i][0],LMAX_X),-LMAX_X);
		pos_init[i][1]=Max(Min(pos_init[i][1],LMAX_Y),-LMAX_Y);
	}

}

void distDVlocalize(param *Param,dvdparam *DVDparam,float pos_init[ANCHORS][DIM]) {
	int i,j,t;
	float TTLmax=DVDparam->TTLmax;
	int maxIteration=DVDparam->maxIteration;
	dvparam DVparam[N];
	apsunit aps_unit[N];

	//DVparamInit
	for(i=0;i<N;i++) {
		for(j=0;j<ANCHORS;j++) {
			DVparam[i].TTL[j]=0;
			DVparam[i].succ[j]=0;
			DVparam[i].dist[j]=0;
		}

		for(j=0;j<Param->net_anc_num[i];j++) {
			int k=Param->net_anc[i][j];
			DVparam[i].TTL[k]=1;
			DVparam[i].succ[k]=1;
			DVparam[i].dist[k]=Param->dist_anc[i][k];			
		}
	}

	for(t=0;t<maxIteration;t++) {
		for(i=0;i<N;i++) {
			for(j=0;j<Param->net_num[i];j++) {
				int index=Param->net[i][j];
				float tmpdist=Param->rDist[i][index];
				DVparamUpdate(tmpdist,&DVparam[i],&DVparam[index],DVDparam,Param);
			}
		}
	}

	//for(i=0;i<N;i++) {
	//	for(j=0;j<ANCHORS;j++) {
	//		printf("%f  ",DVparam[i].TTL[j]);
	//	}
	//	printf("\n");
	//}


	//apsUnitGen
	for(i=0;i<N;i++) {
		int num=0,k=0;
		for(j=0;j<ANCHORS;j++) {
			if(DVparam[i].succ[j]>0) {
				num++;
			}
		}
		aps_unit[i].aps_num=num;
		aps_unit[i].aps_dist=(float *)malloc(num*sizeof(float));
		aps_unit[i].aps_pos=Malloc_float(num,DIM);

		for(j=0;j<ANCHORS;j++) {
			if(DVparam[i].succ[j]>0) {
				aps_unit[i].aps_dist[k]=DVparam[i].dist[j];
				aps_unit[i].aps_pos[k][0]=pos_anc[j][0];
				aps_unit[i].aps_pos[k][1]=pos_anc[j][1];
				k++;
			}
		}

	}

	finalAPSloc(aps_unit,pos_init);
}


void DVparamUpdate(float dist,dvparam *DVparamUp,dvparam *DVparamRef,dvdparam *DVDparam,param *Param) {
	int i,j;
	float TTLmax=DVDparam->TTLmax;
	float *succ_up=DVparamUp->succ,*succ_ref=DVparamRef->succ;
	float *dist_up=DVparamUp->dist,*dist_ref=DVparamRef->dist;
	float *TTLup=DVparamUp->TTL,*TTLref=DVparamRef->TTL;


	for(i=0;i<ANCHORS;i++) {
		if(succ_ref[i]>0) {
			if(succ_up[i]>0) {
				if((dist_ref[i]+dist<dist_up[i])&&(TTLref[i]+1<=TTLmax)){
					TTLup[i]=TTLref[i]+1;
					dist_up[i]=dist_ref[i]+dist;
				}
			}
			else {
				if(TTLref[i]+1<=TTLmax) {
					succ_up[i]=1;
					TTLup[i]=TTLup[i]+1;
					dist_up[i]=dist_ref[i]+dist;
				}
			}
		}
	}

}

void finalAPSloc(apsunit aps_unit[N],float pos_init[N][DIM]) {
	int i,j;
	int anc_num;
	float *dist;
	float **anc_pos;
	float pos_init_est[DIM];      
	for(i=0;i<N;i++) {
		anc_num=aps_unit[i].aps_num;
		if(anc_num>=3) {
			apsLSlocalize(anc_num,aps_unit[i].aps_pos,aps_unit[i].aps_dist,pos_init_est);
		}
		else {
			pos_init_est[0]=0;
			pos_init_est[1]=0;
		}
		pos_init[i][0]=pos_init_est[0];
		pos_init[i][1]=pos_init_est[1];
	}
}

void apsLSlocalize(int anc_num,float **anc_pos,float *dist,float pos_est[2]) {
	int i,j,k;
	float dx[2];
	float **A;
	float *d;
	float *A_serial;
	float *A_t_serial;//A_serial的转置
	float *A_mul;
	float *A_inverse;
	float *A_forSvd;
	float eps=0.000001;
	float sval[2];
	float lambda=0.01;
	float *temp;
	float *pos_get;
		FILE *file;
		char filename[70]="G:\\c_code\\EGAMPlocation\\EGAMPlocation\\data2\\data_";
		char iter_name[4];
		sprintf(iter_name,"%d",writeNum);
		strcat(filename,iter_name);
		strcat(filename,".txt");
		file=fopen(filename,"w+");

	A=Malloc_float(anc_num-1,DIM);
	d=(float *)malloc((anc_num-1)*sizeof(float));
	A_serial=(float*)malloc((anc_num-1)*DIM*sizeof(float));
	A_forSvd=(float*)malloc((anc_num-1)*DIM*sizeof(float));
	A_t_serial=(float*)malloc((anc_num-1)*DIM*sizeof(float));
	A_mul=(float *)malloc(DIM*DIM*sizeof(float));
	A_inverse=(float *)malloc(DIM*DIM*sizeof(float));
	temp=(float *)malloc(DIM*(anc_num-1)*sizeof(float));
	pos_get=(float *)malloc(DIM*sizeof(float));



	for(i=0;i<anc_num-1;i++) {
		dx[0]=anc_pos[i][0]-anc_pos[i+1][0];
		dx[1]=anc_pos[i][1]-anc_pos[i+1][1];
		A[i][0]=2*dx[0];
		A[i][1]=2*dx[1];
		d[i]=pow(dist[i+1],2)-pow(dist[i],2)-((pow(anc_pos[i+1][0],2)-pow(anc_pos[i][0],2))+(pow(anc_pos[i+1][1],2)-pow(anc_pos[i][1],2)));
		A_serial[i*DIM]=A[i][0];
		A_serial[i*DIM+1]=A[i][1];
		A_forSvd[i*DIM]=A[i][0];
		A_forSvd[i*DIM+1]=A[i][1];
	}
	k=dluav(A_forSvd,anc_num-1,DIM,eps,5);//求A_serial矩阵的奇异值
	sval[0]=A_forSvd[0];
	sval[1]=A_forSvd[3];
	transpose(A_serial,anc_num-1,DIM,A_t_serial);//转置
	damul(A_t_serial,A_serial,DIM,anc_num-1,DIM,A_mul);
	for(i=0;i<DIM;i++) {
		A_mul[i*DIM+i]=A_mul[i*DIM+i]+lambda;
	}
	inverse(A_mul,DIM,A_inverse);
	damul(A_inverse,A_t_serial,DIM,DIM,anc_num-1,temp);
	damul(temp,d,DIM,anc_num-1,1,pos_get);
	for(i=0;i<DIM;i++) {
		pos_est[i]=pos_get[i];
	}



	//fprintf(file,"\nd\n");
	//for(i=0;i<anc_num-1;i++) {
	//	fprintf(file,"%f\n",d[i]);
	//}
	//fprintf(file,"\nA\n");
	//for(i=0;i<anc_num-1;i++) {
	//	fprintf(file,"%f   %f\n",A_serial[i*2],A_serial[i*2+1]);
	//}
	//fprintf(file,"\nA_t\n");
	//for(i=0;i<DIM;i++) {
	//	for(j=0;j<anc_num-1;j++) {
	//		fprintf(file,"%f   ",A_t_serial[i*(anc_num-1)+j]);
	//	}
	//}
	//fprintf(file,"\nA_mul\n");
	//for(i=0;i<DIM;i++) {
	//	fprintf(file,"%f   %f\n",A_mul[i*2],A_mul[i*2+1]);
	//}
	//fprintf(file,"\nA_inverse\n");
	//for(i=0;i<DIM;i++) {
	//	fprintf(file,"%f   %f\n",A_inverse[i*2],A_inverse[i*2+1]);
	//}
	//fprintf(file,"\ntemp\n");
	//for(i=0;i<DIM;i++) {
	//	for(j=0;j<anc_num-1;j++) {
	//		fprintf(file,"%f   ",temp[i*(anc_num-1)+j]);
	//	}
	//}
	//fprintf(file,"\npos\n");
	//fprintf(file,"%f   %f\n",pos_est[0],pos_est[1]);
	//writeNum++;
	free(A);
	free(d);
	free(A_serial);
	free(A_t_serial);
	free(A_mul);
	free(A_inverse);
	free(A_forSvd);
	free(temp);
	free(pos_get);
}