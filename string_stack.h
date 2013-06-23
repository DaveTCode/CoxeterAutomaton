/******************************************************************************/
/* This structure represent a single element in the stack of strings. It has  */
/* a string which is the data in the stack and a pointer to both the previous */
/* and next elements in the stack.                                            */
/******************************************************************************/
typedef struct string_stack_element
{
  char *string;
  struct string_stack_element *next;
  struct string_stack_element *prev;
} STRING_STACK_ELEMENT;

/******************************************************************************/
/* Group: INIT_STRING_STACK_ELEMENT_RET_CODES                                 */
/*                                                                            */
/* Return codes for the function init_string_stack_element.                   */
/******************************************************************************/
#define INIT_STRING_STACK_ELEMENT_OK      0
#define INIT_STRING_STACK_ELEMENT_MEM_ERR 1

/******************************************************************************/
/* Group: STRING_STACK_PUSH_RET_CODES                                         */
/*                                                                            */
/* Return codes for the function string_stack_push.                           */
/******************************************************************************/
#define STRING_STACK_PUSH_OK      0
#define STRING_STACK_PUSH_MEM_ERR 1
