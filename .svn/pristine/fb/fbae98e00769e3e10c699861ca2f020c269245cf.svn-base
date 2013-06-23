#include "cox_prot.h"

/******************************************************************************/
/* Function: is_symmetric                                                     */
/*                                                                            */
/* Returns: true if the matrix is symmetric and false otherwise.              */
/*                                                                            */
/* Parameters: IN     matrix - The matrix to be checked.                      */
/*             IN     matrix_size - The width and height of the matrix.       */
/*                                                                            */
/* Operation: Check each element in turn and fail immediately if any is not   */
/* symmetric.                                                                 */
/******************************************************************************/
bool is_symmetric(long **matrix, int matrix_size)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int column;
  int row;
  bool result = true;
  
  /****************************************************************************/
  /* Check input variables.                                                   */
  /****************************************************************************/
  assert(matrix != NULL);
  assert(matrix_size > 0);
  
  /****************************************************************************/
  /* Check each of the elements in the upper triangle of the matrix against   */
  /* the lower triangle to see if the matrix is symmetric.                    */
  /****************************************************************************/
  for (row = 0; row < matrix_size - 1; row++)
  {
    for (column = row + 1; column < matrix_size; column++)
    {
      if (matrix[row][column] != matrix[column][row])
      {
        result = false;
        goto EXIT_LABEL;
      }
    }
  }
  
EXIT_LABEL:
  
  return(result);
}

/******************************************************************************/
/* Function: is_reduced                                                       */
/*                                                                            */
/* Returns: true if the word is reduced and false otherwise.                  */
/*                                                                            */
/* Parameters: IN     first - The first state in the state tree.              */
/*             IN     word - A string consisting of a number of letters which */
/*                           correspond to generators in the group.           */
/*             OUT    fail_index - The index in the word that the word was no */
/*                                 longer reduced. 0 if the word was reduced. */
/*                                                                            */
/* Operation: Travel through the state graph using the next letter in the     */
/*            word as the index into the array of next states.                */
/******************************************************************************/
bool is_reduced(AUTOMATON_STATE *first, 
                char *word, 
                int *fail_index,
                int start_index,
                int finish_index)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  bool reduced;
  int ii;
  int direction;
  int generator_index;
  AUTOMATON_STATE *curr;
  
  /****************************************************************************/
  /* Check input parameters.                                                  */
  /****************************************************************************/
  assert(first != NULL);
  assert(word != NULL);
  
  /****************************************************************************/
  /* If the finish index is greater than the start index then we are looking  */
  /* at the word from left to right. If it is less then we are going from     */
  /* right to left.                                                           */
  /****************************************************************************/
  if (finish_index > start_index)
  {
    direction = SEARCH_FORWARDS;
  }
  else if (finish_index < start_index)
  {
    direction = SEARCH_BACKWARDS;
  }
  else
  {
    reduced = true;
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* Loop through the word. Convert each letter to the appropriate index for  */
  /* array access. Then move to the next state.                               */
  /****************************************************************************/
  curr = first;
  reduced = true;
  for (ii = start_index; ii != finish_index; ii += direction)
  {
    /**************************************************************************/
    /* If the current state is a valid state then continue and convert the    */
    /* next character in the word to an index into the following state.       */
    /**************************************************************************/
    generator_index = (int) word[ii] - ASCII_LOWER_A;
    
    /**************************************************************************/
    /* Move to the next state using the generator_index. This will never be   */
    /* NULL since we have pre generated the state table.                      */
    /**************************************************************************/
    curr = (AUTOMATON_STATE *) (curr->next_states[generator_index]);
    
    /**************************************************************************/
    /* If the state at the index required is NULL then this is a failure      */
    /* state and so the word is not reduced.                                  */
    /**************************************************************************/
    if (curr == NULL)
    {
      reduced = false;
      goto EXIT_LABEL;
    }
  }
  
EXIT_LABEL:
  
  /****************************************************************************/
  /* If the word was not reduced then set the fail index to ii. If it was     */
  /* then set the fail_index to 0.                                            */
  /****************************************************************************/
  if (reduced == false)
  {
    *fail_index = ii;
  }
  else
  {
    *fail_index = 0;
  }
  
  return(reduced);
}

