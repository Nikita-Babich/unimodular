#include <stdio.h>
#include <math.h>
//#include <string.h>
#include <stdlib.h>
#include <time.h>

#define nl printf("\n")
#define ELEM(mat,i,j) (mat->elem)[i * (mat->cols) + j]

//#define DEBUG_MODE 
//#define FILEWORK

/* TODO
	save and extract
*/

typedef struct{ //for matrices
	unsigned int rows;
	unsigned int cols;
	float* elem;
}MAT;

typedef struct{ //for working with row and column indices and optimising row swaps
	unsigned int len;
	unsigned int values[];
}ARRAY;

void print_array(ARRAY* array){
	nl;
	for(int i=0; i< (array->len); i++){
		printf(" %d ", array->values[i]);
	}
	nl;
	return;
}

float rand_nonzero_float(void){ //used at creation and at row operations
	//float result = fmod((float)rand(),10.0) * (rand()%2 ? 1 : -1);	
	//limited by 10 to prevent small number representation problem in division for the last element in mat_create_triangular_det1
	float result = (float)rand()/(float)(RAND_MAX/10.0); //from 0 to a
	if( result==0 ){ 
		result = 1; 
	}
	return result; 
}

ARRAY* shuffle(ARRAY* array){ 
//in place, because there is no case it would not be used
//and "not shuffled array of indices" is trivial and meaningless 0 1 2 ..
	int j,m,n, buffer;
	#ifdef DEBUG_MODE
		printf("\n __reached start of shuffle \n ");
		print_array(array);
	#endif
	if((array->len) > 1){
		for(int i = 0; i < (array->len)-1; i++){
			//j = i+1 + rand()%(array->len-1 - i); //my attempt
			//j = i + rand() / (RAND_MAX / (array->len - i) + 1); //ok google
			//both don't work 
			m = rand()%(array->len);
			n = rand()%(array->len);
			buffer = array->values[m];
			array->values[m] = array->values[n];
			array->values[n] = buffer;
		}
	}
	#ifdef DEBUG_MODE //not reached
		print_array(array);
	#endif 
	return array;
}

ARRAY* create_growing_array(unsigned int max){ //5 -> 0 1 2 3 4
	//for saving indices and reshuffling rows and columns
	ARRAY* ptr = (ARRAY*)malloc( sizeof(unsigned int) + sizeof(unsigned int)*max );
	if(ptr==NULL){
		return NULL;
	}
	for( int i = 0; i<max; i++){
		ptr->values[i]=i;
	}
	ptr->len = max;
	#ifdef DEBUG_MODE
		printf("__first and last elements of index array %d, %d ",ptr->values[0], ptr->values[max-1]);
	#endif 
	return ptr;
}

MAT* mat_create_with_type(unsigned int rows, unsigned int cols){ 
	//Can ptr still be NULL after typecasting?
	float* array_ptr = (float*)malloc( sizeof(float)*cols*rows );
	if( array_ptr == NULL) {
		return NULL;
	} //do not create a struct if not enough space for the array
	MAT* ptr = (MAT*)malloc( sizeof(unsigned int)*2 + sizeof(int) );
	if(ptr==NULL){
		free(array_ptr);
		return NULL;
	}
	ptr->rows = rows;
	ptr->cols = cols;
	ptr->elem = array_ptr;
	return ptr;
}

MAT* mat_copy(MAT* mat){ //
	MAT* ptr = mat_create_with_type(mat->rows, mat->cols);
	ptr->rows = mat->rows;
	ptr->cols = mat->cols;
	for( int i = 0; i<ptr->rows; i++){
		for( int j = 0; j<ptr->cols; j++){
			ELEM(ptr,i,j) = ELEM(mat,i,j);
		}
	}
	return ptr;
}

MAT* mat_add_row_to_row(MAT* origin, int index_modifier, int index_place){
	// index_modifier - index of the row, which, multiplied by a number, is used to add to row with index "index_place"
	float mod = rand_nonzero_float();
	if(index_modifier >= index_place){
		return NULL; 
	}
	for(int i=0; i< origin->cols; i++){
		ELEM(origin, index_place, i) = ELEM(origin, index_place, i) + mod*ELEM(origin, index_modifier, i);
		#ifdef DEBUG_MODE
			printf("__%f ",ELEM(origin, index_place, i));
		#endif 
	}
	return origin;
}

MAT* mat_row_operations(MAT* origin){ //modify in place, because mat_add_row_to_row is also in place
/*
	1 2 3
	4* 5 6
	7* 8* 9   *=to base row addition on.
	Because upper triangular is
	1 2 3
	0 5 6
	0 0 9, the indices also happen to describe the needed rows to pass to mat_add_row_to_row(), in this case 2,1  2,0  1,0
*/
	if(origin->cols != origin->rows){
		return NULL;
	}
	//for each * (zero in the given matrix) lower triangle
	for(int i = origin->rows-1; i>0; i--){ //row to mod
		for(int j = 0; j<i ; j++){ //row to mod by
			//start a row operation
			mat_add_row_to_row(origin, j, i);
		}
	}
	return origin;
}

MAT* mat_shuffle(MAT* origin){ //
	//return new matrix with rows and cols swaps
	int rows = origin->rows;
	int cols = origin->cols;
	ARRAY* row_indices = shuffle(create_growing_array(rows));
	ARRAY* col_indices = shuffle(create_growing_array(cols));
	MAT* result = mat_copy(origin);
	if(row_indices==NULL or col_indices==NULL or result==NULL){
		return NULL;
	}
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			ELEM(result,i,j) = ELEM(origin, (row_indices->values[i]), (col_indices->values[j]) );
		}
	}
	return result;
}

