#define EPSILON_COMP_VAL 0.00001

/******************************************************************************/
/* A root structure contains the coefficients of the generators. This can be  */
/* seen in the following examples:                                            */
/* a + 3b in a group with 2 generators would be encoded as [1,3]              */
/* b + .7d in a group with 7 generators would be encoded as [0,1,0,.7,0,0,0]  */
/*                                                                            */
/* In addition to the information on the actual data this structure also      */
/* contains information on what the result of simple actions on the root were */
/* when they were first performed.                                            */
/*                                                                            */
/* A root is positive minimal if it is positive and doesn't dominate anything */
/******************************************************************************/
typedef struct root
{
  double *coefficients;
  struct root **next_roots;
  _Bool positive_minimal;
} ROOT;

/******************************************************************************/
/* A root element is an element in a one way sorted linked list. Roots are    */
/* added in order so that searching is simple and quick. Each element has a   */
/* reference to a single root.                                                */
/******************************************************************************/
typedef struct root_table_element
{
  struct root *root;
  struct root_table_element *next;
} ROOT_TABLE_ELEMENT;

/******************************************************************************/
/* This structure is a header structure for a root list. It contains a        */
/* a pointer to the first element and a long integer containing the length of */
/* the list. That way, if two root lists have different lengths then          */
/* comparing them is easier.                                                  */
/******************************************************************************/
typedef struct root_table
{
  struct root_table_element *first;
  long length;
} ROOT_TABLE;

/******************************************************************************/
/* Group: INIT_ELEMENT_RET_CODE                                               */
/*                                                                            */
/* Return codes for init_root_table_element function.                         */
/******************************************************************************/
#define INIT_ELEMENT_OK      0
#define INIT_ELEMENT_MEM_ERR 1

/******************************************************************************/
/* Group: INSERT_IN_TABLE_ERR_CODES                                           */
/*                                                                            */
/* Return codes for insert_in_table function.                                 */
/******************************************************************************/
#define INSERT_IN_TABLE_OK          0
#define INSERT_IN_TABLE_MEM_ERR     1
#define INSERT_IN_TABLE_ROOT_EXISTS 2

/******************************************************************************/
/* Group: GENERATE_ROOT_TABLE_RET_CODES                                       */
/*                                                                            */
/* Return codes for insert_in_table function.                                 */
/******************************************************************************/
#define GENERATE_ROOT_TABLE_OK      0
#define GENERATE_ROOT_TABLE_MEM_ERR 1

/******************************************************************************/
/* Group: INIT_ROOT_RET_CODES                                                 */
/*                                                                            */
/* Return codes for init_root function.                                       */
/******************************************************************************/
#define INIT_ROOT_OK      0
#define INIT_ROOT_MEM_ERR 1

/******************************************************************************/
/* Group: INIT_ROOT_TABLE_RET_CODES                                           */
/*                                                                            */
/* Return codes for the function INIT_ROOT_TABLE.                             */
/******************************************************************************/
#define INIT_ROOT_TABLE_OK      0
#define INIT_ROOT_TABLE_MEM_ERR 1

/******************************************************************************/
/* Group: GENEARTE_NEXT_ROOT_RET_CODES                                        */
/*                                                                            */
/* Return codes for generate_next_root function.                              */
/******************************************************************************/
#define GENERATE_NEXT_ROOT_OK            0
#define GENERATE_NEXT_ROOT_MEM_ERR       1
#define GENERATE_NEXT_ROOT_UNHANDLED_ERR 2

/******************************************************************************/
/* Group: OUTPUT_ROOT_TABLE_RET_CODES                                         */
/*                                                                            */
/* Return codes for output_root_table function.                               */
/******************************************************************************/
#define OUTPUT_ROOT_TABLE_OK                  0
#define OUTPUT_ROOT_TABLE_OVERFLOW_STRING_ERR 1
#define OUTPUT_ROOT_TABLE_FILE_FAILURE        2

/******************************************************************************/
/* Group: COMPARE_ROOTS_RET_CODES                                             */
/*                                                                            */
/* Return codes for compare_roots.                                            */
/******************************************************************************/
#define COMPARE_ROOTS_GREATER 1
#define COMPARE_ROOTS_EQUAL   0
#define COMPARE_ROOTS_SMALLER -1
