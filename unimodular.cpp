#include <stdio.h>
//#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define nl printf("\n")
#define ELEM(mat,i,j) (mat->elem)[i * (mat->cols) + j]

typedef struct{
	unsigned int rows;
	unsigned int cols;
	float* elem;
}MAT;

MAT* mat_create_with_type(unsigned int rows, unsigned int cols){
	//Can ptr still be NULL after typecasting?
	float* array_ptr = (float*)malloc( sizeof(float)*cols*rows );
	if( array_ptr == NULL) {return NULL;} //do not create a struct if not enough space for the array
	MAT* ptr = (MAT*)malloc( sizeof(unsigned int)*2 + sizeof(int) );
	ptr->rows = rows;
	ptr->cols = cols;
	ptr->elem = array_ptr;
	return ptr;
}

//MAT* mat_create_by_file(char* filename){
//	
//}

//char mat_save(MAT* mat, char* filename){
//	
//}

void mat_destroy(MAT* mat){
	free(mat->elem);
	mat->elem = NULL;
	free(mat);
	mat = NULL;
	return;
}

void mat_wipe(MAT* mat){
	//wipe out memory if the data was sensitive
	for(int i=0; i< mat->cols * mat->rows; i++){
		mat->elem[i] = 0;
	}
	free(mat->elem);
	mat->elem = NULL;
	free(mat);
	mat = NULL;
	return;
}

void mat_unit(MAT* mat){
	for(int i=0; i < mat->rows; i++){	
		for(int j=0; j < mat->cols; j++){
			if(i==j){
				ELEM(mat,i,j) = 0;
			}else{
				ELEM(mat,i,j) = 1;
			}
		}
	}
	return;
}

void mat_random(MAT* mat){
	int length = mat->rows * mat->cols;
	//printf("%d",length);
	for(int i=0; i < length; i++){	
		mat->elem[i] = (float)(rand()/(float)RAND_MAX*2-1);
	}
	return;
}

void mat_print(MAT* mat){
	printf("\n Matrix with sizes %d, %d", mat->rows, mat->cols);
	nl;
	for(int i=0; i < mat->rows; i++){
		for(int j=0; j < mat->cols; j++){
			printf("\t%+2.2f", ELEM(mat,i,j));
		}
		nl;
	}
}

int main(){
	srand(time(NULL));
	
	MAT* a = mat_create_with_type(4, 4);
	//mat_random(a);
	mat_print(a);
	mat_destroy(a);
	
	MAT* b = mat_create_with_type(3, 7);
	mat_random(b);
	mat_print(b);
	mat_destroy(b);
	
		
}

