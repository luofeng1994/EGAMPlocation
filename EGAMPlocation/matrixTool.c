#include "matrixTool.h"

/*********************************************************************
 * 矩阵的奇异值分解，参见《c 常用算法程序集》徐世良P169
 * 参数说明：
 * a m*n的实矩阵，返回时其对角线给出奇异值（非递增顺序），其余元素为0
 * m,n 矩阵A的行数和列数
 * u m*m的矩阵，存放左奇异向量
 * v n*n的矩阵，存放右奇异向量
 * eps 双精度实型变量，给定精度要求
 * ka 整形变量，其值为max(m,n)+1
 * 返回值：如果返回标志小于0，则说明出现了迭代MAX_ITERA次还未求得某个
 * 奇异值的情况，此时矩阵A的分解式为UAV，如果返回标志大于0，则说明
 * 程序正常运行
 ********************************************************************/
int dluav(float *a,int m,int n,float eps,int ka)
{
	int i,j,k,l,it,ll,kk,ix,iy,mm,nn,iz,ml,ks;
	float d,dd,t,sm,sml,eml,sk,ek,b,c,shh,fg[2],cs[2];
	float *s,*e,*w;
	float *u,*v;
	s=(float*)malloc(ka*sizeof(float));
	e=(float*)malloc(ka*sizeof(float));
	w=(float*)malloc(ka*sizeof(float));

	u=(float *)malloc(m*m*sizeof(float));
	v=(float *)malloc(n*n*sizeof(float));
	for(i=1;i<=m;i++)
	{
		ix=(i-1)*m+i-1;
		u[ix]=0;
	}
	for(i=1;i<=n;i++)
	{
		iy=(i-1)*n+i-1;
		v[iy]=0;
	}
	it=MAX_ITERA;k=n;
	if(m-1<n)
		k=m-1;
	l=m;
	if(n-2<m) l=n-2;
	if(l<0) l=0;
	ll=k;
	if(l>k) ll=l;
	if(ll>=1)
	{
		for(kk=1;kk<=ll;kk++)
		{
			if(kk<=k)
			{
				d=0.0;
				for(i=kk;i<=m;i++)
				{
					ix=(i-1)*n+kk-1;d=d+a[ix]*a[ix];
				}
				s[kk-1]=sqrt(d);
				//if(s[kk-1]!=0.0)
				if(fabs(s[kk-1])>MIN_DOUBLE)
				{
					ix=(kk-1)*n+kk-1;
					//if(a[ix]!=0.0)
					if(fabs(a[ix])>MIN_DOUBLE)
					{
						s[kk-1]=fabs(s[kk-1]);
						if(a[ix]<0.0) s[kk-1]=-s[kk-1];
					}
					for(i=kk;i<=m;i++)
					{
						iy=(i-1)*n+kk-1;
						a[iy]=a[iy]/s[kk-1];
					}
					a[ix]=1.0+a[ix];
				}
				s[kk-1]=-s[kk-1];
			}
			if(n>=kk+1)
			{
				for(j=kk+1;j<=n;j++)
				{
					//if((kk<=k)&&(s[kk-1]!=0.0))
					if((kk<=k)&&(fabs(s[kk-1])>MIN_DOUBLE))
					{
						d=0.0;
						for(i=kk;i<=m;i++)
						{
							ix=(i-1)*n+kk-1;
							iy=(i-1)*n+j-1;
							d=d+a[ix]*a[iy];
						}
						d=-d/a[(kk-1)*n+kk-1];
						for(i=kk;i<=m;i++)
						{
							ix=(i-1)*n+j-1;
							iy=(i-1)*n+kk-1;
							a[ix]=a[ix]+d*a[iy];
						}
					}
					e[j-1]=a[(kk-1)*n+j-1];
				}
			}
			if(kk<=k)
			{
				for(i=kk;i<=m;i++)
				{
					ix=(i-1)*m+kk-1;iy=(i-1)*n+kk-1;
					u[ix]=a[iy];
				}
			}
			if(kk<=l)
			{
				d=0.0;
				for(i=kk+1;i<=n;i++)
					d=d+e[i-1]*e[i-1];
				e[kk-1]=sqrt(d);
				//if(e[kk-1]!=0.0)
				if(fabs(e[kk-1])>MIN_DOUBLE)
				{
					//if(e[kk]!=0.0)
					if(fabs(e[kk])>MIN_DOUBLE)
					{
						e[kk-1]=fabs(e[kk-1]);
						if(e[kk]<0.0)
							e[kk-1]=-e[kk-1];
					}
					for(i=kk+1;i<=n;i++)
						e[i-1]=e[i-1]/e[kk-1];
					e[kk]=1.0+e[kk];
				}
				e[kk-1]=-e[kk-1];
				//if((kk+1<=m)&&(e[kk-1]!=0.0))
				if((kk+1<=m)&&(fabs(e[kk-1])>MIN_DOUBLE))
				{
					for(i=kk+1;i<=m;i++) w[i-1]=0.0;
					for(j=kk+1;j<=n;j++)
						for(i=kk+1;i<=m;i++)
							w[i-1]=w[i-1]+e[j-1]*a[(i-1)*n+j-1];
					for(j=kk+1;j<=n;j++)
						for(i=kk+1;i<=m;i++)
						{
							ix=(i-1)*n+j-1;
							a[ix]=a[ix]-w[i-1]*e[j-1]/e[kk];
						}
				}
				for(i=kk+1;i<=n;i++)
					v[(i-1)*n+kk-1]=e[i-1];
			}
		}
	}
	mm=n;
	if(m+1<n) mm=m+1;
	if(k<n) s[k]=a[k*n+k];
	if(m<mm) s[mm-1]=0.0;
	if(l+1<mm) e[l]=a[l*n+mm-1];
	e[mm-1]=0.0;
	nn=m;
	if(m>n) nn=n;
	if(nn>=k+1)
	{
		for(j=k+1;j<=nn;j++)
		{
			for(i=1;i<=m;i++)
				u[(i-1)*m+j-1]=0.0;
			u[(j-1)*m+j-1]=1.0;
		}
	}
	if(k>=1)/////////////////////////////////
	{
		for(ll=1;ll<=k;ll++)
		{
			kk=k-ll+1;iz=(kk-1)*m+kk-1;
			//if(s[kk-1]!=0.0)
			if(fabs(s[kk-1])>MIN_DOUBLE)
			{
				if(nn>=kk+1)
					for(j=kk+1;j<=nn;j++)
					{
						d=0.0;
						for(i=kk;i<=m;i++)
						{
							ix=(i-1)*m+kk-1;
							iy=(i-1)*m+j-1;
							d=d+u[ix]*u[iy]/u[iz];
						}
						d=-d;
						for(i=kk;i<=m;i++)
						{
							ix=(i-1)*m+j-1;
							iy=(i-1)*m+kk-1;
							u[ix]=u[ix]+d*u[iy];
						}
					}
					for(i=kk;i<=m;i++)
					{
						ix=(i-1)*m+kk-1;
						u[ix]=-u[ix];
					}
					u[iz]=1.0+u[iz];
					if(kk-1>=1)//////////////////////////////////////
						for(i=1;i<=kk-1;i++)
							u[(i-1)*m+kk-1]=0.0;
			}
			else
			{
				for(i=1;i<=m;i++)
					u[(i-1)*m+kk-1]=0.0;
				u[(kk-1)*m+kk-1]=1.0;
			}
		}
	}
	for(ll=1;ll<=n;ll++)
	{
		kk=n-ll+1;iz=kk*n+kk-1;
		//if((kk<=l)&&(e[kk-1]!=0.0))/////////////////////////////
		if((kk<=l)&&(fabs(e[kk-1])>MIN_DOUBLE))
		{
			for(j=kk+1;j<=n;j++)
			{
				d=0.0;
				for(i=kk+1;i<=n;i++)
				{
				ix=(i-1)*n+kk-1;iy=(i-1)*n+j-1;
				d=d+v[ix]*v[iy]/v[iz];
				}
				d=-d;
				for(i=kk+1;i<=n;i++)
				{
					ix=(i-1)*n+j-1;iy=(i-1)*n+kk-1;
					v[ix]=v[ix]+d*v[iy];
				}
			}
		}
		for(i=1;i<=n;i++)
			v[(i-1)*n+kk-1]=0.0;
		v[iz-n]=1.0;
	}
	for(i=1;i<=m;i++)
		for(j=1;j<=n;j++)
			a[(i-1)*n+j-1]=0.0;
	ml=mm;
	it=MAX_ITERA;
	while(1==1)//////////////////////////////////
	{
		if(mm==0)
		{
			ppp(a,e,s,v,m,n);
			free(s);free(e);free(w);
			return l;
		}
		if(it==0)
		{
			ppp(a,e,s,v,m,n);
			free(s);free(e);free(w);
			return -1;
		}
		kk=mm-1;
		//while((kk!=0)&&(fabs(e[kk-1])!=0.0))
		while((kk!=0)&&(fabs(e[kk-1])>MIN_DOUBLE))
		{
			d=fabs(s[kk-1])+fabs(s[kk]);
			dd=fabs(e[kk-1]);
			if(dd>eps*d)
				kk=kk-1;
			else
				e[kk-1]=0.0;
		}
		if(kk==mm-1)
		{
			kk=kk+1;
			if(s[kk-1]<0.0)
			{
				s[kk-1]=-s[kk-1];
				for(i=1;i<=n;i++)
				{
					ix=(i-1)*n+kk-1;
					v[ix]=-v[ix];
				}
			}
			while((kk!=ml)&&(s[kk-1]<s[kk]))
			{
				d=s[kk-1];s[kk-1]=s[kk];s[kk]=d;
				if(kk<n)
					for(i=1;i<=n;i++)
					{
						ix=(i-1)*n+kk-1;iy=(i-1)*n+kk;
						d=v[ix];v[ix]=v[iy];v[iy]=d;
					}
					if(kk<m)
						for(i=1;i<=m;i++)
						{
							ix=(i-1)*m+kk-1;
							iy=(i-1)*m+kk;
							d=u[ix];u[ix]=u[iy];u[iy]=d;
						}
						kk=kk+1;
			}
			it=MAX_ITERA;
			mm=mm-1;
		}
		else
		{
			ks=mm;
			//while((ks>kk)&&(fabs(s[ks-1])!=0.0))
			while((ks>kk)&&(fabs(s[ks-1])>MIN_DOUBLE))
			{
				d=0.0;
				if(ks!=mm)
					d=d+fabs(e[ks-1]);
				if(ks!=kk+1) d=d+fabs(e[ks-2]);
				dd=fabs(s[ks-1]);
				if(dd>eps*d)
					ks=ks-1;
				else
					s[ks-1]=0.0;
			}
			if(ks==kk)
			{
				kk=kk+1;
				d=fabs(s[mm-1]);
				t=fabs(s[mm-2]);
				if(t>d)
					d=t;
				t=fabs(e[mm-2]);
				if(t>d)
					d=t;
				t=fabs(s[kk-1]);
				if(t>d)
					d=t;
				t=fabs(e[kk-1]);
				if(t>d)
					d=t;
				sm=s[mm-1]/d;sml=s[mm-2]/d;
				eml=e[mm-2]/d;
				sk=s[kk-1]/d;ek=e[kk-1]/d;
				b=((sml+sm)*(sml-sm)+eml*eml)/2.0;
				c=sm*eml;c=c*c;shh=0.0;
				//if((b!=0.0)||(c!=0.0))
				if((fabs(b)>MIN_DOUBLE)||(fabs(c)>MIN_DOUBLE))
				{
					shh=sqrt(b*b+c);
					if(b<0.0)
						shh=-shh;
					shh=c/(b+shh);
				}
				fg[0]=(sk+sm)*(sk-sm)-shh;
				fg[1]=sk*ek;
				for(i=kk;i<=mm-1;i++)
				{
					sss(fg,cs);
					if(i!=kk)
						e[i-2]=fg[0];
					fg[0]=cs[0]*s[i-1]+cs[1]*e[i-1];
					e[i-1]=cs[0]*e[i-1]-cs[1]*s[i-1];
					fg[1]=cs[1]*s[i];
					s[i]=cs[0]*s[i];
					//if((cs[0]!=1.0)||(cs[1]!=0.0))
					if((fabs(cs[0]-1.0)>MIN_DOUBLE)||(fabs(cs[1])>MIN_DOUBLE))
						for(j=1;j<=n;j++)
						{
							ix=(j-1)*n+i-1;
							iy=(j-1)*n+i;
							d=cs[0]*v[ix]+cs[1]*v[iy];
							v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];
							v[ix]=d;
						}
					sss(fg,cs);
					s[i-1]=fg[0];
					fg[0]=cs[0]*e[i-1]+cs[1]*s[i];
					s[i]=-cs[1]*e[i-1]+cs[0]*s[i];
					fg[1]=cs[1]*e[i];
					e[i]=cs[0]*e[i];
					if(i<m)
						//if((cs[0]!=1.0)||(cs[1]!=0.0))
						if((fabs(cs[0]-1.0)>MIN_DOUBLE)||(fabs(cs[1])>MIN_DOUBLE))
							for(j=1;j<=m;j++)
							{
								ix=(j-1)*m+i-1;
								iy=(j-1)*m+i;
								d=cs[0]*u[ix]+cs[1]*u[iy];
								u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];
								u[ix]=d;
							}
				}
				e[mm-2]=fg[0];
				it=it-1;
			}
			else
			{
				if(ks==mm)
				{
					kk=kk+1;
					fg[1]=e[mm-2];e[mm-2]=0.0;
					for(ll=kk;ll<=mm-1;ll++)
					{
						i=mm+kk-ll-1;
						fg[0]=s[i-1];
						sss(fg,cs);
						s[i-1]=fg[0];
						if(i!=kk)
						{
							fg[1]=-cs[1]*e[i-2];
							e[i-2]=cs[0]*e[i-2];
						}
						//if((cs[0]!=1.0)||(cs[1]!=0.0))
						if((fabs(cs[0]-1.0)>MIN_DOUBLE)||(fabs(cs[1])>MIN_DOUBLE))
							for(j=1;j<=n;j++)
							{
								ix=(j-1)*n+i-1;
								iy=(j-1)*n+mm-1;
								d=cs[0]*v[ix]+cs[1]*v[iy];
								v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];
								v[ix]=d;
							}
					}
				}
				else
				{
					kk=ks+1;
					fg[1]=e[kk-2];
					e[kk-2]=0.0;
					for(i=kk;i<=mm;i++)
					{
						fg[0]=s[i-1];
						sss(fg,cs);
						s[i-1]=fg[0];
						fg[1]=-cs[1]*e[i-1];
						e[i-1]=cs[0]*e[i-1];
						//if((cs[0]!=1.0)||(cs[1]!=0.0))
						if((fabs(cs[0]-1.0)>MIN_DOUBLE)||(fabs(cs[1])>MIN_DOUBLE))
							for(j=1;j<=m;j++)
							{
								ix=(j-1)*m+i-1;
								iy=(j-1)*m+kk-2;
								d=cs[0]*u[ix]+cs[1]*u[iy];								
								u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];
								u[ix]=d;
							}
					}
				}
			}
			}
			}
			free(s);free(e);free(w);
			return l;
}
void ppp(float a[],float e[],float s[],float v[],int m,int n)
{
	int i,j,p,q;
	float d;
	if(m>=n)
		i=n;
	else
		i=m;
	for(j=1;j<=i-1;j++)
	{
		a[(j-1)*n+j-1]=s[j-1];
		a[(j-1)*n+j]=e[j-1];
	}
	a[(i-1)*n+i-1]=s[i-1];
	if(m<n)
		a[(i-1)*n+i]=e[i-1];
	for(i=1;i<=n-1;i++)
	for(j=i+1;j<=n;j++)
	{
		p=(i-1)*n+j-1;
		q=(j-1)*n+i-1;
		d=v[p];v[p]=v[q];v[q]=d;
	}
	return;
}
void sss(float fg[2],float cs[2])
{
	float r,d;
	//if((fabs(fg[0])+fabs(fg[1]))==0.0)
	if((fabs(fg[0])+fabs(fg[1]))<MIN_DOUBLE)
	{
		cs[0]=1.0;cs[1]=0.0;d=0.0;
	}
	else
	{
		d=sqrt(fg[0]*fg[0]+fg[1]*fg[1]);
		if(fabs(fg[0])>fabs(fg[1]))
		{
			d=fabs(d);
			if(fg[0]<0.0)
				d=-d;
		}
		if(fabs(fg[1])>=fabs(fg[0]))
		{
			d=fabs(d);
			if(fg[1]<0.0)
				d=-d;
		}
		cs[0]=fg[0]/d;
		cs[1]=fg[1]/d;
	}
	r=1.0;
	if(fabs(fg[0])>fabs(fg[1]))
		r=cs[1];
	else
		//if(cs[0]!=0.0)
		if(fabs(cs[0])>MIN_DOUBLE)
			r=1.0/cs[0];
	fg[0]=d;
	fg[1]=r;
	return;
}
//矩阵相乘
void damul(float *a,float *b,int m,int n,int k,float *c)
{
	int i,j,l,u;
	for(i=0;i<=m-1;i++)
		for(j=0;j<=k-1;j++)
		{
			u=i*k+j;
			c[u]=0;
			for(l=0;l<=n-1;l++)
				c[u]=c[u]+a[i*n+l]*b[l*k+j];
		}
	return;
}