void mat_destroy(MAT* mat){ //
	free(mat->elem);
	free(mat);
	return;
}

void mat_wipe(MAT* mat){
	//wipe out memory if the data was sensitive
	/*
		example of sensitive data: matrix describing orientation of a satellite
		It can be recognised and recovered from byte-array due to trigonometric properties.
		For example, it's det=1 no matter orientation.
	*/
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
			ELEM(mat,i,j) = rand_nonzero_float(); // (float)rand(); //-old version, imprecise
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

MAT* mat_create_triangular_det1(unsigned int rows, unsigned int cols){
	if(rows!=cols){ printf("error"); return NULL; }
	MAT* ptr = mat_create_with_type(rows, cols);
	if(ptr==NULL){
		return NULL;
	}
	ptr = rand_above_diag(ptr);
	ptr = zeros_under_diag(ptr);
	float accumulated_value = 1;
	float new_rand = 1;
	for(int i=0; i < ptr->rows-1; i++){	
		new_rand = rand_nonzero_float();
		accumulated_value *= new_rand;
		#ifdef DEBUG_MODE
			printf(" %f | %f   ##  \n",accumulated_value, 1.0/accumulated_value);
		#endif
		ELEM(ptr,i,i) = new_rand;
	}
	ELEM(ptr, ((ptr->rows)-1), ((ptr->cols)-1)) = 1.0/fabs(accumulated_value); 
	#ifdef DEBUG_MODE
		printf(" %f | %f   ##  \n",accumulated_value, 1.0/fabs(accumulated_value)); 
	#endif
	return ptr;
}

void mat_print(MAT* mat){ //format applicable for Wolfram Alpha
	if (mat==NULL){
		printf("\n The matrix is already deleted or was never successfully created");
	}else{
		printf("\n Matrix at 0x%.8X with sizes %d, %d", mat, mat->rows, mat->cols);
		nl;
		printf("{\n");
		for(int i=0; i < mat->rows; i++){
			printf("{");
			for(int j=0; j < mat->cols; j++){
				printf("%3.8f", ELEM(mat,i,j));
				if (j != mat->cols - 1){
					printf(", ");
				}
			}
			printf("}");
			if (i != mat->rows - 1){
				printf(",");
			}
			 nl;
		}
		printf("}");
		nl;
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

#ifdef FILEWORK
char mat_save(MAT* mat){
	FILE* outfile;
	outfile = fopen("matrix.bin", "wb");
	if (outfile == NULL) {
        printf("\nError with opening file\n");
        return 0;
    }
	int result1 = fwrite(&(mat->rows), sizeof(unsigned int), 1, outfile);
	int result2 = fwrite(&(mat->cols), sizeof(unsigned int), 1, outfile);
	int result3 = fwrite(mat->elem, sizeof(float), (mat->cols) * (mat->rows), outfile);
	fclose(outfile);
	return (char)(result1*result2*result3);
}

MAT* mat_create_by_file(){
	FILE* infile;
	infile = fopen("matrix.bin", "wb+");
	
	MAT* ptr = (MAT*)malloc( sizeof(MAT) );
	if(ptr==NULL){
		return NULL;
	}
	
	fread(&(ptr->rows), sizeof(unsigned int), 1, infile);
	fread(&(ptr->cols), sizeof(unsigned int), 1, infile);
	printf("!!! %d, %d ", ptr->rows, ptr->cols);
	
	float* array_ptr = (float*)malloc( sizeof(float) * ptr->cols * ptr->rows );
	if( array_ptr == NULL) {
		free(ptr);
		return NULL;
	} //do not create a struct if not enough space for the array
	
	ptr->elem = array_ptr;
	
	fread(array_ptr, sizeof(float), ptr->cols * ptr->rows, infile);
	
	fclose(infile);
	
	return(ptr);
}
#endif

int main(){
	srand(time(NULL));
	
	printf("\n Testing creation and random fill \n");
	MAT* a = mat_create_with_type(4, 4);
	mat_random(a);
	mat_print(a);
	mat_destroy(a);
	
	printf("\n Testing creation of uneven dimensions \n");
	MAT* b = mat_create_with_type(3, 7);
	mat_random(b);
	mat_print(b);
	
	printf("\n Testing copy \n");
	MAT* c = mat_copy(b);
	mat_print(c);
	mat_destroy(c);
	mat_destroy(b);
	
	printf("\n Testing creation with det=+-1 \n");
	MAT* d = mat_create_triangular_det1(4,4);
	mat_print(d);
	mat_destroy(d);
	
	printf("\n Testing smaller \n");
	MAT* e = mat_create_triangular_det1(3,3);
	mat_print(e);
	
	printf("\n Testing row operations \n");
	MAT* f = mat_row_operations(e); 
	mat_print(f);
	
	printf("\n Testing shuffle \n");
	MAT* g = mat_shuffle(f); 
	mat_print(g);
	
	#ifdef FILEWORK
	printf("\n Saving to file and extracting \n");
	mat_save(g);
	MAT* recovered = mat_create_by_file();
	mat_print(recovered);
	#endif
	//det values due to rounding -1.00003 0.999999 0.999992 0.999984 1. -0.999841
}
