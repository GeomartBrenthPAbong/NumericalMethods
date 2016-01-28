#include<stdlib.h>

#ifndef MATRIX_H
#define MATRIX_H

typedef struct Matrix{
	int *m_rows;
	int *m_columns;
	double** m_cells;
} Matrix;

Matrix* mtx_create_matrix(int p_rows, int p_columns);
void mtx_delete_matrix(Matrix *p_matrix);

void mtx_set(Matrix *p_matrix, int p_row, int p_column, double p_value);
double mtx_get(Matrix *p_matrix, int p_row, int p_column);
double* mtx_get_row(Matrix *p_matrix, int p_row);

int mtx_cell_exists(Matrix *p_matrix, int p_row, int p_column);

int mtx_get_num_rows(Matrix *p_matrix);
int mtx_get_num_columns(Matrix *p_matrix);

#endif