#include "cox_prot.h"

/******************************************************************************/
/* Function: create_state                                                     */
/*                                                                            */
/* Returns: One of CREATE_STATE_RET_CODES.                                    */
/*                                                                            */
/* Parameters: IN    num_generators - The number of generators used in the    */
/*                                    group.                                  */
/*             OUT   state - A pointer to the element which is being created. */
/*                                                                            */
/* Operation: Allocate memory for the object itself and then for each element */
/*            in the array of pointers to more states.                        */
/******************************************************************************/
int create_state(int num_generators, AUTOMATON_STATE **state)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ret_code = CREATE_STATE_OK;
  int ret_val;
  
  /****************************************************************************/
  /* Allocate the necessary amount of memory to the new state.                */
  /****************************************************************************/
  (*state) = (AUTOMATON_STATE *) malloc(sizeof(AUTOMATON_STATE));
  if ((*state) == NULL)
  {
    ret_code = CREATE_STATE_MEM_ERR;
    goto EXIT_LABEL;
  }
  (*state)->next_states = (AUTOMATON_STATE **) 
                              calloc(num_generators, sizeof(AUTOMATON_STATE *));
  if ((*state)->next_states == NULL)
  {
    ret_code = CREATE_STATE_MEM_ERR;
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* Set the root table to initially be NULL.                                 */
  /****************************************************************************/
  (*state)->root_table = NULL;
  
EXIT_LABEL:
  
  return(ret_code);
}

/******************************************************************************/
/* Function: free_state                                                       */
/*                                                                            */
/* Returns: Nothing.                                                          */
/*                                                                            */
/* Parameters: IN     state - The automaton state whose memory is to be       */
/*                            released.                                       */
/*                                                                            */
/* Operation: Free the array of pointers to next states and then free the     */
/*            root list attached to the state but do NOT free the roots as    */
/*            they may be referenced elsewhere. Finally free the state itself.*/
/******************************************************************************/
void free_state(AUTOMATON_STATE *state)
{
  /****************************************************************************/
  /* Free the array of next states pointers.                                  */
  /****************************************************************************/
  free(state->next_states);
  
  /****************************************************************************/
  /* Free the root table object associated with the state.                    */
  /****************************************************************************/
  if (state->root_table != NULL)
  {
    free_root_table(state->root_table, NO_DELETE_ROOTS);
  }
  
  /****************************************************************************/
  /* Free the state itself.                                                   */
  /****************************************************************************/
  free(state);
  
  return;
}

/******************************************************************************/
/* Function: free_state_tree                                                  */
/*                                                                            */
/* Returns: Nothing.                                                          */
/*                                                                            */
/* Parameters: IN     binary_tree - Will be returned with memory freed.       */
/*                                                                            */
/* Operation: Call this function recursively on the branches of the current   */
/*            node until the end of a branch is reached. Then free the state  */
/*            and the tree element and go back up the tree.                   */
/******************************************************************************/
void free_state_tree(BINARY_TREE_ELEMENT *binary_tree)
{
  /****************************************************************************/
  /* Recursively call this on both branches of the tree unless they are null. */
  /****************************************************************************/
  if (binary_tree->left != NULL)
  {
    free_state_tree(binary_tree->left);
  }
  if (binary_tree->right != NULL)
  {
    free_state_tree(binary_tree->right);
  }
  
  /****************************************************************************/
  /* Free the automaton state corresponding to this binary tree element and   */
  /* then free the binary tree element itself.                                */
  /****************************************************************************/
  free_state(binary_tree->state);
  free(binary_tree);
}

