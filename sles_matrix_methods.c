#include "sles_matrix_methods.h"

void sum_of_prod_of_coeff_guess(double* p_sum, double* p_matrix_row, 
                                Matrix* p_xs, int* p_except_index){
    int i=0;
    
    *p_sum = 0;
    for(i; i < mtx_get_num_rows(p_xs); i++){
        if(i == *p_except_index)
            continue;
        
        *p_sum += p_matrix_row[i] * mtx_get(p_xs, i, 0);
    }
}

Matrix* iterative_method(int* p_imethod, Matrix *p_coeff_matrix, Matrix *p_initial_guess, 
                         Matrix *p_solution_matrix, int *p_iter, int *p_error,
                         void (*p_iteration_displayer)(Matrix*, double*, int*), int *p_error_code){
    int current_iteration = 0, index, error_met,
        num_roots = mtx_get_num_rows(p_coeff_matrix);
	double old_val, sum_of_cols, *ea, new_x_value, divisor;
    float error = *p_error;
    Matrix *cur_xs, 
           *xs = copy_matrix(p_initial_guess);
    
    ea = malloc(sizeof(double) * num_roots);
    
    int i=0;
    for(i; i < num_roots; i++)
        ea[i] = 100;
    
    printf("\n\n%5s %5s %13s %13s\n", "iter", "i", "xi", "Ea");
    
    do{
        if(1 == *p_imethod) // GAUSS SEIDEL
            cur_xs = xs;
        else // JACOBI
            cur_xs = copy_matrix(xs);
        
        for(index=0; index < num_roots; index++){
            if(ea[index] <= *p_error)
                continue;
            
            old_val = mtx_get(xs, index, 0);
            sum_of_prod_of_coeff_guess(&sum_of_cols, mtx_get_row(p_coeff_matrix, index), cur_xs, &index);
            divisor = mtx_get(p_coeff_matrix, index, index);
            
            if(0 == divisor){
                *p_error_code = 1;
                break;
            }
            
            new_x_value = (mtx_get(p_solution_matrix, index, 0) - sum_of_cols) / divisor;
            mtx_set(xs, index, 0, new_x_value);
            ea[index] = fabs(old_val - mtx_get(xs, index, 0)) * 100 / mtx_get(xs, index, 0);
        }
        
        
        if(2 == *p_imethod)
            mtx_delete_matrix(cur_xs);
        
        if(*p_error_code)
            break;
        
        error_met = 1;
        
        for(i=0; i < num_roots; i++)
            if(ea[i] > error){
                error_met = 0;
                break;
            }
       
       
       p_iteration_displayer(xs, ea, &current_iteration);
    }while(++current_iteration < *p_iter && error_met == 0);
    
    // Free ea
    free(ea);
    ea = 0;
    
    return (*p_error_code)? 0 : xs;
}

Matrix* to_triangular_matrix(char p_side, Matrix *p_matrix, int *p_error, 
                             void (*p_iteration_displayer)(Matrix*, char*),
                             Matrix *p_lower_composition, char *p_desc, int p_counter){
    if('U' != p_side && 'L' != p_side)
        return 0;
    
    Matrix* triangular_matrix = copy_matrix(p_matrix);
    
    int pivot_row, pivot_col,
        counter = p_counter,
        num_rows = mtx_get_num_rows(p_matrix),
        num_columns = mtx_get_num_columns(p_matrix),
        target_row = num_rows - 2;
 
    double multiplier = 0;
    
    pivot_row = ('U' == p_side)? 0 : num_rows-1;
    
    while(1){
        if(('U' == p_side && pivot_row >= num_rows - 1) || ('L' == p_side && pivot_row <= 0))
            break;
        
        target_row = ('U' == p_side)?pivot_row + 1: pivot_row - 1;
        
        if(0 != p_desc){
            char buffer[50];
            sprintf(buffer, "\n%s %d:\n", p_desc, counter++);
            
            printf("\n%s\n", buffer);
        }
        
        while(1){
            if(('U' == p_side && target_row >= num_rows) || ('L' == p_side && target_row < 0))
                break;
            
            if(0 == mtx_get(triangular_matrix, pivot_row, pivot_row)){
                *p_error = 1;
                break;
            }
            
            multiplier = mtx_get(triangular_matrix, target_row, pivot_row) / mtx_get(triangular_matrix, pivot_row, pivot_row);
            
            
            for(pivot_col=0; pivot_col < num_columns; pivot_col++)
                triangular_matrix->m_cells[target_row][pivot_col] = mtx_get(triangular_matrix, target_row, pivot_col) - multiplier * mtx_get(triangular_matrix, pivot_row, pivot_col);
            
            mtx_set(triangular_matrix, target_row, pivot_row, 0);
            
            if(0 != p_lower_composition)
                mtx_set(p_lower_composition, target_row, pivot_row, multiplier);
            
            if(0 != p_desc)
                printf("Row %d <-- (Row %d) - (%lf * Row %d)\n", target_row + 1, target_row + 1, multiplier, pivot_row + 1);
           
            target_row = ('U' == p_side)? target_row + 1 : target_row - 1;
        }
        
        if(0 != p_desc)
            p_iteration_displayer(triangular_matrix, "\n");
        
        if(*p_error){
            mtx_delete_matrix(triangular_matrix);
            break;
        }
    
        pivot_row = ('U' == p_side)? pivot_row + 1 : pivot_row - 1;
    }
    
    return (*p_error)? 0 : triangular_matrix;
}

