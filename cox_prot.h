#include "coxeter_inc.h"

/* automaton_binary_tree.c */
extern int init_binary_tree_element(BINARY_TREE_ELEMENT **);
extern void free_binary_tree_element(BINARY_TREE_ELEMENT *);
extern int add_state_to_binary_tree(BINARY_TREE_ELEMENT **, AUTOMATON_STATE *, AUTOMATON_STATE **, int);
/* automaton_graph.c */
extern int create_state(int, AUTOMATON_STATE **);
extern void free_state(AUTOMATON_STATE *);
extern void free_state_tree(BINARY_TREE_ELEMENT *);
extern int compare_states(AUTOMATON_STATE *, AUTOMATON_STATE *, int);
extern bool state_in_tree(AUTOMATON_STATE *, AUTOMATON_STATE *, AUTOMATON_STATE **, int);
extern int generate_state_tree(MATRIX_DATA *, int, ROOT_TABLE *, AUTOMATON_STATE **, BINARY_TREE_ELEMENT **);
extern int generate_next_automaton_state(MATRIX_DATA *, int, ROOT_TABLE *, AUTOMATON_STATE *, AUTOMATON_STATE *, BINARY_TREE_ELEMENT **, int);
/* cox_action.c */
extern double cox_scalar_product(MATRIX_DATA *, int, int);
extern double cox_scalar_product_root(MATRIX_DATA *, int, ROOT *, int);
extern int fill_scalar_product_matrix(MATRIX_DATA *, int);
extern double cox_action(MATRIX_DATA *, int, int, int);
extern int fill_cox_action_matrix(MATRIX_DATA *, int);
extern int cox_action_on_root(MATRIX_DATA *, int, int, ROOT *, ROOT **, ROOT_TABLE *, _Bool *);
extern int cox_action_on_root_list(ROOT_TABLE *, ROOT_TABLE **, int, int, MATRIX_DATA *);
/* file_input_output_matrix.c */
extern int load_matrix_from_file(char *, long, long, long ***, MATRIX_FILE_INFO **);
extern void free_file_info(MATRIX_FILE_INFO *);
/* main.c */
extern bool is_symmetric(long **, int);
extern bool is_reduced(AUTOMATON_STATE *, char *, int *, int, int);
extern int init_matrix_data(MATRIX_DATA **, int);
extern void free_matrix_data(MATRIX_DATA *, int);
extern int main(void);
/* root_table.c */
extern int init_root(int, ROOT **);
extern void free_root(ROOT *);
extern int init_root_table_element(ROOT_TABLE_ELEMENT **);
extern void free_root_table_element(ROOT_TABLE_ELEMENT *);
extern int init_root_table(ROOT_TABLE **);
extern void free_root_table(ROOT_TABLE *, bool);
extern int compare_roots(ROOT *, ROOT *, int);
extern int insert_in_table(ROOT_TABLE_ELEMENT **, ROOT_TABLE **, int);
extern bool root_in_list(ROOT_TABLE *, ROOT *, ROOT **, int);
extern int generate_root_table(MATRIX_DATA *, ROOT_TABLE **, ROOT_TABLE **, int);
extern bool root_positive(ROOT *, int);
extern int generate_next_root(MATRIX_DATA *, int, ROOT *, ROOT_TABLE **, ROOT_TABLE **);
extern int output_root_table(FILE *, ROOT_TABLE *, int);
/* user_input.c */
extern void flush_stdin(void);
extern int input_string(int, char **);
extern int user_input_word(int, int, char **);
extern int user_input_file(char **);
/* string_stack.c */
extern int init_string_stack_element(int, STRING_STACK_ELEMENT **);
extern void free_string_stack_element(STRING_STACK_ELEMENT *);
extern int string_stack_push(char *, STRING_STACK_ELEMENT **);
extern char *string_stack_pop(STRING_STACK_ELEMENT **);
extern void empty_string_stack(STRING_STACK_ELEMENT *);