/******************************************************************************/
/* Function: compare_states                                                   */
/*                                                                            */
/* Returns: COMPARE_STATES_GREATER if the first state has a 'greater' root    */
/*          list.                                                             */
/*          COMPARE_STATES_SMALLER if the second state has a 'smaller' root   */
/*          list.                                                             */
/*          COMPARE_STATES_EQUAL if the root lists are the same.              */
/*                                                                            */
/* Parameters: IN     state_a - The first state.                              */
/*             IN     state_b - The second state.                             */
/*             IN     num_generators - The number of group generators.        */
/*                                                                            */
/* Operation: We define a root list to be greater if its length is larger     */
/*            than the list to which it is being compared.                    */
/*            If the lengths are the same then walk along the root lists      */
/*            until a root is found which is different in the two lists. If   */
/*            this root is larger in list a then list a is greater. Otherwise */
/*            list b is greater. Finally if no different root is found and    */
/*            the lengths are the same then the states are equal.             */
/******************************************************************************/
int compare_states(AUTOMATON_STATE *state_a, 
                   AUTOMATON_STATE *state_b, 
                   int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int compare_roots_ret = COMPARE_ROOTS_EQUAL;
  ROOT_TABLE_ELEMENT *state_a_curr;
  ROOT_TABLE_ELEMENT *state_b_curr;
  int result;
  
  /****************************************************************************/
  /* Check input values are valid. If not the fail program as it is a coding  */
  /* error not a user input error.                                            */
  /****************************************************************************/
  assert(state_a != NULL);
  assert(state_b != NULL);
  assert(num_generators > 0);
  
  /****************************************************************************/
  /* Set up temporary variables to walk through the root lists.               */
  /****************************************************************************/
  state_a_curr = (ROOT_TABLE_ELEMENT *) (state_a->root_table->first);
  state_b_curr = (ROOT_TABLE_ELEMENT *) (state_b->root_table->first);
  
  /****************************************************************************/
  /* First compare the different sizes of root list. If list a is longer than */
  /* list b then say that a is greater. If b is longer then b is greater.     */
  /****************************************************************************/
  if (state_a->root_table->length > state_b->root_table->length)
  {
    result = COMPARE_STATES_GREATER;
  }
  else if (state_a->root_table->length < state_b->root_table->length)
  {
    result = COMPARE_STATES_SMALLER;
  }
  else
  {
    
    /**************************************************************************/
    /* Walk through the root list for both states comparing the roots.        */
    /**************************************************************************/
    while(state_a_curr != NULL && 
          state_b_curr != NULL && 
          compare_roots_ret == COMPARE_ROOTS_EQUAL)
    {
      /************************************************************************/
      /* Compare the current pair of roots. Check that the return value from  */
      /* the function was OK.                                                 */
      /************************************************************************/
      compare_roots_ret = compare_roots(state_a_curr->root, 
                                        state_b_curr->root,
                                        num_generators);
      
      /************************************************************************/
      /* Get the next element in each root list.                              */
      /************************************************************************/
      state_a_curr = state_a_curr->next;
      state_b_curr = state_b_curr->next;
    }
  
    /**************************************************************************/
    /* If the loop above exits due to two roots being different then set the  */
    /* result to be the answer to that compare. Otherwise, if it exits because*/
    /* all elements were the same then return that the two lists are equal.   */
    /**************************************************************************/
    if (compare_roots_ret == COMPARE_ROOTS_GREATER)
    {
      result = COMPARE_STATES_GREATER;
    }
    else if (compare_roots_ret == COMPARE_ROOTS_SMALLER)
    {
      result = COMPARE_STATES_SMALLER;
    }
    else
    {
      result = COMPARE_STATES_EQUAL;
    }
  }
  
  return(result);
}

