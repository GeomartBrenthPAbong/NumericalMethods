#include <stdio.h>
#include "matrix.h"
#include "sles_functions.h"
#include "sles_matrix_methods.h"

void introscreen();
void get_inputs_and_solve(int*);
void get_iter_error(int*, int*);
void display_iteration(Matrix*, double *, int*);
int valid_choice(int*);

int main(){
    interface(introscreen, valid_choice, get_inputs_and_solve);
    return 0;
}

void get_inputs_and_solve(int* p_sle_solver){
    Matrix *matrix, *initial_guess, *solution_matrix, *roots;
    int rows, columns, single_column = 1, iter, error, error_code = 0;

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
    
    printf("\nEnter unknown matrix initial guesses: \n");
    initial_guess = get_input_matrix(&rows, &single_column, 0, 0, 'X');
    display_matrix(initial_guess, "\nInitial Unknown Matrix X: \n");
    
    get_iter_error(&iter, &error);
    
    roots = iterative_method(p_sle_solver, matrix, initial_guess, 
                             solution_matrix, &iter, &error, display_iteration, &error_code);
    
    if(0 == roots && error_code == 1)
        printf("Division by zero.\n");
    else{
        printf("The unknowns of the system of equations are:\n\n");
        display_roots(roots, 'x');
        mtx_delete_matrix(roots);
    }
    
    mtx_delete_matrix(matrix);
    mtx_delete_matrix(solution_matrix);
    mtx_delete_matrix(initial_guess);
}

void get_iter_error(int *p_iter, int* p_error){
    printf("\nEnter the Criterion Error <0-100>: ");
    scanf("%d", p_error);
    printf("Enter the number of maximum iteration: ");
    scanf("%d", p_iter);
}

void introscreen(){
    printf("\nWELCOME! This program will solve simultaneous");
    printf("\nlinear equations (matrices) using Iterative");
    printf("\nMethods. Please select the method you want to use:\n");
    printf("\n1. Gauss-Seidel Method");
    printf("\n2. Jacobi Iteration Method\n");
    printf("\nOption No.> ");
}

void display_iteration(Matrix *p_xs, double *p_ea, int* p_cur_iter){
    int i;
    for(i=0; i < mtx_get_num_rows(p_xs); i++)
        printf("%5d %5d %13lf %13lf\n", *p_cur_iter + 1, i + 1, mtx_get(p_xs, i, 0), p_ea[i]);
    printf("\n");
}

int valid_choice(int *p_choice){
    return (1 == *p_choice || 2 == *p_choice);
}