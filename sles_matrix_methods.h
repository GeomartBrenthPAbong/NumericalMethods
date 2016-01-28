#include<stdio.h>
#include<math.h>
#include "matrix.h"
#include "matrix_functions.h"

#ifndef SLES_MATRIX_METHODS_H
#define SLES_MATRIX_METHODS_H

Matrix* iterative_method(int* p_imethod, Matrix *p_coeff_matrix, Matrix *p_initial_guess, 
                         Matrix *p_solution_matrix, int *p_iter, int *p_error,
                         void (*p_iteration_displayer)(Matrix*, double*, int*), int *p_error_code);

Matrix* to_triangular_matrix(char p_side, Matrix *p_matrix, int *p_error, 
                             void (*p_iteration_displayer)(Matrix*, char*),
                             Matrix *p_lower_composition, char *p_desc, int p_counter);

Matrix* normalize_matrix(Matrix* p_matrix, void (*p_iteration_displayer)(Matrix*, char*), char *p_desc);

Matrix* substitution(char p_side, Matrix* p_matrix, int *p_error);

Matrix* naive_gaussian(Matrix* p_coeff_matrix, Matrix* p_sol_matrix,
                       void (*p_iteration_displayer)(Matrix*, char*), int *p_error);

Matrix* gauss_jordan(Matrix* p_coeff_matrix, Matrix* p_sol_matrix,
                     void (*p_iteration_displayer)(Matrix*, char*), int *p_error);
                          
Matrix* lu_decomposition(Matrix* p_coeff_matrix, Matrix* p_sol_matrix,
                         void (*p_iteration_displayer)(Matrix*, char*), int *p_error);

#endif