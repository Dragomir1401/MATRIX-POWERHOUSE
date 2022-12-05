#include <stdio.h>
#include <stdlib.h>
#define ERROR "No matrix with the given index\n"//defines ERROR OF index
//defines error of size in multiplication
#define CANT_MULTIPLY "Cannot perform matrix multiplication\n"
#define LIMIT 10007//defines limit of elements in multiplication and
//limit of sum of elements in sorting algorithm
void print_matrix(int **p, int rows, int cols)
{
	//simple function for printing a matrix
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			printf("%d ", p[i][j]);
		printf("\n");
	}
}

void free_matrix_global(int **p, int rows)
{
	//simple function for freeing a matrix
	for (int i = 0; i < rows; i++)
		free(p[i]);

	free(p);
	//it works by freeing each line then the matrix itself
}

int **load_memory(int n, int m)
{
	//simple function to alloc memory for a matrix with given dim
	int **a = malloc(n * sizeof(int *));
	//allocs for a n lines of memory
	if (!a)
		return NULL;
	//defensive approach
	for (int i = 0; i < n; i++) {
		a[i] = malloc(m * sizeof(int));
		if (!a) {//allocs for each line m integers
			free_matrix_global(a, i);//if it cant alloc a given element
			return NULL;//it fress all elements until that one
		}
	}

	return a;
}

void read_matrix(int **a, int n, int m)
{
	//simple function for reading a matrix
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			scanf("%d", &a[i][j]);
}

void alloc_function(int ****arr_mat, int number, int **dim)
{
	//function to alloc a new size of matrix in the end of the array
	int ***tmp = realloc(*arr_mat, (number + 1) * sizeof(int **));
	if (tmp)//realloc makes the size of the array bigger with an int **
		*arr_mat = tmp;
	else//defensive approach
		printf("Cannot alloc memory\n");
	int size = 2 * (number + 1);//we alloc memory by doubling the size
	int *dim_tmp = realloc(*dim, 2 * size * sizeof(int));
	if (dim_tmp)//dim doubles its size because of its nature
		*dim = dim_tmp;//of retaining lines and cols
	else//defensive approach
		printf("Cant alloc memory\n");
}

void multiply_matrices(int ****arr_mat, int **dim,
					   int index, int index1, int *number)
{
	//naive approach of multiply mat by modifying at the address of the array
	if ((*dim)[2 * index + 1] != (*dim)[2 * index1]) {
		fprintf(stdout, CANT_MULTIPLY);//error of size of mat corespondence
		return;
	}
	(*number)++;//creates a spot in the end of array of mat
	(*dim)[2 * (*number)] = (*dim)[2 * index];//creates dim
	(*dim)[2 * (*number) + 1] = (*dim)[2 * index1 + 1];
	alloc_function(arr_mat, *number, dim);//allocs mem for it
	(*arr_mat)[(*number)] =
	load_memory((*dim)[2 * index], (*dim)[2 * index1 + 1]);
	//creates a memory spot for the result of the multiplication
	for (int i = 0; i < (*dim)[2 * index]; i++)
		for (int j = 0; j < (*dim)[2 * index1 + 1]; j++) {
			(*arr_mat)[(*number)][i][j] = 0;//initialization with 0
			//performs multiplication element by element
			for (int k = 0; k < (*dim)[2 * index + 1]; k++) {
				(*arr_mat)[(*number)][i][j] +=
				((*arr_mat)[index][i][k] *
				(*arr_mat)[index1][k][j]) % LIMIT;//mods with the lim
				(*arr_mat)[(*number)][i][j] %= LIMIT;
			}
			if ((*arr_mat)[(*number)][i][j] < 0)
				(*arr_mat)[(*number)][i][j] += LIMIT;
			//if the element is negative we positive it in range (LIMIT, LIMIT)
		}
}

int **transpose_matrix(int **p, int rows, int cols)
{
	//simple function which return a copy of a given matrix transposed
	int **copy;
	copy = load_memory(cols, rows);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			copy[j][i] = p[i][j];
	return copy;
}

void transpose_function(int ***arr_mat, int *dim, int index)
{
	int rows = dim[2 * index];//determines dim of the given matrix
	int cols = dim[2 * index + 1];
	int **tmp = transpose_matrix(arr_mat[index], rows, cols);
	//tmp = temporary matrix
	free_matrix_global(arr_mat[index], rows);//free spot of the matrix
	arr_mat[index] = tmp;//puts the new matrix in that memory place
	int aux = dim[2 * index];//swaps nr of cols with nr of lines
	dim[2 * index] = dim[2 * index + 1];
	dim[2 * index + 1] = aux;
}

