/******************************************************************************/
/* Group: INIT_MATRIX_DATA_RET_CODES                                          */
/*                                                                            */
/* Return codes for the function init_matrix_data.                            */
/******************************************************************************/
#define INIT_MATRIX_DATA_OK      0
#define INIT_MATRIX_DATA_MEM_ERR 1

/******************************************************************************/
/* The ascii value of a lower case a. Used in printing out roots and parsing  */
/* words.                                                                     */
/******************************************************************************/
#define ASCII_LOWER_A 97

/******************************************************************************/
/* When freeing a root table it is necessary to decide whether the roots are  */
/* to be freed or not. Pass NO_DELETE_ROOTS if they are not to be freed and   */
/* DELETE_ROOTS if they are.                                                  */
/******************************************************************************/
#define DELETE_ROOTS false
#define NO_DELETE_ROOTS true

/******************************************************************************/
/* These are not arbitrary constants. They are used in a for loop to decide   */
/* the direction of search through a word when detecting if it is reduced.    */
/******************************************************************************/
#define SEARCH_FORWARDS 1
#define SEARCH_BACKWARDS -1

/******************************************************************************/
/* This structure contains all the pre calculated information about the       */
/* coxeter matrix.                                                            */
/* coxeter_matrix - The matrix of coefficients itself.                        */
/* scalar_products - A table of values corresponding to row.column where the  */
/*                   dot product here is the coxeter group dot product.       */
/* simple_action_results - A table of results from the calculation r_a(b)     */
/*                         where a and b vary over the whole generating set.  */
/* simple_roots - An array of pointers to simple roots so that they can be    */
/*                easily accessed throughout the code.                        */
/******************************************************************************/
typedef struct matrix_data
{
  long **coxeter_matrix;
  double **scalar_products;
  double **simple_action_results;
  struct root **simple_roots;
} MATRIX_DATA;
