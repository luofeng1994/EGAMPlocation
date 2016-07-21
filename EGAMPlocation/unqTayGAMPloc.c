#include "unqTayGAMPloc.h"

extern float pos_anc[ANCHORS][DIM];
extern float pos_init[N][DIM];

float *y_anc;
float **A_anc_elements;
int *A_anc_index;
float *noise_anc;

float *y_rel;
float **A_rel_elements;
int **A_rel_index;
float *noise_rel;

float *y;
float **A_elements;
int **A_index;
float **Av_elements;
int **Av_index;
int Av_num[D]={0};
float **A2_elements;
float **Av2_elements;
float *noiseVar;


int meaNum_anc;
int meaNum_rel;
int meaNum;

void unqTayGAMPloc(param *Param,float pos_gamp[N][DIM]){
	int i,j,iter=0,ttt=0;
	float pos_[D][DIM];
	//int ITER=(int)MAXITERATION;

	float *xhat=&(Param->gamParam.d_init[0]);
	float *Lmax=&(Param->gamParam.l_max[0]);//D*1数组
	float *xhat_prev=(float *)malloc(D*sizeof(float));
	float *vx=(float *)malloc(D*sizeof(float));//D*1数组
	float *vx_prev=(float *)malloc(D*sizeof(float));
	float *shat;//ROW*1数组
	float *vp;//ROW*1数组
	float *phat;
	float *vs;
	float *vr=(float *)malloc(D*sizeof(float));
	float *rhat=(float *)malloc(D*sizeof(float));
	FILE *file;

	measureNumGen(Param);//////////////////////////
	
	meaNum_anc=Param->measureNum_anc;
	meaNum_rel=Param->measureNum_rel;
	meaNum=meaNum_anc+meaNum_rel;

//******************************************************得到测量数据个数后，对一些矩阵进行动态内存分配******************************************************************
	y_anc=(float *)malloc(meaNum_anc*sizeof(float));
	A_anc_elements=Malloc_float(meaNum_anc,2);
	A_anc_index=(int *)malloc(meaNum_anc*sizeof(int));
	noise_anc=(float *)malloc(meaNum_anc*sizeof(float));
	y_rel=(float *)malloc(meaNum_rel*sizeof(float));
	A_rel_elements=Malloc_float(meaNum_rel,4);
	A_rel_index=Malloc_int(meaNum_rel,2);
	noise_rel=(float *)malloc(meaNum_rel*sizeof(float));
	y=(float *)malloc(meaNum*sizeof(float));
	A_elements=Malloc_float(meaNum,4);
	A_index=Malloc_int(meaNum,2);
	A2_elements=Malloc_float(meaNum,4);
	Av_elements=(float **)malloc(D*sizeof(float *));
	Av_index=(int **)malloc(D*sizeof(int *));
	Av2_elements=(float **)malloc(D*sizeof(float *));
	noiseVar=(float *)malloc(meaNum*sizeof(float));
	shat=(float *)malloc(meaNum*sizeof(float));
	vp=(float *)malloc(meaNum*sizeof(float));
	phat=(float *)malloc(meaNum*sizeof(float));
	vs=(float *)malloc(meaNum*sizeof(float));
//******************************************************计算y,A,Av,noiseVar矩阵****************************************************************************************
	gpuI_locAncMat(pos_init,Param,y_anc,A_anc_elements,A_anc_index,noise_anc);/////////////////////////
	gpuI_formObsMat(pos_init,Param,y_rel,A_rel_elements,A_rel_index,noise_rel);////////////////////////

	for(i=0;i<meaNum_anc;i++) {
		noiseVar[i]=noise_anc[i];
	}
	for(i=0;i<meaNum_rel;i++) {
		noiseVar[meaNum_anc+i]=noise_rel[i];
	}


	for(i=0;i<meaNum_anc;i++) {
		y[i]=y_anc[i];
		A_elements[i][0]=A_anc_elements[i][0];
		A_elements[i][1]=A_anc_elements[i][1];
		A_index[i][0]=A_anc_index[i];
		A2_elements[i][0]=A_elements[i][0]*A_elements[i][0];
		A2_elements[i][1]=A_elements[i][1]*A_elements[i][1];
	}
	for(i=0;i<meaNum_rel;i++) {
		y[meaNum_anc+i]=y_rel[i];
		A_elements[meaNum_anc+i][0]=A_rel_elements[i][0];
		A_elements[meaNum_anc+i][1]=A_rel_elements[i][1];
		A_elements[meaNum_anc+i][2]=A_rel_elements[i][2];
		A_elements[meaNum_anc+i][3]=A_rel_elements[i][3];
		A_index[meaNum_anc+i][0]=A_rel_index[i][0];
		A_index[meaNum_anc+i][1]=A_rel_index[i][1];
		A2_elements[meaNum_anc+i][0]=A_elements[meaNum_anc+i][0]*A_elements[meaNum_anc+i][0];
		A2_elements[meaNum_anc+i][1]=A_elements[meaNum_anc+i][1]*A_elements[meaNum_anc+i][1];
		A2_elements[meaNum_anc+i][2]=A_elements[meaNum_anc+i][2]*A_elements[meaNum_anc+i][2];
		A2_elements[meaNum_anc+i][3]=A_elements[meaNum_anc+i][3]*A_elements[meaNum_anc+i][3];

	}

	for(i=0;i<N;i++) {
		Av_num[i*DIM]=0;
		Av_num[i*DIM+1]=0;
		for(j=0;j<meaNum_anc;j++) {
			if(A_anc_index[j]==i) {
				Av_num[i*DIM]++;
				Av_num[i*DIM+1]++;
			}
		}

		for(j=0;j<meaNum_rel;j++) {
			if(A_rel_index[j][0]==i||A_rel_index[j][1]==i) {
				Av_num[i*DIM]++;
				Av_num[i*DIM+1]++;
			}
		}

		Av_elements[i*DIM]=(float *)malloc(Av_num[i*DIM]*sizeof(float));
		Av_elements[i*DIM+1]=(float *)malloc(Av_num[i*DIM+1]*sizeof(float));
		Av_index[i*DIM]=(int *)malloc(Av_num[i*DIM]*sizeof(int));
		Av_index[i*DIM+1]=(int *)malloc(Av_num[i*DIM+1]*sizeof(int));
		Av2_elements[i*DIM]=(float *)malloc(Av_num[i*DIM]*sizeof(float));
		Av2_elements[i*DIM+1]=(float *)malloc(Av_num[i*DIM+1]*sizeof(float));

		ttt=0;
		for(j=0;j<meaNum_anc;j++) {
			if(A_anc_index[j]==i) {
				Av_index[i*DIM][ttt]=j;
				Av_index[i*DIM+1][ttt]=j;
				Av_elements[i*DIM][ttt]=A_anc_elements[j][0];
				Av_elements[i*DIM+1][ttt]=A_anc_elements[j][1];
				Av2_elements[i*DIM][ttt]=Av_elements[i*DIM][ttt]*Av_elements[i*DIM][ttt];
				Av2_elements[i*DIM+1][ttt]=Av_elements[i*DIM+1][ttt]*Av_elements[i*DIM+1][ttt];
				ttt++;
			}
		}

		for(j=0;j<meaNum_rel;j++) {
			if(A_rel_index[j][0]==i) {
				Av_index[i*DIM][ttt]=meaNum_anc+j;
				Av_index[i*DIM+1][ttt]=meaNum_anc+j;
				Av_elements[i*DIM][ttt]=A_rel_elements[j][0];
				Av_elements[i*DIM+1][ttt]=A_rel_elements[j][1];
				Av2_elements[i*DIM][ttt]=Av_elements[i*DIM][ttt]*Av_elements[i*DIM][ttt];
				Av2_elements[i*DIM+1][ttt]=Av_elements[i*DIM+1][ttt]*Av_elements[i*DIM+1][ttt];
				ttt++;
			}
			else if(A_rel_index[j][1]==i) {
				Av_index[i*DIM][ttt]=meaNum_anc+j;
				Av_index[i*DIM+1][ttt]=meaNum_anc+j;
				Av_elements[i*DIM][ttt]=A_rel_elements[j][2];
				Av_elements[i*DIM+1][ttt]=A_rel_elements[j][3];
				Av2_elements[i*DIM][ttt]=Av_elements[i*DIM][ttt]*Av_elements[i*DIM][ttt];
				Av2_elements[i*DIM+1][ttt]=Av_elements[i*DIM+1][ttt]*Av_elements[i*DIM+1][ttt];
				ttt++;
			}
		}
		
	}
//************************************************************************************************************************************************

	//for(i=0;i<meaNum;i++) {
	//	printf("%d  %f\n",i,y[i]);
	//
	//}
	//for(i=0;i<meaNum_anc+meaNum_rel;i++) {
	//	printf("%d  %d %d  %f  %f  %f  %f\n",i,A_index[i][0],A_index[i][1],A_elements[i][0],A_elements[i][1],A_elements[i][2],A_elements[i][3]);
	//}

	//for(i=0;i<D;i++) {
	//	printf("%d   ",Av_num[i]);
	//	for(j=0;j<Av_num[i];j++)  {
	//		printf("%f ",Av2_elements[i][j]);
	//	}
	//	printf("\n");
	//}
	printf("************************\n");
	printf("**************************\n");
	for(i=0;i<N;i++) {
		vx[i*DIM]=pow(Param->gamParam.l_max[0],2)/3;
		vx[i*DIM+1]=pow(Param->gamParam.l_max[1],2)/3;
	}
	for(i=0;i<meaNum;i++) {
		shat[i]=0;
	}

	iter=0;


	while(iter<60) {
		char filename[70]="G:\\c_code\\EGAMPlocation\\EGAMPlocation\\data\\data_";
		char iter_name[4];
		sprintf(iter_name,"%d",iter);
		strcat(filename,iter_name);
		strcat(filename,".txt");
		file=fopen(filename,"w+");


		for(i=0;i<D;i++) {
			xhat_prev[i]=xhat[i];
			vx_prev[i]=vx[i];
		}
		matvecMult(A2_elements,A_index,vx,vp);
		matvecMult(A_elements,A_index,xhat,phat);
		for(i=0;i<meaNum;i++) {
			phat[i]=phat[i]-vp[i]*shat[i];
		}
		shat_vs_update(y,phat,vp,noiseVar,shat,vs);
		vmatvecMult(Av2_elements,Av_index,Av_num,vs,vr);
		vmatvecMult(Av_elements,Av_index,Av_num,shat,rhat);

		for(i=0;i<D;i++) {
			vr[i]=1/vr[i];
		}
		for(i=0;i<D;i++) {
			rhat[i]=xhat[i]+vr[i]*rhat[i];
		}

		fa_fv(rhat,vr,Lmax,&(Param->priorParam),xhat,vx);

		for(i=0;i<D;i++) {
			xhat[i]=(1-DAMPGAMP)*xhat[i]+DAMPGAMP*xhat_prev[i];
			vx[i]=(1-DAMPGAMP)*vx[i]+DAMPGAMP*vx_prev[i];
		}

		iter++;

		for(i=0;i<N;i++) {
			pos_[i][0]=xhat[i*DIM];
			pos_[i][1]=xhat[i*DIM+1];
		}
		//本次迭代估计值到此获得

		//计算下一次迭代所需的向量、矩阵值
		gpuI_locAncMat(pos_,Param,y_anc,A_anc_elements,A_anc_index,noise_anc);/////////////////////////
		gpuI_formObsMat(pos_,Param,y_rel,A_rel_elements,A_rel_index,noise_rel);////////////////////////

		for(i=0;i<meaNum_anc;i++) {
			noiseVar[i]=noise_anc[i];
		}
		for(i=0;i<meaNum_rel;i++) {
			noiseVar[meaNum_anc+i]=noise_rel[i];
		}


		for(i=0;i<meaNum_anc;i++) {
			y[i]=y_anc[i];
			A_elements[i][0]=A_anc_elements[i][0];
			A_elements[i][1]=A_anc_elements[i][1];
			A_index[i][0]=A_anc_index[i];
			A2_elements[i][0]=A_elements[i][0]*A_elements[i][0];
			A2_elements[i][1]=A_elements[i][1]*A_elements[i][1];
		}
		for(i=0;i<meaNum_rel;i++) {
			y[meaNum_anc+i]=y_rel[i];
			A_elements[meaNum_anc+i][0]=A_rel_elements[i][0];
			A_elements[meaNum_anc+i][1]=A_rel_elements[i][1];
			A_elements[meaNum_anc+i][2]=A_rel_elements[i][2];
			A_elements[meaNum_anc+i][3]=A_rel_elements[i][3];
			A_index[meaNum_anc+i][0]=A_rel_index[i][0];
			A_index[meaNum_anc+i][1]=A_rel_index[i][1];
			A2_elements[meaNum_anc+i][0]=A_elements[meaNum_anc+i][0]*A_elements[meaNum_anc+i][0];
			A2_elements[meaNum_anc+i][1]=A_elements[meaNum_anc+i][1]*A_elements[meaNum_anc+i][1];
			A2_elements[meaNum_anc+i][2]=A_elements[meaNum_anc+i][2]*A_elements[meaNum_anc+i][2];
			A2_elements[meaNum_anc+i][3]=A_elements[meaNum_anc+i][3]*A_elements[meaNum_anc+i][3];

		}


		for(i=0;i<N;i++) {
			Av_num[i*DIM]=0;
			Av_num[i*DIM+1]=0;
			for(j=0;j<meaNum_anc;j++) {
				if(A_anc_index[j]==i) {
					Av_num[i*DIM]++;
					Av_num[i*DIM+1]++;
				}
			}

			for(j=0;j<meaNum_rel;j++) {
				if(A_rel_index[j][0]==i||A_rel_index[j][1]==i) {
					Av_num[i*DIM]++;
					Av_num[i*DIM+1]++;
				}
			}
			//不需要再给以下矩阵分配空间
			//Av_elements[i*DIM]=(float *)malloc(Av_num[i*DIM]*sizeof(float));
			//Av_elements[i*DIM+1]=(float *)malloc(Av_num[i*DIM+1]*sizeof(float));
			//Av_index[i*DIM]=(int *)malloc(Av_num[i*DIM]*sizeof(int));
			//Av_index[i*DIM+1]=(int *)malloc(Av_num[i*DIM+1]*sizeof(int));
			//Av2_elements[i*DIM]=(float *)malloc(Av_num[i*DIM]*sizeof(float));
			//Av2_elements[i*DIM+1]=(float *)malloc(Av_num[i*DIM+1]*sizeof(float));

			ttt=0;
			for(j=0;j<meaNum_anc;j++) {
				if(A_anc_index[j]==i) {
					Av_index[i*DIM][ttt]=j;
					Av_index[i*DIM+1][ttt]=j;
					Av_elements[i*DIM][ttt]=A_anc_elements[j][0];
					Av_elements[i*DIM+1][ttt]=A_anc_elements[j][1];
					Av2_elements[i*DIM][ttt]=Av_elements[i*DIM][ttt]*Av_elements[i*DIM][ttt];
					Av2_elements[i*DIM+1][ttt]=Av_elements[i*DIM+1][ttt]*Av_elements[i*DIM+1][ttt];
					ttt++;
				}
			}

			for(j=0;j<meaNum_rel;j++) {
				if(A_rel_index[j][0]==i) {
					Av_index[i*DIM][ttt]=meaNum_anc+j;
					Av_index[i*DIM+1][ttt]=meaNum_anc+j;
					Av_elements[i*DIM][ttt]=A_rel_elements[j][0];
					Av_elements[i*DIM+1][ttt]=A_rel_elements[j][1];
					Av2_elements[i*DIM][ttt]=Av_elements[i*DIM][ttt]*Av_elements[i*DIM][ttt];
					Av2_elements[i*DIM+1][ttt]=Av_elements[i*DIM+1][ttt]*Av_elements[i*DIM+1][ttt];
					ttt++;
				}
				else if(A_rel_index[j][1]==i) {
					Av_index[i*DIM][ttt]=meaNum_anc+j;
					Av_index[i*DIM+1][ttt]=meaNum_anc+j;
					Av_elements[i*DIM][ttt]=A_rel_elements[j][2];
					Av_elements[i*DIM+1][ttt]=A_rel_elements[j][3];
					Av2_elements[i*DIM][ttt]=Av_elements[i*DIM][ttt]*Av_elements[i*DIM][ttt];
					Av2_elements[i*DIM+1][ttt]=Av_elements[i*DIM+1][ttt]*Av_elements[i*DIM+1][ttt];
					ttt++;
				}
			}
		
		}



		fprintf(file,"pos_\n");
		for(i=0;i<N;i++) {
			fprintf(file,"%f   %f\n",pos_[i][0],pos_[i][1]);
		}
		fprintf(file,"\nvp\n");
		for(i=0;i<meaNum;i++) {
			fprintf(file,"%f   ",vp[i]);
			if((i+1)%10==0)
				fprintf(file,"\n");
		}
		fprintf(file,"\nphat\n");
		for(i=0;i<meaNum;i++) {
			fprintf(file,"%f   ",phat[i]);
			if((i+1)%10==0)
				fprintf(file,"\n");
		}
		fprintf(file,"\nvs\n");
		for(i=0;i<meaNum;i++) {
			fprintf(file,"%f   ",vs[i]);
			if((i+1)%10==0)
				fprintf(file,"\n");
		}
		fprintf(file,"\nshat\n");
		for(i=0;i<meaNum;i++) {
			fprintf(file,"%f   ",shat[i]);
			if((i+1)%10==0)
				fprintf(file,"\n");
		}
		fprintf(file,"\nvr\n");
		for(i=0;i<D;i++) {
			fprintf(file,"%f   ",vr[i]);
			if((i+1)%10==0)
				fprintf(file,"\n");			
		}
		fprintf(file,"\nrhat\n");
		for(i=0;i<D;i++) {
			fprintf(file,"%f   ",rhat[i]);
			if((i+1)%10==0)
				fprintf(file,"\n");			
		}
		fprintf(file,"\nvx\n");
		for(i=0;i<D;i++) {
			fprintf(file,"%f   ",vx[i]);
			if((i+1)%10==0)
				fprintf(file,"\n");
		}
		fprintf(file,"\nnoiseVar\n");
		for(i=0;i<meaNum;i++) {
			fprintf(file,"%f   ",noiseVar[i]);
			if((i+1)%10==0)
				fprintf(file,"\n");
		}
		fprintf(file,"\ny\n");
		for(i=0;i<meaNum;i++) {
			fprintf(file,"%f   ",y[i]);
			if((i+1)%10==0)
				fprintf(file,"\n");
		}
		fprintf(file,"\nA\n");
		for(i=0;i<meaNum;i++) {
			fprintf(file,"%f   %f   %f   %f\n",A_elements[i][0],A_elements[i][1],A_elements[i][2],A_elements[i][3]);
		}
		fprintf(file,"\nAv\n");
		for(i=0;i<N;i++) {
			for(j=0;j<Av_num[i];j++) {
				fprintf(file,"%f   ",Av_elements[i][j]);
			}
			fprintf(file,"\n");
		}



	}
	for(i=0;i<N;i++) {
		pos_gamp[i][0]=xhat[i*DIM];
		pos_gamp[i][1]=xhat[i*DIM+1];
	}













//************************************打印测试部分************************************************************
	//for(i=0;i<D;i++) {
	//	printf("%f\n",xhat[i]);
	//}
	//printf("***********************");
	//for(i=0;i<D;i++) {
	//	printf("%f\n",vx[i]);
	//}

	//for(i=0;i<D;i++) {
	//	printf("%f\n",vx[i]);
	//}
	//printf("**************************\n");

	//for(i=0;i<meaNum;i++) {
	//	printf("%f\n",vs[i]);
	//}
	//printf("***********************");
	//for(i=0;i<meaNum;i++) {
	//	printf("%f\n",shat[i]);
	//}
	//printf("***********************");
	//for(i=0;i<D;i++) {
	//	printf("%f\n",vr[i]);
	//}


	//for(i=0;i<meaNum_anc+meaNum_rel;i++) {
	//	printf("%f\n",y[i]);
	//}
	//
	//for(i=0;i<meaNum_anc+meaNum_rel;i++) {
	//	printf("%d %d  %f  %f  %f  %f\n",A_index[i][0],A_index[i][1],A2_elements[i][0],A2_elements[i][1],A2_elements[i][2],A2_elements[i][3]);
	//}



	//for(i=0;i<D;i++) {
	//	printf("%d\n",Av_num[i]);
	//}

	//for(i=0;i<D;i++) {
	//	for(j=0;j<Av_num[i];j++)  {
	//		printf("%d ",Av_index[i][j]);
	//	}

	//	for(j=0;j<Av_num[i];j++)  {
	//		printf("%f ",Av2_elements[i][j]);
	//	}
	//	printf("\n");
	//}
	

}