int inverse(float *A,int m,float *B) {
	int i,j,k;
	float *M;
	float temp=0,b=0;
	int m_2=2*m;

	M=(float *)malloc(m*m*2*sizeof(float));

	for(i=0;i<m;i++) {
		for(j=0;j<m_2;j++) {
			if(j<m) {
				M[i*m_2+j]=A[i*m+j];
			}
			else {
				if(i==j-m) {
					M[i*m_2+j]=1;
				}
				else {
					M[i*m_2+j]=0;
				}
			}
		}
	}

	for(k=0;k<m-1;k++) {
		if(M[k*m_2+k]==0) {
			for(i=k+1;i<m;i++) {
				if(M[i*m_2+k]!=0) {
					break;
				}
			}
			if(i>=m) {
				return -1;
			}
			else {
				for(j=0;j<m_2;j++) {
					temp=M[k*m_2+j];
					M[k*m_2+j]=M[(k+1)*m_2+j];
					M[(k+1)*m_2+j]=temp;
				}
			}
		}
		
		for(i=k+1;i<m;i++) {
			b=M[i*m_2+k]/M[k*m_2+k];
			for(j=0;j<m_2;j++) {
				temp=M[i*m_2+j]-b*M[k*m_2+j];
				M[i*m_2+j]=temp;
			}
		}
	}

	for(k=m-1;k>0;k--) {
		if(M[k*m_2+k]==0) {
			for(i=k+1;i<m;i++) {
				if(M[i*m_2+k]!=0) {
					break;
				}
			}
			if(i>=m) {
				return -1;
			}
			else {
				for(j=0;j<m_2;j++) {
					temp=M[k*m_2+j];
					M[k*m_2+j]=M[(k+1)*m_2+j];
					M[(k+1)*m_2+j]=temp;
				}
			}
		}

		for(i=k-1;i>=0;i--) {
			b=M[i*m_2+k]/M[k*m_2+k];
			for(j=0;j<m_2;j++) {
				temp=M[i*m_2+j]-b*M[k*m_2+j];
				M[i*m_2+j]=temp;
			}
		}
	}

	for(i=0;i<m;i++) {
		if(M[i*m_2+i]!=1) {
			b=1/M[i*m_2+i];
			for(j=0;j<m_2;j++) {
				temp=M[i*m_2+j]*b;
				M[i*m_2+j]=temp;
			}
		}
	}

	for(i=0;i<m;i++) {
		for(j=0;j<m;j++) {
			B[i*m+j]=M[i*m_2+j+m];
		}
	}

	free(M);
	return 1;

}

//A矩阵转置得到A_t，m、n为A矩阵的行列数。
void transpose(float *A,int m,int n,float *A_t) {
	int i,j;

	for(i=0;i<m;i++) {
		for(j=0;j<n;j++) {
			A_t[j*m+i]=A[i*n+j];
		}
	}
}