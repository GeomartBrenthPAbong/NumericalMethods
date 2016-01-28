#include "matrix_functions.h"

Matrix* copy_matrix(Matrix *p_matrix){
	Matrix *new_matrix = mtx_create_matrix(mtx_get_num_rows(p_matrix), mtx_get_num_columns(p_matrix));
	
	int i, j;
	for(i=0; i < mtx_get_num_rows(p_matrix); i++)
		for(j=0; j < mtx_get_num_columns(p_matrix); j++)
			new_matrix->m_cells[i][j] = p_matrix->m_cells[i][j];
	
	return new_matrix;
}

Matrix* augment_matrices(Matrix *p_left_matrix, Matrix *p_right_matrix){
	if(mtx_get_num_rows(p_left_matrix) != mtx_get_num_rows(p_right_matrix))
		return 0;
	
	int new_num_columns = *(p_left_matrix->m_columns) + *(p_right_matrix->m_columns);
	int i, j;
	
	Matrix *augmented_matrix = mtx_create_matrix(*(p_left_matrix->m_rows), new_num_columns);

	for(i=0; i < mtx_get_num_rows(p_left_matrix); i++)
		for(j=0; j < new_num_columns; j++)
			augmented_matrix->m_cells[i][j] = (j < mtx_get_num_columns(p_left_matrix))?
											  mtx_get(p_left_matrix, i, j) : 
											  mtx_get(p_right_matrix, i, j - mtx_get_num_columns(p_left_matrix));
	
	return augmented_matrix;
}

Matrix* identity_matrix(int p_rows, int p_columns){
	Matrix *identity_matrix = mtx_create_matrix(p_rows, p_columns);
	
	int i, j;
	
	for(i=0; i < p_rows; i++)
		for(j=0; j < p_columns; j++)
			mtx_set(identity_matrix, i, j, (i == j));
	
	return identity_matrix;
}