void gpuI_locAncMat(float pos[N][DIM],param *Param,float *y_anc,float **A_anc_elements,int *A_anc_index,float *noise_anc) {
	int i,j,row=0;
	float *dist_anc_i;
	int *net_anc_i;
	int net_anc_num_i;
	//float d_anc[N*ANCHORS]={0};

	for(i=0;i<N;i++) {
		float pos_aa[DIM]={pos[i][0],pos[i][1]};//第i个节点坐标
		dist_anc_i=&(Param->dist_anc[i][0]);//第i个节点与左右anchors的连接距离
		net_anc_i=&(Param->net_anc[i][0]);//第i个节点连接的anchors下标
		net_anc_num_i=Param->net_anc_num[i];//第i个节点连接的anchors数量
		for(j=0;j<net_anc_num_i;j++) {
			float d_obs;
			float a[DIM];
			float pos_anc_i[DIM]={pos_anc[*(net_anc_i+j)][0],pos_anc[*(net_anc_i+j)][1]};
			sub_gampVecForm(pos_aa,pos_anc_i,Param,&d_obs,a);
			//d_anc[row]=d_obs;
			y_anc[row]=*(dist_anc_i+*(net_anc_i+j))-d_obs;
			A_anc_elements[row][0]=a[0];
			A_anc_elements[row][1]=a[1];
			A_anc_index[row]=i;
			noise_anc[row]=Param->noise_anc[i][*(net_anc_i+j)];
			row++;
		}
	}

}

