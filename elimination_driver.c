#include <stdio.h>
#include "matrix.h"
#include "sles_functions.h"
#include "sles_matrix_methods.h"

void introscreen();
void get_inputs_and_solve(int*);
int valid_choice(int*);
void elimination_method(Matrix* p_coeff_matrix, Matrix* p_sol_matrix, int *p_emethod);

int main(){
    interface(introscreen, valid_choice, get_inputs_and_solve);
    return 0;
}

void get_inputs_and_solve(int* p_sle_solver){
    Matrix *matrix, *solution_matrix, *roots;
    int rows, columns, iter, error = 0, single_column = 1;

    get_row_col(&rows, &columns,
                "Enter no. of rows of coefficient matrix: ",
                "Enter no. of columns of coefficient matrix: ",
                "Error! Coefficient matrix is not a square matrix. Try again.\n",
                is_square_matrix);
    
    printf("\n");
    matrix = get_input_matrix(&rows, &columns, 1, 1, 'A');
    display_matrix(matrix, "Coefficient Matrix A: \n");
    
    printf("\nEnter solution matrix values: \n");
    solution_matrix = get_input_matrix(&rows, &single_column, 0, 0, 'C');
    display_matrix(solution_matrix, "\nSolution Matrix C: \n");
	
    elimination_method(matrix, solution_matrix, p_sle_solver);
    
    mtx_delete_matrix(matrix);
    mtx_delete_matrix(solution_matrix);
}

void introscreen(){
    printf("\nWELCOME! This program will solve simultaneous");
    printf("\nlinear equations (matrices) using Elimination");
    printf("\nMethods. Please select the method you want to use:\n");
    printf("\n1. Naive Gaussian Elimination Method");
    printf("\n2. Gauss-Jordan Elimination Method");
    printf("\n3. LU Decomposition Elimination Method\n");
    printf("\nOption No.> ");
}

int valid_choice(int *p_choice){
    return (1 == *p_choice || 2 == *p_choice || 3 == *p_choice);
}

void elimination_method(Matrix* p_coeff_matrix, Matrix* p_sol_matrix, int *p_emethod){
    Matrix *roots;
    int error = 0;
    
    if(1 == *p_emethod)
        roots = naive_gaussian(p_coeff_matrix, p_sol_matrix, display_matrix, &error);
    else if(2 == *p_emethod)
        roots = gauss_jordan(p_coeff_matrix, p_sol_matrix, display_matrix, &error);
    else
        roots = lu_decomposition(p_coeff_matrix, p_sol_matrix, display_matrix, &error);
    
    if(error && 1 == error)
        printf("\nDivision By Zero.\n\n");
    else{
        printf("\nThe unknowns of the system of equations are:\n\n");
        display_roots(roots, 'x');
        
        mtx_delete_matrix(roots);
    }
}