void free_function(int ****arr_mat, int **dim,
				   int number, int index)
{
	//function to free a matrix in the array and move the rest over it
	int rows = (*dim)[2 * index];//determines rows from dim array
	free_matrix_global((*arr_mat)[index], rows);
	//free the spot in the matrix given by the readen index
	for (int i = index; i < number; i++) {//shifts the rest of the pointers
	//by moving each pointer to the right and forgetting the freed spot
		(*arr_mat)[i] = (*arr_mat)[i + 1];
		(*dim)[2 * i] = (*dim)[2 * (i + 1)];//shifts dim
		(*dim)[2 * i + 1] = (*dim)[2 * (i + 1) + 1];//to the right
	}
}

int sum_matrix_elements(int **a, int n, int m)
{
	//simple function to determine sum of matrix elements with a limit of 10^4+7
	int sum = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			sum += a[i][j];
			sum %= LIMIT;
		}
	return sum;
}

void sort_matrices(int ****arr_mat, int **dim, int number)
{
	//function to sort the array of matrices in memory based on sum of elements
	for (int i = 0; i < number; i++)
		for (int j = i + 1; j <= number; j++) {
			//we proceed to determine each matrix sum of elements and also
			//limiting it to 10 ^ 4 + 7
			int sum1 = (sum_matrix_elements((*arr_mat)[i],
											(*dim)[2 * i],
											(*dim)[2 * i + 1])) % LIMIT;
			int sum2 = (sum_matrix_elements((*arr_mat)[j],
											(*dim)[2 * j],
											(*dim)[2 * j + 1])) % LIMIT;
			//if the sums are < 0 we add 10 ^ 4 + 7;
			if (sum1 < 0)
				sum1 += LIMIT;
			if (sum2 < 0)
				sum2 += LIMIT;
			if (sum1 > sum2) {
				//we perform swap of pointers to the matrices which dont respect
				//ascendent order of sums
				int **tmp;
				tmp = (*arr_mat)[i];
				(*arr_mat)[i] = (*arr_mat)[j];
				(*arr_mat)[j] = tmp;
				//we also swap nr of lines and nr of cols in dim array
				int tmp1;
				tmp1 = (*dim)[2 * i];
				(*dim)[2 * i] = (*dim)[2 * j];
				(*dim)[2 * j] = tmp1;

				tmp1 = (*dim)[2 * i + 1];
				(*dim)[2 * i + 1] = (*dim)[2 * j + 1];
				(*dim)[2 * j + 1] = tmp1;
			}
		}
}

void resize_matrix(int ****arr_mat,
				   int **dim, int index, int number)
{
	//function to resize matrix and put it over the matrix we need to resize
	int *lines, *columns, nr_lines, nr_columns, **copy;
	//we read the rules for lines and columns which we are going to use for
	//picking elements from the previous matrix and then putting them in a copy
	scanf("%d", &nr_lines);
	lines = malloc(nr_lines * sizeof(int));

	for (int i = 0; i < nr_lines; i++)
		scanf("%d", &lines[i]);

	scanf("%d", &nr_columns);
	columns = malloc(nr_columns * sizeof(int));

	for (int i = 0; i < nr_columns; i++)
		scanf("%d", &columns[i]);

	if (index > number || index < 0) {//if the index is not in list then error
		fprintf(stdout, ERROR);
		free(lines);
		free(columns);
		return;//exit function
	}

	copy = load_memory(nr_lines, nr_columns);
	//we alloc memory for the copy then proceed to create copy based on
	//restrictions given
	for (int i = 0; i < nr_lines; i++)
		for (int j = 0; j < nr_columns; j++)
			copy[i][j] = (*arr_mat)[index][lines[i]][columns[j]];
	//free lines and columns arrays previously allocated dynamic
	free(lines);
	free(columns);
	//free the matrix with the given index
	free_matrix_global((*arr_mat)[index], (*dim)[2 * index]);
	(*arr_mat)[index] = copy;//put the copy over it
	(*dim)[2 * index] = nr_lines;//modify dim array to match
	(*dim)[2 * index + 1] = nr_columns;
}