void sub_gampVecForm(float pos_a[DIM],float pos_anc[DIM],param *Param,float *d_obs,float a[DIM]){
	float d=norm(pos_a[0],pos_a[1],pos_anc[0],pos_anc[1]);
	float marg[DIM]={pos_a[0]-pos_anc[0],pos_a[1]-pos_anc[1]};
	float d1=marg[0]/d;
	float d2=marg[1]/d;
	*d_obs=d-d1*pos_a[0]-d2*pos_a[1];
	a[0]=d1;
	a[1]=d2;
}

void gpuI_formObsMat(float pos[N][DIM],param *Param,float *y_rel,float **A_rel_elements,int **A_rel_index,float *noise_rel) {
	int i,j,row=0;
	float *rDist_i;
	int *net_i;
	int net_num_i;
	//float d_anc[N*ANCHORS]={0};



	for(i=0;i<N;i++) {
		float pos_aa[DIM]={pos[i][0],pos[i][1]};//第i个节点坐标
		rDist_i=&(Param->rDist[i][0]);//第i个节点与其他节点的连接距离
		net_i=&(Param->net[i][0]);//第i个节点连接的节点下标
		net_num_i=Param->net_num[i];//第i个节点连接的节点数量

		//printf("%d  \n",i);

		for(j=0;j<net_num_i;j++) {
			if(*(net_i+j)>i) {
				float d_obs;
				float a[2*DIM];
				float pos_i[DIM]={pos[*(net_i+j)][0],pos[*(net_i+j)][1]};
				TaylorRowForm(pos_aa,pos_i,Param,&d_obs,a);
				y_rel[row]=*(rDist_i+*(net_i+j))-d_obs;
				A_rel_elements[row][0]=a[0];
				A_rel_elements[row][1]=a[1];
				A_rel_elements[row][2]=a[2];
				A_rel_elements[row][3]=a[3];
				A_rel_index[row][0]=i;
				A_rel_index[row][1]=*(net_i+j);
				noise_rel[row]=Param->noise_agent[i][*(net_i+j)];
				row++;

				//printf("%f  %f  %f  %f\n",a[0],a[1],a[2],a[3]);
			}
		}
		//printf("\n");
	}	
}

