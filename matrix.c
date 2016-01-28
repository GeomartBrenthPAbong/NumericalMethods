#include "matrix.h"

Matrix* mtx_create_matrix(int p_rows, int p_columns){
	if(p_rows <= 0 && p_columns <= 0)
		return 0;
	
	Matrix *matrix = malloc(sizeof(Matrix));
	
	matrix->m_rows = malloc(sizeof(int));
	matrix->m_columns = malloc(sizeof(int));
	
	*(matrix->m_rows) = p_rows;
	*(matrix->m_columns) = p_columns;
	
	matrix->m_cells = malloc(sizeof(double*) * p_rows);
	
	int i = 0;
	for(i; i < p_rows; i++)
		matrix->m_cells[i] = malloc(sizeof(double) * p_columns);
	
	return matrix;
}

void mtx_delete_matrix(Matrix *p_matrix){
	if(0 == p_matrix)
		return;
	
	int i = 0;
	for(i; i < *(p_matrix->m_rows); i++)
		free(p_matrix->m_cells[i]);
	
	free(p_matrix->m_cells);
	free(p_matrix->m_rows);
	free(p_matrix->m_columns);
	
	p_matrix->m_cells = 0;
	p_matrix->m_rows = 0;
	p_matrix->m_columns = 0;
	
	free(p_matrix);
	
	p_matrix = 0;
}

void mtx_set(Matrix *p_matrix, int p_row, int p_column, double p_value){
	if(0 == p_matrix || !mtx_cell_exists(p_matrix, p_row, p_column))
		return;
	
	p_matrix->m_cells[p_row][p_column] = p_value;
}

double mtx_get(Matrix *p_matrix, int p_row, int p_column){
	if(0 == p_matrix || !mtx_cell_exists(p_matrix, p_row, p_column))
		return 0;
	
	return p_matrix->m_cells[p_row][p_column];
}

double* mtx_get_row(Matrix *p_matrix, int p_row){
	if(p_row < 0 || p_row >= *(p_matrix->m_rows))
		return 0;
	
	return p_matrix->m_cells[p_row];
}

inline int mtx_cell_exists(Matrix *p_matrix, int p_row, int p_column){
	return p_row >= 0 && p_row < *(p_matrix->m_rows) && 
		   p_column >= 0 && p_column < *(p_matrix->m_columns);
}

inline int mtx_get_num_rows(Matrix *p_matrix){
	return *(p_matrix->m_rows);
}

inline int mtx_get_num_columns(Matrix *p_matrix){
	return *(p_matrix->m_columns);
}
