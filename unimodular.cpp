#include <stdio.h>
#include <math.h>
//#include <string.h>
#include <stdlib.h>
#include <time.h>

#define nl printf("\n")
#define ELEM(mat,i,j) (mat->elem)[i * (mat->cols) + j]

#define DEBUG_MODE

typedef struct{
	unsigned int rows;
	unsigned int cols;
	float* elem;
}MAT;

typedef struct{
	unsigned int len;
	unsigned int* values;
}INDICES;

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

MAT* mat_copy(MAT* mat){ 
	float* array_ptr = (float*)malloc( sizeof(float) * mat->cols * mat->rows );
	if( array_ptr == NULL) {return NULL;} //do not create a struct if not enough space for the array
	MAT* ptr = (MAT*)malloc( sizeof(unsigned int)*2 + sizeof(int) );
	ptr->rows = mat->rows;
	ptr->cols = mat->cols;
	ptr->elem = mat->elem;
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
	mat->rows = 0;
	mat->cols = 0;
	free(mat);
	mat = NULL;
	return;
}

void mat_unit(MAT* mat){
	for(int i=0; i < mat->rows; i++){	
		for(int j=0; j < mat->cols; j++){
			if(i!=j){
				ELEM(mat,i,j) = 0;
			}else{
				ELEM(mat,i,j) = 1;
			}
		}
	}
	return;
}

MAT* rand_above_diag(MAT* mat){
	for(int i=0; i < mat->rows; i++){	
		for(int j=i+1; j < mat->cols; j++){
			ELEM(mat,i,j) = (float)rand();
		}
	}
	return mat;
}

MAT* zeros_under_diag(MAT* mat){
	for(int i=0; i < mat->rows; i++){	
		for(int j=0; j < i; j++){
			ELEM(mat,i,j) = 0;
		}
	}
	return mat;
}

void mat_random(MAT* mat){
	int length = mat->rows * mat->cols;
	for(int i=0; i < length; i++){	
		mat->elem[i] =  ((float)rand()) / (float)RAND_MAX * 2 - 1; //between -1 and 1
	}
	return;
}

float rand_nonzero_float(void){
	float result = (float)rand();
	if( result==0 or result == NULL ){ result = 1; }
	return result;
}

MAT* mat_create_triangular_det1(unsigned int rows, unsigned int cols){
	if(rows!=cols){ printf("error"); return NULL; }
	MAT* ptr = mat_create_with_type(rows, cols);
	ptr = rand_above_diag(ptr);
	ptr = zeros_under_diag(ptr);
	float accumulated_value = 1;
	float new_rand = 1;
	for(int i=0; i < ptr->rows-1; i++){	
		new_rand = rand_nonzero_float();
		accumulated_value *= new_rand;
		ELEM(ptr,i,i) = new_rand;
	}
	ELEM(ptr, ptr->rows-1, ptr->rows-1) = 1/fabs(accumulated_value);
	return ptr;
}

void mat_print(MAT* mat){
	if (mat==NULL){
		printf("\n The matrix is already deleted or was never successfully created");
	}else{
		printf("\n Matrix with sizes %d, %d", mat->rows, mat->cols);
		nl;
		for(int i=0; i < mat->rows; i++){
			for(int j=0; j < mat->cols; j++){
				printf("\t%+10.2f", ELEM(mat,i,j));
			}
		nl;
		}
	}
}



float diag_det(MAT* mat){ //also works for triangular
	int size = mat->rows;
	float answer = 1;
	for(int i = 0; i<size; i++){
		answer *= ELEM(mat,i,i);
	}
	return answer;
}


//float det(MAT* mat){ //may be not needed
	//must recieve square matrix
	//row reduction algorithm, to upper triangular
	//int size = mat->rows;
	//compare each string with 0th (
	//compare each string from i=2 with i=1
	//
	//for(int j=0; j<size-1; j++){
		//ELEM(mat,i,j)
	//}
	//return diag_det(mat);
//}

//MAT* mat_row_operations(){
//	
//}

//MAT* mat_rows_shuffle(){
//	
//}

int main(){
	srand(time(NULL));
	
	MAT* a = mat_create_with_type(4, 4);
	mat_random(a);
	mat_print(a);
	mat_destroy(a);
	
	MAT* b = mat_create_with_type(3, 7);
	mat_random(b);
	mat_print(b);
	
	
	MAT* c = mat_copy(b);
	mat_print(c);
	mat_destroy(c);
	mat_destroy(b);
	
	MAT* d = mat_create_triangular_det1(4,4);
	mat_print(d);
	mat_destroy(d);
		
}