void TaylorRowForm(float pos_a[DIM],float pos_b[DIM],param *Param,float *d_obs,float a[2*DIM]){
	int i,j;
	float d=norm(pos_a[0],pos_a[1],pos_b[0],pos_b[1]);
	float sqrt_div=1/d;
	float pos[2*DIM]={pos_a[0],pos_b[0],pos_a[1],pos_b[1]};
	float C=d;
	float vec_tay[2*DIM]={sqrt_div*(pos[0]-pos[1]),sqrt_div*(pos[1]-pos[0]),sqrt_div*(pos[2]-pos[3]),sqrt_div*(pos[3]-pos[2])};
	float coef_one[2*DIM];
	int index[2*DIM]={0,2,1,3};

	for(i=0;i<2*DIM;i++) {
		C=C-vec_tay[i]*pos[i];
		coef_one[i]=vec_tay[i];
	}

	a[0]=coef_one[index[0]];
	a[1]=coef_one[index[1]];
	a[2]=coef_one[index[2]];
	a[3]=coef_one[index[3]];
	*d_obs=C;

}

void matvecMult(float **A,int **A_index,float *v,float *p) {
	int i,j;
	for(i=0;i<meaNum_anc;i++) {
		p[i]=A[i][0]*v[A_index[i][0]*DIM]+\
			 A[i][1]*v[A_index[i][0]*DIM+1];
	}
	for(i=0;i<meaNum_rel;i++) {
		p[meaNum_anc+i]=A[meaNum_anc+i][0]*v[A_index[meaNum_anc+i][0]*DIM]+\
						A[meaNum_anc+i][1]*v[A_index[meaNum_anc+i][0]*DIM+1]+\
						A[meaNum_anc+i][2]*v[A_index[meaNum_anc+i][1]*DIM]+\
						A[meaNum_anc+i][3]*v[A_index[meaNum_anc+i][1]*DIM+1];
	}
}