/******************************************************************************/
/* Function: state_in_tree                                                    */
/*                                                                            */
/* Returns: true if the state is in the tree and false otherwise.             */
/*                                                                            */
/* Parameters: IN     first - The head of the tree branch being searched.     */
/*             IN     new_state - The state we are searching for.             */
/*             OUT    existing_state - A pointer to the state that is found   */
/*                                     if return value is true and NULL if    */
/*                                     the return value is false.             */
/*             IN     num_generators - The number of group generators.        */
/*                                                                            */
/* Operation: Compare the current state with the one we are searching for. If */
/*            it is not the current state then recursively call this function */
/*            for each of the next states in the array.                       */
/******************************************************************************/
bool state_in_tree(AUTOMATON_STATE *first, 
                   AUTOMATON_STATE *new_state,
                   AUTOMATON_STATE **existing_state,
                   int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  AUTOMATON_STATE *current = first;
  bool result = false;
  int ii;
  
  /****************************************************************************/
  /* Traverse the tree from the first element following each branch until the */
  /* state in question is found. If it is then exit and return true.          */
  /****************************************************************************/
  if (current != NULL)
  {
    /**************************************************************************/
    /* Compare the current state with the one that is being searched for.     */
    /**************************************************************************/
    if (compare_states(current, new_state, num_generators))
    {
      result = true;
      (*existing_state) = current;
      goto EXIT_LABEL;
    }
    
    /**************************************************************************/
    /* Call this function recursively on each of the next states.             */
    /**************************************************************************/
    for (ii = 0; ii < num_generators; ii++)
    {
      result = state_in_tree(current->next_states[ii], 
                             new_state, 
                             existing_state, 
                             num_generators);
      if (result)
      {
        goto EXIT_LABEL;
      }
    }
  }
  
EXIT_LABEL:
  
  return(result);
}

/******************************************************************************/
/* Function: generate_state_tree                                              */
/*                                                                            */
/* Returns: One of GENERATE_STATE_TREE_RET_CODES.                             */
/*                                                                            */
/* Parameters: IN     matrix_data - Pre calculated data about the group.      */
/*             IN     num_generators - The number of group generators.        */
/*             IN     root_list - The list of roots over which the tree is to */
/*                                be created.                                 */
/*             OUT    tree - The state tree generated by this function.       */
/*             IN/OUT binary_tree - A binary tree which will be returned with */
/*                                  all the states in it.                     */
/*                                                                            */
/* Operation: Create the initial state (with an empty root list).             */
/*            For each generator call generate_next_automaton_state. This     */
/*            recursively generate a tree of all possible states.             */
/******************************************************************************/
int generate_state_tree(MATRIX_DATA *matrix_data, 
                        int num_generators, 
                        ROOT_TABLE *root_table,
                        AUTOMATON_STATE **tree,
                        BINARY_TREE_ELEMENT **binary_tree)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  AUTOMATON_STATE *tree_start;
  ROOT_TABLE *empty_root_table;
  int ret_code = GENERATE_STATE_TREE_OK;
  int ret_val;
  int ii;
  
  /****************************************************************************/
  /* Create the first element in the state tree.                              */
  /****************************************************************************/
  ret_val = create_state(num_generators, &tree_start);
  if (ret_val != CREATE_STATE_OK)
  {
    if (ret_val == CREATE_STATE_MEM_ERR)
    {
      printf("There was a memory allocation error creating the state tree.\n");
      ret_code = GENERATE_STATE_TREE_MEM_ERR;
      goto EXIT_LABEL;
    }
    else
    {
      printf("There was an unhandled error creating the state tree.\n");
    }
  }
  
  /****************************************************************************/
  /* Manually create a root table for the top of the tree.                    */
  /****************************************************************************/
  ret_val = init_root_table(&empty_root_table);
  if (ret_val != INIT_ROOT_TABLE_OK)
  {
    if (ret_val == INIT_ROOT_TABLE_MEM_ERR)
    {
      printf("A memory allocation error occured creating root table for head of tree.\n");
      ret_code = GENERATE_STATE_TREE_MEM_ERR;
      goto EXIT_LABEL;
    }
    else
    {
      printf("An unhandled error occured initailising root table for the head of the tree.\n");
    }
  }
  
  /****************************************************************************/
  /* Set the start of the tree to have the empty root table as its table.     */
  /****************************************************************************/
  tree_start->root_table = empty_root_table;
  
  /****************************************************************************/
  /* Run through each of the generators filling in the branch of the tree     */
  /* starting from the simple root of that generator.                         */
  /****************************************************************************/
  for (ii = 0; ii < num_generators; ii++)
  {
    ret_val = generate_next_automaton_state(matrix_data,
                                            num_generators, 
                                            root_table, 
                                            tree_start, 
                                            tree_start, 
                                            binary_tree,
                                            ii);
    if (ret_val != GENERATE_NEXT_AUTOMATON_STATE_OK)
    {
      if (ret_val == GENERATE_NEXT_AUTOMATON_STATE_MEM_ERR)
      {
        ret_code = ret_val;
        goto EXIT_LABEL;
      }
      else
      {
        printf("An unhandled error occured during creating automaton.\n");
      }
    }
  }
  
  /****************************************************************************/
  /* Set the return value for the state tree to be the tree which we have     */
  /* generated.                                                               */
  /****************************************************************************/
  (*tree) = tree_start;
  