int **add(int **M1, int **M2, int n)
{
	//simple function which returns a allocated dynamic matrix as a sum of 2
	//given matrices
	int **temp = load_memory(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			temp[i][j] = (M1[i][j] + M2[i][j]) % LIMIT;

	return temp;
}

int **subtract(int **M1, int **M2, int n)
{
	//simple function which returns a allocated dynamic matrix as a difference
	//of 2 given matrices
	int **temp = load_memory(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			temp[i][j] = (M1[i][j] - M2[i][j]) % LIMIT;
	return temp;
}

//complex algorithm for matrix multiplication made by Strassen
//it is based on divide and conquer by splitting a big matrix in
//sections of 4 and then treating them like elements and performing
//in the last step a simple formula multiplciation
//quite memory heavy but in the end it frees all the resources used:)
int **strassen_multiply(int **A, int **B, int n)
{
	if (n == 1) {
		int **C = load_memory(1, 1);
		C[0][0] = A[0][0] * B[0][0];
		return C;
	}
	int **C = load_memory(n, n);
	int k = n / 2;
	int **A11 = load_memory(k, k); int **A12 = load_memory(k, k);
	int **A21 = load_memory(k, k); int **A22 = load_memory(k, k);
	int **B11 = load_memory(k, k); int **B12 = load_memory(k, k);
	int **B21 = load_memory(k, k); int **B22 = load_memory(k, k);
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++) {
			A11[i][j] = A[i][j]; A12[i][j] = A[i][k + j];
			A21[i][j] = A[k + i][j]; A22[i][j] = A[k + i][k + j];
			B11[i][j] = B[i][j]; B12[i][j] = B[i][k + j];
			B21[i][j] = B[k + i][j]; B22[i][j] = B[k + i][k + j];
		}
	int **to_free1 = subtract(B12, B22, k); int **to_free2 = add(A11, A12, k);
	int **to_free3 = add(A21, A22, k); int **to_free4 = subtract(B21, B11, k);
	int **to_free5 = add(A11, A22, k); int **to_free6 = add(B11, B22, k);
	int **to_free7 = subtract(A12, A22, k); int **to_free8 = add(B21, B22, k);
	int **to_free9 = subtract(A11, A21, k); int **to_free10 = add(B11, B12, k);

	int **P1 = strassen_multiply(A11, to_free1, k);
	int **P2 = strassen_multiply(to_free2, B22, k);
	int **P3 = strassen_multiply(to_free3, B11, k);
	int **P4 = strassen_multiply(A22, to_free4, k);
	int **P5 = strassen_multiply(to_free5, to_free6, k);
	int **P6 = strassen_multiply(to_free7, to_free8, k);
	int **P7 = strassen_multiply(to_free9, to_free10, k);
	free_matrix_global(to_free1, k); free_matrix_global(to_free2, k);
	free_matrix_global(to_free3, k); free_matrix_global(to_free4, k);
	free_matrix_global(to_free5, k); free_matrix_global(to_free6, k);
	free_matrix_global(to_free7, k); free_matrix_global(to_free8, k);
	free_matrix_global(to_free9, k); free_matrix_global(to_free10, k);
	int **to_free11 = add(P5, P4, k);
	int **to_free12 = add(to_free11, P6, k);
	int **to_free13 = add(P5, P1, k);
	int **to_free14 = subtract(to_free13, P3, k);
	int **C11 = subtract(to_free12, P2, k);
	int **C12 = add(P1, P2, k);
	int **C21 = add(P3, P4, k);
	int **C22 = subtract(to_free14, P7, k);

	free_matrix_global(to_free11, k); free_matrix_global(to_free12, k);
	free_matrix_global(to_free13, k); free_matrix_global(to_free14, k);

	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++) {
			//we put results in a matrix C and limit the elements to
			//10 ^ 4 + 7
			C[i][j] = C11[i][j] % LIMIT;
			C[i][j + k] = C12[i][j] % LIMIT;
			C[k + i][j] = C21[i][j] % LIMIT;
			C[k + i][k + j] = C22[i][j] % LIMIT;
			//if we get negative elements we add 10 ^ 4 + 7
			if (C[i][j] < 0)
				C[i][j] += LIMIT;
			if (C[i][j + k] < 0)
				C[i][j + k] += LIMIT;
			if (C[k + i][j] < 0)
				C[k + i][j] += LIMIT;
			if (C[k + i][k + j] < 0)
				C[k + i][k + j] += LIMIT;
		}

	free_matrix_global(A11, k); free_matrix_global(A12, k);
	free_matrix_global(A21, k); free_matrix_global(A22, k);
	free_matrix_global(B11, k); free_matrix_global(B12, k);
	free_matrix_global(B21, k); free_matrix_global(B22, k);
	free_matrix_global(P1, k); free_matrix_global(P2, k);
	free_matrix_global(P3, k); free_matrix_global(P4, k);
	free_matrix_global(P5, k); free_matrix_global(P6, k);
	free_matrix_global(P7, k); free_matrix_global(C11, k);
	free_matrix_global(C12, k); free_matrix_global(C21, k);
	free_matrix_global(C22, k);

	return C;
}

void multiply_matrices_strassen(int ****arr_mat, int **dim,
								int index, int index1, int *number)
{	//function to multiply with Strassen and put result at end of array
	if ((*dim)[2 * index + 1] != (*dim)[2 * index1]) {
		fprintf(stdout, CANT_MULTIPLY);
		return;
	}
	(*number)++;
	(*dim)[2 * (*number)] = (*dim)[2 * index];
	(*dim)[2 * (*number) + 1] = (*dim)[2 * index1 + 1];
	alloc_function(arr_mat, *number, dim);

	(*arr_mat)[(*number)] =
	strassen_multiply((*arr_mat)[index],
					  (*arr_mat)[index1], (*dim)[2 * index]);
}

void free_all_resources(int ***arr_mat, int number,
						int *dim)
{	//functions to free all matrix used and the arrays
	for (int i = 0; i <= number; i++)
		free_matrix_global(arr_mat[i], dim[2 * i]);
	free(arr_mat);
	free(dim);
}
