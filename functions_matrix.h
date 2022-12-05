void alloc_function(int ****arr_mat, int number, int **dim);
void free_matrix_global(int **p, int rows);
int **load_memory(int n, int m);
void print_matrix(int **p, int rows, int cols);
int **transpose_matrix(int **p, int rows, int cols);
void transpose_function(int ***arr_mat, int *dim, int index);
void free_function(int ****arr_mat, int **dim, int number,
				   int index);
void multiply_matrices(int ****arr_mat, int **dim, int index,
					   int index1, int *number);
void read_matrix(int **a, int n, int m);
void free_matrix_local(int n, int **a);
void sort_matrices(int ****arr_mat, int **dim, int number);
void resize_matrix(int ****arr_mat, int **dim, int index,
				   int number);
void multiply_matrices_strassen(int ****arr_mat, int **dim,
								int index, int index1, int *number);
void free_all_resources(int ***arr_mat, int number, int *dim);