Matrix* normalize_matrix(Matrix* p_matrix, void (*p_iteration_displayer)(Matrix*, char*), char *p_desc){
    int i,j,
        num_rows = mtx_get_num_rows(p_matrix),
        num_columns = mtx_get_num_columns(p_matrix);
    
    double divisor;
    Matrix *normalized_matrix = copy_matrix(p_matrix);
    
    for(i=0; i < num_rows; i++){
        divisor = mtx_get(normalized_matrix, i, i);
        
        if(1 != divisor && 0 != divisor){
            for(j=0; j < num_columns; j++)
                normalized_matrix->m_cells[i][j] = normalized_matrix->m_cells[i][j] / divisor;
            
            if(0 != p_desc){
                char buffer[50];
                sprintf(buffer, "\n%s %d:\n", p_desc, i + 1);
                printf("%s", buffer);
                printf("\nRow %d <-- (Row %d) / %lf\n", i + 1, i + 1, divisor);
                
                p_iteration_displayer(normalized_matrix, "\n");
            }
        }
    }
    
    return normalized_matrix;
}

Matrix* substitution(char p_side, Matrix* p_matrix, int *p_error){
    if('U' != p_side && 'L' != p_side)
        return 0;

    int num_rows = mtx_get_num_rows(p_matrix),
        num_columns = mtx_get_num_columns(p_matrix);
     
    double sum_of_prod;
    Matrix* roots = mtx_create_matrix(num_rows, 1);
    
    int i,j;
    i = ('U' == p_side)? num_rows - 1 : 0;
    
    while(1){
        if(('U' == p_side && i < 0) || ('L' == p_side && i >=  num_rows))
            break;
        
        *p_error = 0;
        
        if(0 == mtx_get(p_matrix, i, i)){
            *p_error = 1;
            mtx_delete_matrix(roots);
            break;
        }
        
        sum_of_prod = 0;
        
        j = ('U' == p_side)? num_columns - 2 : 0;
        
        while(1){
            if(('U' == p_side && j <= i) || ('L' == p_side && j >= i))
                break;
            
            sum_of_prod += mtx_get(p_matrix, i, j) * mtx_get(roots, j, 0);
            
            j = ('U' == p_side)? j - 1: j + 1;
        }
        
        roots->m_cells[i][0] = (mtx_get(p_matrix, i, num_columns - 1) - sum_of_prod) / mtx_get(p_matrix, i, i);
        
        i = ('U' == p_side)? i - 1: i + 1;
    }
    
    return (*p_error)? 0 : roots;
}

Matrix* extract_jacobi_roots(Matrix* p_matrix){
    int i, num_rows = mtx_get_num_rows(p_matrix),
        num_columns = mtx_get_num_columns(p_matrix);
    
    Matrix *roots = mtx_create_matrix(num_rows, 1);
    
    for(i=0; i < num_rows; i++)
        roots->m_cells[i][0] = mtx_get(p_matrix, i, num_columns - 1);
    
    return roots;
}