void shat_vs_update(float *y,float *phat,float *vp,float *noiseVar,float *shat,float *vs) {
	int i,j;
	for(i=0;i<meaNum;i++) {
		shat[i]=(y[i]-phat[i])/(vp[i]+noiseVar[i]);
		vs[i]=1/(vp[i]+noiseVar[i]);
	}
}

void vmatvecMult(float **av_elements,int **av_index,int *av_num,float *vv,float *rr) {
	int i,j;

	for(i=0;i<D;i++) {
		rr[i]=0;
		for(j=0;j<av_num[i];j++) {
			rr[i]+=av_elements[i][j]*vv[av_index[i][j]];
		}
	}
}

void fa_fv(float *rhat,float *vr,float *Lmax,PriorParam *priorParam,float *xhat,float *vx) {
	int i,j;
	int mode=priorParam->priorMode;

	if(mode==GAUSS) {
		
	}
	else if(mode==UNIFORM) {
		for(i=0;i<D;i++) {
			int t=i%DIM;
			float xhat_i;
			float vx_i;
			uniform_sub_fa_fv(rhat[i],vr[i],Lmax[t],&xhat_i,&vx_i);
			xhat[i]=xhat_i;
			vx[i]=vx_i;
		}
	}
}

void uniform_sub_fa_fv(float rhat,float vr,float L,float *xhat,float *vx) {
	float eps=pow(10.0,-5);
	float kesai0,kesai1,kesai2;
	float up=(L-rhat)/(sqrt(2.0*vr));
	float low=(-L-rhat)/(sqrt(2.0*vr));
	float C=sqrt(PI*vr/2)*(erf(up)-erf(low));
	if(C<eps)
		C=eps;
	kesai0=sqrt(PI)/2*(erf(up)-erf(low));
	kesai1=0.5*(exp(-pow(up,2))-exp(-pow(low,2)));
	kesai2=-0.5*(up*exp(-pow(up,2))-low*exp(-pow(low,2))-kesai0);
	*xhat=(2*vr*kesai1+sqrt(2*vr)*rhat*kesai0)/C;
	*vx=(2*sqrt(2.0)*pow(sqrt(vr),3)*kesai2+4*vr*rhat*kesai1+sqrt(2*vr)*pow(rhat,2)*kesai0)/C-pow(*xhat,2);
}