/******************************************************************************/
/* Function: init_matrix_data                                                 */
/*                                                                            */
/* Returns: One of INIT_MATRIX_DATA_RET_CODES.                                */
/*                                                                            */
/* Parameters: OUT    matrix_data - Will be returned with all necessary       */
/*                                  memory allocated.                         */
/*             IN     num_generators - The number of group generators.        */
/*                                                                            */
/* Operation: Allocate the required amount of memory for the object itself    */
/*            then allocate the memory needed to the array of simple roots.   */
/******************************************************************************/
int init_matrix_data(MATRIX_DATA **matrix_data, int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ret_code = INIT_MATRIX_DATA_OK;
  
  /****************************************************************************/
  /* Allocate the required amount of memory for the matrix data object.       */
  /****************************************************************************/
  (*matrix_data) = (MATRIX_DATA *) calloc(sizeof(MATRIX_DATA), 1);
  
  if (*matrix_data == NULL)
  {
    ret_code = INIT_MATRIX_DATA_MEM_ERR;
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* Allocate the memory needed for the array of simple roots.                */
  /****************************************************************************/
  (*matrix_data)->simple_roots = (ROOT **) 
                                        malloc(sizeof(ROOT *) * num_generators);
  
  if ((*matrix_data)->simple_roots == NULL)
  {
    ret_code = INIT_MATRIX_DATA_MEM_ERR;
    goto EXIT_LABEL;
  }
  
EXIT_LABEL:
  
  return(ret_code);
}

/******************************************************************************/
/* Function: free_matrix_data                                                 */
/*                                                                            */
/* Returns: Nothing.                                                          */
/*                                                                            */
/* Parameters: IN     matrix_data - The data to be freed.                     */
/*                                                                            */
/* Operation: Free the array of simple roots and then the data itself. DOES   */
/*            NOT FREE ROOTS THEMSELVES.                                      */
/******************************************************************************/
void free_matrix_data(MATRIX_DATA *matrix_data, int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ii;
  
  /****************************************************************************/
  /* Free the matrix of simple roots but leave the roots themselves.          */
  /****************************************************************************/
  free(matrix_data->simple_roots);
  
  /****************************************************************************/
  /* Free the 2d matrices for the simple coxeter actions, the scalar products */
  /* products and the coxeter matrix.                                         */
  /****************************************************************************/
  for (ii = 0; ii < num_generators; ii++)
  {
    free(matrix_data->coxeter_matrix[ii]);
    free(matrix_data->scalar_products[ii]);
    free(matrix_data->simple_action_results[ii]);
  }
  free(matrix_data->coxeter_matrix);
  free(matrix_data->scalar_products);
  free(matrix_data->simple_action_results);
  
  /****************************************************************************/
  /* Free the object itself.                                                  */
  /****************************************************************************/
  free(matrix_data);
  
  return;
}

int main (void)
{
  int ret_code;
  int ret_val;
  int left_fail_index;
  int right_fail_index;
  char *word;
  char *temp_word;
  char *reduced_word;
  char *swap_word;
  char *filename;
  long **input_matrix;
  bool matrix_is_symmetric;
  MATRIX_DATA *matrix_data;
  MATRIX_FILE_INFO *file_info;
  ROOT_TABLE *root_table = NULL;
  ROOT_TABLE *minimal_root_table = NULL;
  AUTOMATON_STATE *state_tree;
  BINARY_TREE_ELEMENT *binary_state_tree = NULL;
  
  do
  {
    /**************************************************************************/
    /* Get a filename for the matrix from the user.                           */
    /* If the filename is bad then keep asking for a new one.                 */
    /**************************************************************************/
    do
    {
      ret_code = user_input_file(&filename);
      printf("\n");
    } while (ret_code != FILE_INPUT_OK);
  
    /**************************************************************************/
    /* Load the matrix data from the file. This fills in the file info object */
    /* with dimension information about the matrix and also fills in the      */
    /* matrix itself.                                                         */
    /* If the return code from this function is bad then we must fail as no   */
    /* calculations can be done without the matrix.                           */
    /**************************************************************************/
    ret_val = load_matrix_from_file(filename, 
                                    MAX_GENERATORS, 
                                    MAX_GENERATORS, 
                                    &input_matrix, 
                                    &file_info);
    
    /**************************************************************************/
    /* Check that the matrix inputted is symmetric. All coxeter matrices are. */
    /**************************************************************************/
    matrix_is_symmetric = is_symmetric(input_matrix, file_info->width);
    if (!matrix_is_symmetric)
    {
      printf("The matrix inputted is not symmetric.\n");
    }
    
  } while (ret_val != FILE_INPUT_ERR_NONE || !matrix_is_symmetric);
  
  /****************************************************************************/
  /* The filename is no longer needed so free the allocated memory.           */
  /****************************************************************************/
  free(filename);
  
  /****************************************************************************/
  /* Set up the object which will hold the precalculated matrix data.         */
  /****************************************************************************/
  ret_code = init_matrix_data(&matrix_data, file_info->width);
  assert(ret_code == INIT_MATRIX_DATA_OK);
  
  /****************************************************************************/
  /* Point the matrix data to the matrix inputted earlier.                    */
  /****************************************************************************/
  matrix_data->coxeter_matrix = input_matrix;
  
  /****************************************************************************/
  /* Create the minimal (and standard) root table for use in the automaton.   */
  /****************************************************************************/
  ret_code = generate_root_table(matrix_data, 
                                 &root_table, 
                                 &minimal_root_table, 
                                 file_info->width);
  assert(ret_code == GENERATE_ROOT_TABLE_OK);
  
  /****************************************************************************/
  /* Print out the root table for the group.                                  */
  /****************************************************************************/
  printf("The minimal root table for the group inputted is:\n");
  output_root_table(stdout, minimal_root_table, file_info->width);
  printf("\n");
  printf("The root table for the group inputted is:\n");
  output_root_table(stdout, root_table, file_info->width);
  printf("\n");
  
  /****************************************************************************/
  /* Create the state tree for the minimal root table that was generated.     */
  /****************************************************************************/
  ret_code = generate_state_tree(matrix_data, 
                                 file_info->width, 
                                 minimal_root_table, 
                                 &state_tree,
                                 &binary_state_tree);
  assert(ret_code == GENERATE_STATE_TREE_OK);
  
  /****************************************************************************/
  /* Ask the user to enter a word and then check whether it is reduced.       */
  /****************************************************************************/
  printf("To exit program enter nothing when asked for a word.\n");
  while(true)
  {
    ret_code = user_input_word(file_info->width, MAX_WORD_LEN - 1, &word);
    if (ret_code == WORD_INPUT_OK)
    {
      printf("word: %s\n", word);
      /************************************************************************/
      /* Exit program if user enters empty string for word.                   */
      /************************************************************************/
      if (strlen(word) == 0)
      {
        free(word);
        goto EXIT_LABEL;
      }
      
      /************************************************************************/
      /* Allocate the memory needed for a temporary string to store the       */
      /* reduced word in.                                                     */
      /************************************************************************/
      reduced_word = malloc((strlen(word) + 1) * sizeof(char));
      if (reduced_word == NULL)
      {
        printf("There was a memory allocation error creating the reduced word.\n");
        free(word);
        goto EXIT_LABEL;
      }
      strncpy(reduced_word, word, strlen(word) + 1);
      
      /************************************************************************/
      /* Allocate the memory required for a temporary string to hold parts of */
      /* the word during calculation.                                         */
      /************************************************************************/
      temp_word = malloc((strlen(word) + 1) * sizeof(char));
      if (temp_word == NULL)
      {
        printf("There was a memory allocation error creating the temp word.\n");
        free(word);
        free(reduced_word);
        goto EXIT_LABEL;
      }
      
      /************************************************************************/
      /* In order to reduce a word in a Coxeter group run through it from the */
      /* left until the point at which it is not reduced is found. Then a new */
      /* subword has been found. Search that sub word from the right until    */
      /* the point at which that word is not reduced is found. A new subword  */
      /* has then been formed from the first word. Delete the first and last  */
      /* elements of this subword and rerun all of the above until the word   */
      /* is reduced.                                                          */
      /************************************************************************/
      while (!is_reduced(state_tree, 
                         reduced_word, 
                         &left_fail_index, 
                         0, 
                         strlen(reduced_word)))
      {
        is_reduced(state_tree, 
                   reduced_word, 
                   &right_fail_index, 
                   left_fail_index, 
                   -1);
        
        memset(temp_word, 0, (strlen(word) + 1) * sizeof(char));
        strncpy(temp_word, reduced_word, right_fail_index);
        strncat(temp_word, 
                reduced_word + right_fail_index + 1, 
                left_fail_index - right_fail_index - 1);
        strncat(temp_word,
                reduced_word + left_fail_index + 1, 
                strlen(reduced_word) - left_fail_index - 1);
        swap_word = temp_word;
        temp_word = reduced_word;
        reduced_word = swap_word;
      }
      
      /************************************************************************/
      /* Print the reduced form of the word.                                  */
      /************************************************************************/
      printf("The reduced form of %s is:\n%s\n", word, reduced_word);
      
      /************************************************************************/
      /* Free the memory used for the word.                                   */
      /************************************************************************/
      free(word);
      free(temp_word);
      free(reduced_word);
    }
  }
  
EXIT_LABEL:
  
  /****************************************************************************/
  /* Clean up by cleaning the root tables, the automaton table and the        */
  /* automaton tree. Freeing the root table frees all of the roots as well as */
  /* the list elements whilst freeing the minimal root table then only needs  */
  /* to free the list elements.                                               */
  /* Also free the precalculated group information.                           */
  /****************************************************************************/
  free_root_table(root_table, DELETE_ROOTS);
  free_root_table(minimal_root_table, NO_DELETE_ROOTS);
  free_state_tree(binary_state_tree);
  free_state(state_tree);
  free_matrix_data(matrix_data, file_info->width);
  free_file_info(file_info);
   
  return(0);
}