EXIT_LABEL:
  
  return(ret_code);
}

/******************************************************************************/
/* Function: generate_next_automaton_state                                    */
/*                                                                            */
/* Returns: One of GENERATE_NEXT_AUTOMATON_STATE_RET_CODES.                   */
/*                                                                            */
/* Parameters: IN     matrix_data - Precalculated information about the group.*/
/*             IN     num_generators - The number of group generators.        */
/*             IN     root_table - The automaton consists of sets of elements */
/*                                 from this list.                            */
/*             IN     start - The first element in the root tree.             */
/*             IN/OUT tree_state - The current element in the tree. It will   */
/*                                 be returned with a pointer to the next     */
/*                                 state following the generator.             */
/*             IN/OUT binary_tree - A binary tree of the states currently in  */
/*                                  the automaton. Used for quick searching.  */
/*             IN     generator - The generator which we are adding to the    */
/*                                state that was inputted.                    */
/*                                                                            */
/* Operation: Check whether the generator already exists in the root list of  */
/*            the state passed in. If it does then the new state is a fail    */
/*            state.                                                          */
/*            Create the new root list by applying the generator to the old   */
/*            states list and then adding the generator as a simple root.     */
/*            If this state already exists then point the current one at it   */
/*            return. If not then point the current state at the new state    */
/*            and repeat for all generators.                                  */
/******************************************************************************/
int generate_next_automaton_state(MATRIX_DATA *matrix_data,
                                  int num_generators, 
                                  ROOT_TABLE *root_table,
                                  AUTOMATON_STATE *start,
                                  AUTOMATON_STATE *tree_state,
                                  BINARY_TREE_ELEMENT **binary_tree,
                                  int generator)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  AUTOMATON_STATE *new_state;
  AUTOMATON_STATE *existing_state;
  ROOT_TABLE *new_root_list = NULL;
  ROOT *simple_root;
  ROOT *existing_root;
  ROOT_TABLE_ELEMENT *simple_root_element;
  int ret_code = GENERATE_NEXT_AUTOMATON_STATE_OK;
  int ret_val;
  int ii;
  
  /****************************************************************************/
  /* Check that the input variables are valid.                                */
  /****************************************************************************/
  assert(root_table != NULL);
  assert(num_generators > 0);
  assert(tree_state != NULL);
  assert(generator < num_generators);
  
  /****************************************************************************/
  /* Create the new state.                                                    */
  /****************************************************************************/
  ret_val = create_state(num_generators, &new_state);
  if (ret_val != CREATE_STATE_OK)
  {
    if (ret_val == CREATE_STATE_MEM_ERR)
    {
      printf("There was a memory allocation error creating the state tree.\n");
      goto EXIT_LABEL;
    }
    else
    {
      printf("There was an unhandled error creating the state tree.\n");
    }
  }
  
  /****************************************************************************/
  /* Retrieve the simple root object from the matrix data.                    */
  /****************************************************************************/
  simple_root = matrix_data->simple_roots[generator];
  
  /****************************************************************************/
  /* Check whether the generator exists in the root list. If it does then     */
  /* this is a failure path so set the accept state to false and stop the     */
  /* branch.                                                                  */
  /****************************************************************************/
  if (root_in_list(tree_state->root_table, 
                   simple_root, 
                   &existing_root, 
                   num_generators))
  {
    tree_state->next_states[generator] = NULL;
    free_state(new_state);
  }
  else
  {
    /**************************************************************************/
    /* Apply the generator to each of the roots in the root list to create a  */
    /* new list.                                                              */
    /**************************************************************************/
    cox_action_on_root_list(tree_state->root_table,
                            &new_root_list,
                            generator,
                            num_generators,
                            matrix_data);
    
    /**************************************************************************/
    /* Create a root element to correspond to the simple root so that it can  */
    /* be added to the list.                                                  */
    /**************************************************************************/
    init_root_table_element(&simple_root_element);
    simple_root_element->root = simple_root;
    
    /**************************************************************************/
    /* Add the simple root to the list.                                       */
    /**************************************************************************/
    ret_val = insert_in_table(&simple_root_element, 
                              &new_root_list, 
                              num_generators);
    if (ret_val != INSERT_IN_TABLE_OK)
    {
      if (ret_val == INSERT_IN_TABLE_MEM_ERR)
      {
        printf("A memory occured error adding the simple root to the list.\n");
        ret_code = GENERATE_NEXT_AUTOMATON_STATE_MEM_ERR;
        goto EXIT_LABEL;
      }
    }
    
    /**************************************************************************/
    /* Use the newly created root list as the root list for this state.       */
    /**************************************************************************/
    new_state->root_table = new_root_list;
    
    /**************************************************************************/
    /* Attempt to add the new state to the binary tree.                       */
    /**************************************************************************/
    ret_val = add_state_to_binary_tree(binary_tree, 
                                       new_state, 
                                       &existing_state, 
                                       num_generators);
    if (ret_val == ADD_STATE_TO_BINARY_TREE_OK)
    {
      /************************************************************************/
      /* If the state wasn't in the binary tree then it has now been added    */
      /* and we point the current one at it in the context of the state tree. */
      /************************************************************************/
      tree_state->next_states[generator] = new_state;
      
      /************************************************************************/
      /* For each of the generators recursively perform this function on the  */
      /* new state.                                                           */
      /************************************************************************/
      for (ii = 0; ii < num_generators; ii++)
      {
        ret_val = generate_next_automaton_state(matrix_data,
                                                num_generators, 
                                                root_table, 
                                                start,
                                                new_state, 
                                                binary_tree,
                                                ii);
        if (ret_val != GENERATE_NEXT_AUTOMATON_STATE_OK)
        {
          if (ret_val == GENERATE_NEXT_AUTOMATON_STATE_MEM_ERR)
          {
            ret_code = ret_val;
            goto EXIT_LABEL;
          }
          else
          {
            printf("An unhandled error occured during creating automaton.\n");
          }
        }
      }
    }
    else if (ret_val == ADD_STATE_TO_BINARY_TREE_EXISTS)
    {
      /************************************************************************/
      /* If the state existed in the tree then it has been retrieved so point */
      /* the current one at it in the context of the tree.                    */
      /************************************************************************/
      tree_state->next_states[generator] = existing_state;
      free_state(new_state);
    }
    else if (ret_val == ADD_STATE_TO_BINARY_TREE_MEM_ERR)
    {
      printf("A memory allocation error occured adding state to binary tree.\n");
      free_state(new_state);
      ret_code = GENERATE_NEXT_AUTOMATON_STATE_MEM_ERR;
      goto EXIT_LABEL;
    }
    else if (ret_val == ADD_STATE_TO_BINARY_TREE_COMPARE_ERR)
    {
      printf("An error occured comparing two states.\n");
      free_state(new_state);
      ret_code = GENERATE_NEXT_AUTOMATON_STATE_MEM_ERR;
      goto EXIT_LABEL;
    }
    else
    {
      printf("An unhandled error occured adding state to binary tree.\n");
    }
  }
  
EXIT_LABEL:
  
  return(ret_code);
}
