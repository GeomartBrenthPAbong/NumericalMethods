#include "sles_functions.h"

void display_matrix(Matrix *p_matrix, char *p_desc){
	int i, j;
	
	if(0 != p_desc)
		printf(p_desc);
	
	for(i=0; i < mtx_get_num_rows(p_matrix); i++){
        for(j=0; j < mtx_get_num_columns(p_matrix); j++)
            printf("%13lf", mtx_get(p_matrix, i, j));
        printf("\n");
    }
}

void display_roots(Matrix *p_roots, char p_root_name){
	int i;
    
	for(i=0; i < mtx_get_num_rows(p_roots); i++)
		printf("%c%d = %lf\n", p_root_name, i + 1, mtx_get(p_roots, i, 0));
}

void interface(void(*p_show_choices)(), int (*p_choice_validator)(int*), void(*p_server)(int*)){
    int choice, right_choice;
    char want_to_continue;
    
    do{
        // Get systems of linear equation solver
        do{
            right_choice = 1;
            p_show_choices();
            scanf("%d", &choice);
            
            printf("\n");
            if(!p_choice_validator(&choice)){
                printf("Invalid option.\n");
                right_choice = 0;
            }
        }while(!right_choice);

        // Get matrix and solve

		p_server(&choice);

        printf("\nWould you like to continue? 'Y' for yes and any character for no.\n");
        getchar();
        scanf("%c", &want_to_continue);

    }while('Y' == want_to_continue);
    
    printf("Thank you for using this program.");
}

Matrix* get_input_matrix(int *p_rows, int *p_columns, 
                         int p_show_before, int p_show_after, 
                         char p_matrix_name){
	int i, j;
	
    Matrix *matrix = mtx_create_matrix(*p_rows, *p_columns);
    
	for(i=0; i < *p_rows; i++){
        if(p_show_before)
            printf("Enter row %d values:\n", i + 1);
        
        for(j=0; j < *p_columns; j++){
            printf("\t%c(%d,%d) --> ",p_matrix_name, i, j);
            scanf("%lf", &(matrix->m_cells[i][j]));
        }
        
        if(p_show_after)
            printf("\n");
    }
    
    return matrix;
}

void get_row_col(int *p_row, int *p_col, 
				 char *p_row_disp, char *p_col_disp,
				 char *p_err_string, int (*p_checker)(int *, int *)){
	do{
		printf(p_row_disp);
		scanf("%d", p_row);
		
		printf(p_col_disp);
		scanf("%d", p_col);
		
		if(p_checker(p_row, p_col))
			break;
		else
			printf(p_err_string);
	}while(1);
}

inline int is_square_matrix(int *p_row, int *p_col){
	return *p_row > 0 && *p_col > 0 && *p_row == *p_col;
}