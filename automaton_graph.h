/******************************************************************************/
/* Group: CREATE_STATE_RET_CODES.                                             */
/*                                                                            */
/* The return codes for function create_state.                                */
/******************************************************************************/
#define CREATE_STATE_OK      0
#define CREATE_STATE_MEM_ERR 1

/******************************************************************************/
/* Group: COMPARE_STATES_RET_CODES                                            */
/*                                                                            */
/* The return codes for function compare_states.                              */
/******************************************************************************/
#define COMPARE_STATES_GREATER 1
#define COMPARE_STATES_EQUAL   0
#define COMPARE_STATES_SMALLER -1

/******************************************************************************/
/* Group: GENERATE_STATE_TREE_RET_CODES                                       */
/*                                                                            */
/* The return codes for function generate_state_tree.                         */
/******************************************************************************/
#define GENERATE_STATE_TREE_OK      0
#define GENERATE_STATE_TREE_MEM_ERR 1

/******************************************************************************/
/* Group: GENERATE_NEXT_AUTOMATON_STATE_RET_CODES                             */
/*                                                                            */
/* The return codes for the function generate_next_automaton_state.           */
/******************************************************************************/
#define GENERATE_NEXT_AUTOMATON_STATE_OK      0
#define GENERATE_NEXT_AUTOMATON_STATE_MEM_ERR 1

/******************************************************************************/
/* This structure refers  to a single state that the automaton can be in. It  */
/* is either a reject state (in which case accept_state = false) or an accept */
/* state. The only other information contained is an array of pointers to     */
/* states, this array has one element per generator in the group. If          */
/* accept_state = false then next_states = NULL.                              */
/* Each state corresponds to a set of roots from Delta' so the root list is a */
/* list of pointers to those roots.                                           */
/******************************************************************************/
struct automaton_state
{
  struct automaton_state **next_states;
  struct root_table *root_table;
};
typedef struct automaton_state AUTOMATON_STATE;
