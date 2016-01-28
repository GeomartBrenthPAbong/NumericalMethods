#include "matrix.h"

#ifndef MATRIX_FUNCTIONS_H
#define MATRIX_FUNCTIONS_H

Matrix* copy_matrix(Matrix *p_matrix);
Matrix* augment_matrices(Matrix *p_left_matrix, Matrix *p_right_matrix);
Matrix* identity_matrix(int p_rows, int p_columns);

#endif