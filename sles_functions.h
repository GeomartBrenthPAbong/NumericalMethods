#include<stdio.h>
#include "matrix.h"

#ifndef SLES_FUNCTIONS_H
#define SLES_FUNCTIONS_H

void display_matrix(Matrix *p_matrix, char *p_desc);
void display_roots(Matrix *p_roots, char p_root_name);
void interface(void(*p_show_choices)(), int (*p_choice_validator)(int*), void(*p_server)(int*));

Matrix* get_input_matrix(int *p_rows, int *p_columns, 
						 int p_show_before, int p_show_after, 
						 char p_matrix_name);

void get_row_col(int *p_row, int *p_col, 
				 char *p_row_disp, char *p_col_disp,
				 char *p_err_string, int (*p_checker)(int *, int *));

int is_square_matrix(int *p_row, int *p_col);

#endif