#include "Malloc.h"

#define malloc_(type,n) (type *)malloc((n)*sizeof(type))

float** Malloc_float(int row,int column) {
	int i,j;
	float **array;
	
	array=malloc_(float *,row);  
    for(i=0;i<row;i++)  
    {  
        array[i]=malloc_(float,column);  
    }  
    //validation  
    for(i=0;i<row;i++)  
    {  
        for(j=0;j<column;j++)  
        {  
            array[i][j]=0;  
        }   
    }
	return array;
	
}

int** Malloc_int(int row,int column) {
	int i,j;
	int **array;
	
	array=malloc_(int *,row);  
    for(i=0;i<row;i++)  
    {  
        array[i]=malloc_(int,column);  
    }  
    //validation  
    for(i=0;i<row;i++)  
    {  
        for(j=0;j<column;j++)  
        {  
            array[i][j]=0;  
        }   
    }
	return array;
	
}