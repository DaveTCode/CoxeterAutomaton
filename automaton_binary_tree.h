/******************************************************************************/
/* This structure is an element in the binary tree of automaton states. It is */
/* initialised using init_binary_tree_element before it can be used.          */
/******************************************************************************/
typedef struct binary_tree_element
{
  struct automaton_state *state;
  struct binary_tree_element *left;
  struct binary_tree_element *right;
} BINARY_TREE_ELEMENT;

/******************************************************************************/
/* Group: INIT_BINARY_TREE_ELEMENT_RET_CODES                                  */
/*                                                                            */
/* Function return codes for init_binary_tree_element.                        */
/******************************************************************************/
#define INIT_BINARY_TREE_ELEMENT_OK      0
#define INIT_BINARY_TREE_ELEMENT_MEM_ERR 1

/******************************************************************************/
/* Group: ADD_STATE_TO_BINARY_TREE_RET_CODES                                  */
/*                                                                            */
/* Function return codes for add_state_to_binary_tree.                        */
/******************************************************************************/
#define ADD_STATE_TO_BINARY_TREE_OK          0
#define ADD_STATE_TO_BINARY_TREE_MEM_ERR     1
#define ADD_STATE_TO_BINARY_TREE_COMPARE_ERR 2
#define ADD_STATE_TO_BINARY_TREE_EXISTS      3
