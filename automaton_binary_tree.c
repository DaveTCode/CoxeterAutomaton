#include "cox_prot.h"

/******************************************************************************/
/* Function: init_binary_tree_element                                         */
/*                                                                            */
/* Returns: One of INIT_BINARY_TREE_ELEMENT_RET_CODES.                        */
/*                                                                            */
/* Parameters: IN     element - The element which is being allocated.         */
/*                                                                            */
/* Operation: Allocate the memory and initialise it to zero so that its child */
/*            pointers can be checked against the NULL pointer.               */
/******************************************************************************/
int init_binary_tree_element(BINARY_TREE_ELEMENT **element)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ret_code = INIT_BINARY_TREE_ELEMENT_OK;
  
  /****************************************************************************/
  /* Allocate memory for the element and set it to NULL initially.            */
  /****************************************************************************/
  (*element) = (BINARY_TREE_ELEMENT *) calloc(1, sizeof(BINARY_TREE_ELEMENT));
  if (*element == NULL)
  {
    ret_code = INIT_BINARY_TREE_ELEMENT_MEM_ERR;
    goto EXIT_LABEL;
  }
  
EXIT_LABEL:
  
  return(ret_code);
}

/******************************************************************************/
/* Function: free_binary_tree_element                                         */
/*                                                                            */
/* Returns: Nothing.                                                          */
/*                                                                            */
/* Parameters: IN     element - The element to be freed.                      */
/*                                                                            */
/* Operation:                                                                 */
/******************************************************************************/
void free_binary_tree_element(BINARY_TREE_ELEMENT *element)
{
  free(element);
}

/******************************************************************************/
/* Function: add_state_to_binary_tree                                         */
/*                                                                            */
/* Returns: One of ADD_STATE_TO_BINARY_TREE_RET_CODES.                        */
/*                                                                            */
/* Parameters: IN/OUT first - The first element in the tree. If this is NULL  */
/*                            then it will be returned as the new element.    */
/*             IN     new - The new automaton state to be added.              */
/*             OUT    existing - If a state is found in the tree which is the */
/*                               same as the new one then this will contain   */
/*                               the existing element on return.              */
/*             IN     num_generators - The number of group generators.        */
/*                                                                            */
/* Operation: Scan through the tree until either the element is found or the  */
/*            end of a branch is reached. If the end of a branch is reached   */
/*            then add the new element. If the state is found then return it. */
/******************************************************************************/
int add_state_to_binary_tree(BINARY_TREE_ELEMENT **first, 
                             AUTOMATON_STATE *new,
                             AUTOMATON_STATE **existing,
                             int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  BINARY_TREE_ELEMENT *curr = *first;
  BINARY_TREE_ELEMENT *prev = NULL;
  BINARY_TREE_ELEMENT *new_element;
  int compare_val = 1;
  int ret_val;
  int ret_code = ADD_STATE_TO_BINARY_TREE_OK;
  
  /****************************************************************************/
  /* Scan through the binary tree. Compare the new state to the current state */
  /* and if it is 'less' then go left and if it is 'more' then go right.      */
  /* Continue until either the state is found in the tree or the branch comes */
  /* to an end. If the state is found then return the found state and if the  */
  /* branch comes to an end then add the new state there.                     */
  /****************************************************************************/
  while (curr != NULL)
  {
    /**************************************************************************/
    /* Compare the states.                                                    */
    /**************************************************************************/
    compare_val = compare_states(new, curr->state, num_generators);
    
    prev = curr;
    
    if (compare_val == COMPARE_STATES_SMALLER)
    {
      curr = curr->left;
    }
    else if (compare_val == COMPARE_STATES_GREATER)
    {
      curr = curr->right;
    }
    else if (compare_val == COMPARE_STATES_EQUAL)
    {
      *existing = curr->state;
      ret_code = ADD_STATE_TO_BINARY_TREE_EXISTS;
      goto EXIT_LABEL;
    }
    else
    {
      printf("There was an error comparing two states.\n");
      ret_code = ADD_STATE_TO_BINARY_TREE_COMPARE_ERR;
      goto EXIT_LABEL;
    }
  }
  
  /****************************************************************************/
  /* Allocate memory for a new tree element to correspond to the state added. */
  /****************************************************************************/
  ret_val = init_binary_tree_element(&new_element);
  if (ret_val != INIT_BINARY_TREE_ELEMENT_OK)
  {
    if (ret_val == INIT_BINARY_TREE_ELEMENT_MEM_ERR)
    {
      ret_code = ADD_STATE_TO_BINARY_TREE_MEM_ERR;
      goto EXIT_LABEL;
    }
    else
    {
      printf("There was an unhandled exception creating a binary tree element.\n");
    }
  }
  
  /****************************************************************************/
  /* Set the new tree element to point to the state.                          */
  /****************************************************************************/
  new_element->state = new;
  
  /****************************************************************************/
  /* If the previous element is NULL then the tree was empty so set the first */
  /* element in the tree to be the new one. Otherwise, set the previous tree  */
  /* element to point to the new one.                                         */
  /****************************************************************************/
  if (prev == NULL)
  {
    *first = new_element;
  }
  else
  {
    if (compare_val == COMPARE_STATES_GREATER)
    {
      prev->right = new_element;
    } 
    else if (compare_val == COMPARE_STATES_SMALLER)
    {
      prev->left = new_element;
    }
  }
  
EXIT_LABEL:
  
  return(ret_code);
}