Matrix* naive_gaussian(Matrix* p_coeff_matrix, Matrix* p_sol_matrix,
                       void (*p_iteration_displayer)(Matrix*, char*), int *p_error){
    Matrix *augmented_matrix, *upper_matrix, *roots;
    
    augmented_matrix = augment_matrices(p_coeff_matrix, p_sol_matrix);
    p_iteration_displayer(augmented_matrix, "\nAugmented Matrix: \n");
    
    upper_matrix = to_triangular_matrix('U', augmented_matrix, p_error, 
                                        p_iteration_displayer, 0, "Forward Elimination No.", 1);
    
    mtx_delete_matrix(augmented_matrix);
    
    if(0 == upper_matrix)
        return 0;
    
    roots = substitution('U', upper_matrix, p_error);
    
    mtx_delete_matrix(upper_matrix);
    
    return roots;
}

Matrix* gauss_jordan(Matrix* p_coeff_matrix, Matrix* p_sol_matrix,
                     void (*p_iteration_displayer)(Matrix*, char*), int *p_error){
    Matrix *augmented_matrix, *upper_matrix, 
           *normalized_matrix, *lower_matrix, 
           *roots;
    
    augmented_matrix = augment_matrices(p_coeff_matrix, p_sol_matrix);
    p_iteration_displayer(augmented_matrix, "\nAugmented Matrix: \n");
    
    upper_matrix = to_triangular_matrix('U', augmented_matrix, p_error, 
                                        p_iteration_displayer, 0, "Forward Elimination No.", 1);
    
    mtx_delete_matrix(augmented_matrix);
    
    if(0 == upper_matrix)
        return 0;
    
    normalized_matrix = normalize_matrix(upper_matrix, p_iteration_displayer, "Normalize Row");
    lower_matrix = to_triangular_matrix('L', normalized_matrix, p_error, 
                                        p_iteration_displayer, 0, "Backward Elimination No.", 1);
    
    mtx_delete_matrix(normalized_matrix);
    mtx_delete_matrix(upper_matrix);
    
    if(0 == lower_matrix)
        return 0;
    
    roots = extract_jacobi_roots(lower_matrix);
    
    mtx_delete_matrix(lower_matrix);
    
    return roots;
}

Matrix* lu_decomposition(Matrix* p_coeff_matrix, Matrix* p_sol_matrix,
                         void (*p_iteration_displayer)(Matrix*, char*), int *p_error){
    Matrix *upper_matrix, *lower_matrix, 
           *lc, *z, *ux, *roots;
        
    lower_matrix = identity_matrix(mtx_get_num_rows(p_coeff_matrix), mtx_get_num_columns(p_coeff_matrix));
    
    upper_matrix = to_triangular_matrix('U', p_coeff_matrix, p_error, 
                   p_iteration_displayer, lower_matrix, "Forward Elimination No.", 1);
    
    if(0 == upper_matrix){
        mtx_delete_matrix(lower_matrix);
        return 0;
    }
    
    p_iteration_displayer(upper_matrix, "\nUpper Triangular Matrix U:\n");
    p_iteration_displayer(lower_matrix, "\nLower Triangular Matrix L:\n");
    
    lc = augment_matrices(lower_matrix, p_sol_matrix);
    
    p_iteration_displayer(lc, "\nAugmented L-Triangular Matrix and Solution Matrix:\n");
    
    z = substitution('L', lc, p_error);
    
    p_iteration_displayer(z, "\nSolutions to Z Matrix (Forward Substitution):\n");
    
    mtx_delete_matrix(lc);
    mtx_delete_matrix(lower_matrix);
    
    if(0 == z){
        mtx_delete_matrix(upper_matrix);
        return 0;
    }
    
    ux = augment_matrices(upper_matrix, z);
    
    p_iteration_displayer(ux, "\nAugmented U-Triangular Matrix and Z Matrix:\n");
    
    roots = substitution('U', ux, p_error);
    
    mtx_delete_matrix(upper_matrix);
    mtx_delete_matrix(z);
    mtx_delete_matrix(ux);
    
    return roots;
}