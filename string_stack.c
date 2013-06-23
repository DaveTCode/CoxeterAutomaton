#include "cox_prot.h"

/******************************************************************************/
/* Function: init_string_stack_element                                        */
/*                                                                            */
/* Returns: One of INIT_STRING_STACK_ELEMENT_RET_CODES.                       */
/*                                                                            */
/* Parameters: IN     max_string_length - The maximum size of the string in   */
/*                                        the stack element.                  */
/*             OUT    new_element - A pointer to the new object.              */
/*                                                                            */
/* Operation: Create memory for the stack object and the string. Set the      */
/*            previous and next pointers to NULL initially.                   */
/******************************************************************************/
int init_string_stack_element(int max_string_length, 
                              STRING_STACK_ELEMENT **new_element)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ret_code = INIT_STRING_STACK_ELEMENT_OK;
  
  /****************************************************************************/
  /* Allocate memory required for the new stack element.                      */
  /****************************************************************************/
  *new_element = malloc(sizeof(STRING_STACK_ELEMENT));
  if (new_element == NULL)
  {
    ret_code = INIT_STRING_STACK_ELEMENT_MEM_ERR;
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* Allocate the memory required for the string in the new element.          */
  /****************************************************************************/
  (*new_element)->string = malloc(max_string_length * sizeof(char));
  if ((*new_element)->string == NULL)
  {
    ret_code = INIT_STRING_STACK_ELEMENT_MEM_ERR;
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* Set the next and previous element pointers to NULL initially.            */
  /****************************************************************************/
  (*new_element)->next = NULL;
  (*new_element)->prev = NULL;
  
EXIT_LABEL:
  
  return(ret_code);
}

/******************************************************************************/
/* Function: free_string_stack_element                                        */
/*                                                                            */
/* Returns: Nothing.                                                          */
/*                                                                            */
/* Parameters: IN     old_element - Element to be freed.                      */
/*                                                                            */
/* Operation: Does NOT free the string attached to the element. This should   */
/*            be done seperately.                                             */
/******************************************************************************/
void free_string_stack_element(STRING_STACK_ELEMENT *old_element)
{
  free(old_element);
  
  return;
}

/******************************************************************************/
/* Function: string_stack_push                                                */
/*                                                                            */
/* Returns: One of STRING_STACK_PUSH_RET_CODES.                               */
/*                                                                            */
/* Parameters: IN     new_string - The string to be added to the stack.       */
/*             IN/OUT top - The current top element in the stack.             */
/*                                                                            */
/* Operation: Add the string to the stack and return the new top element.     */
/******************************************************************************/
int string_stack_push(char *new_string, STRING_STACK_ELEMENT **top)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  STRING_STACK_ELEMENT *new_element;
  int ret_code = STRING_STACK_PUSH_OK;
  int ret_val;
  
  /****************************************************************************/
  /* Create a new element to hold the string.                                 */
  /****************************************************************************/
  ret_val = init_string_stack_element(MAX_WORD_LEN, &new_element);
  if (ret_val != INIT_STRING_STACK_ELEMENT_OK)
  {
    if (ret_val == INIT_STRING_STACK_ELEMENT_MEM_ERR)
    {
      ret_code = STRING_STACK_PUSH_MEM_ERR;
      goto EXIT_LABEL;
    }
    else
    {
      printf("An unhandled error occured creating a string stack element.");
    }
  }
  
  /****************************************************************************/
  /* Set the new element to have the string inputted. It is the new top       */
  /* element in the stack so set pointers accordingly.                        */
  /****************************************************************************/
  strncpy(new_element->string, new_string, strlen(new_string) + 1);
  
  /****************************************************************************/
  /* If top is NULL then the stack is empty so we don't need to set a next or */
  /* prev pointers.                                                           */
  /****************************************************************************/
  if (*top != NULL)
  {
    (*top)->next = new_element;
    new_element->prev = (*top);
  }
  (*top) = new_element;
  
EXIT_LABEL:
  
  return(ret_code);
}

/******************************************************************************/
/* Function: string_stack_pop                                                 */
/*                                                                            */
/* Returns: The string at the top of the stack.                               */
/*                                                                            */
/* Parameters: IN/OUT top - The top element in the stack.                     */
/*                                                                            */
/* Operation: Pop the string off the stack and return the new top element.    */
/******************************************************************************/
char *string_stack_pop(STRING_STACK_ELEMENT **top)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  char *return_string;
  
  /****************************************************************************/
  /* It is a programming error if this function is called with a null pointer.*/
  /****************************************************************************/
  assert(top != NULL);
  
  /****************************************************************************/
  /* The string to be returned is the one at the top of the stack.            */
  /****************************************************************************/
  return_string = (*top)->string;
  
  /****************************************************************************/
  /* To sort out the top of the stack set the top pointer to be the one prev  */
  /* to the top. Then free the old top.                                       */
  /* Note that after this the only reference to the memory in the string is   */
  /* the one returned by this function.                                       */
  /****************************************************************************/
  if ((*top)->prev == NULL)
  {
    free_string_stack_element(*top);
    *top = NULL;
  }
  else
  {
    *top = (*top)->prev;
    free_string_stack_element((*top)->next);
  }
  
  return(return_string);
}

void empty_string_stack(STRING_STACK_ELEMENT *top)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  char *current_string;
  
  /****************************************************************************/
  /* Remove each string from the stack and free the memory it had allocated.  */
  /****************************************************************************/
  while(top != NULL)
  {
    current_string = string_stack_pop(&top);
    free(current_string);
  }
  
  return;
}
