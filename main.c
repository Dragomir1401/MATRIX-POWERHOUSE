#include <stdio.h>
#include <stdlib.h>
#include "functions_matrix.h"
#include <string.h>
#define ERROR "No matrix with the given index\n"//defines ERROR
int main(void)
{
	int ***arr_mat, number = -1, index, *dim, index1;
	char control = 'A'; int increment = -1;//increment is for dim array
	dim = malloc(2 * sizeof(int)); arr_mat = malloc(sizeof(int **));
	while (control != 'Q') {
		int done = 0; scanf("%c", &control);//done->tracking unspecified command
		if (control != 'O' && control != 'L' && control != 'Q')
			scanf("%d", &index);
		if (control == 'L') {//load command
			number++; int n, m; scanf("%d%d", &n, &m);//++nr of matrices
			dim[++increment] = n; dim[++increment] = m;//mat sizes
			alloc_function(&arr_mat, number, &dim);
			arr_mat[number] = load_memory(n, m);//new spot and alloc
			read_matrix(arr_mat[number], n, m); done = 1;
		} else if (control == 'D') {
			if (index > number || index < 0)
				fprintf(stdout, ERROR);//if index is out of list print ERROR
			else//prints the dim from the array previously created
				printf("%d %d\n", dim[2 * index],
					   dim[2 * index + 1]); done = 1;
		} else if (control == 'P') {
			if (index > number || index < 0) {
				fprintf(stdout, ERROR);
			} else {//calculates nr of rows and cols from dim array
				int rows = dim[2 * index]; int cols = dim[2 * index + 1];
				print_matrix(arr_mat[index], rows, cols); done = 1;
			}
		} else if (control == 'T') {//performs transpose
			if (index > number || index < 0)
				fprintf(stdout, ERROR);
			else
				transpose_function(arr_mat, dim, index);
			done = 1;
		} else if (control == 'F') {
			if (index > number || index < 0) {
				fprintf(stdout, ERROR);
			} else {
				if (number != index) {
					free_function(&arr_mat, &dim,
								  number, index); number--;
				} else {
					int rows = dim[2 * index];
					free_matrix_global(arr_mat[index], rows);
					increment -= 2; number--;
				}
			} done = 1;
		} else if (control == 'M') {//matrix multiplication (naive)
			scanf("%d", &index1);
			if (index > number || index < 0) {
				fprintf(stdout, ERROR);
			} else {
				if (index1 > number || index1 < 0)
					fprintf(stdout, ERROR);
				else
					multiply_matrices(&arr_mat, &dim,
									  index, index1, &number);
			} done = 1;
		} else if (control == 'O') {//sorting the matrices
			sort_matrices(&arr_mat, &dim, number);
		} else if (control == 'C') {//resize based on given input
			resize_matrix(&arr_mat, &dim, index, number);
		} else if (control == 'S') {//Strassen algorithm for multiplication
			scanf("%d", &index1);
			if (index > number || index < 0) {
				fprintf(stdout, ERROR);
			} else {
				if (index1 > number || index1 < 0)
					fprintf(stdout, ERROR);
				else
					multiply_matrices_strassen(&arr_mat,
											   &dim, index,
											   index1, &number);
			} done = 1;
		} else if (!done && control != '\n' && control != 'Q') {//if we didnt
			printf("Unrecognized command\n");//make a command or we reach '\n'
		}
	}

	free_all_resources(arr_mat, number, dim);//frees array
	return 0;
}
