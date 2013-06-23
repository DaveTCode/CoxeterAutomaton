#include "cox_prot.h"

/******************************************************************************/
/* Function: cox_scalar_product                                               */
/*                                                                            */
/* Returns: The value of the scalar product a.b                               */
/*                                                                            */
/* Parameters: IN     matrix_data - Precalculated data about group. Includes  */
/*                                  matrix of coefficients.                   */
/*             IN     a                                                       */
/*             IN     b                                                       */
/*                                                                            */
/* Operation: Check that the coefficient matrix is not null and then find a.b */
/*            according to the formula:                                       */
/*            a.b = 1                               if a = b                  */
/*                = -cos(pi / coxeter_matrix[a][b]) if a != b                 */
/******************************************************************************/
double cox_scalar_product(MATRIX_DATA *matrix_data,
                          int a, 
                          int b)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  double scalar_product_a_b;
  
  /****************************************************************************/
  /* Check that the coxeter matrix is not a null pointer.                     */
  /****************************************************************************/
  assert(matrix_data != NULL);
  
  /****************************************************************************/
  /* If a = b then the function returns 1. Otherwise it returns:              */
  /* - cos(pi / coxeter_matrix[a][b]).                                        */
  /****************************************************************************/
  if (a == b)
  {
    scalar_product_a_b = 1.0;
  }
  else
  {
    /**************************************************************************/
    /* In order to cope with groups which have infinity as the exponent for   */
    /* two generators (in this case a and b), we use 0 in the coxeter matrix. */
    /* Therefore since as n -> inf cos(pi/n) -> cos(0) = 1 we hard code that  */
    /* to avoid divide by zero errors.                                        */
    /**************************************************************************/
    if (matrix_data->coxeter_matrix[a][b] == 0)
    {
      scalar_product_a_b = (double) -1.0;
    }
    else
    {
      scalar_product_a_b = (double) (-1.0 * 
                         cos(M_PI / (float) matrix_data->coxeter_matrix[a][b]));
    }
  }
  
  return(scalar_product_a_b);
}

/******************************************************************************/
/* Function: cox_scalar_product_root                                          */
/*                                                                            */
/* Returns: The solution to a.root for the given parameters.                  */
/*                                                                            */
/* Parameters: IN/OUT matrix_data - Precalculated group information.          */
/*             IN     a - The generator we are applying to the root.          */
/*             IN     root - The root in the calculation.                     */
/*             IN     num_generators - Number of generators in the group.     */
/*                                                                            */
/* Operation: The scalar product of a generator and a root is calculated by   */
/*            noting that the product is bilinear and therefore:              */
/*            a.(k1b + k2c ... + knx) = k1(a.b) + ... + kn(a.x)               */
/*            It is crucial that the matrix data has been filled in by this   */
/*            point.                                                          */
/******************************************************************************/
double cox_scalar_product_root(MATRIX_DATA *matrix_data,
                               int a,
                               ROOT *root,
                               int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ii;
  double answer = 0.0; 
  
  /****************************************************************************/
  /* Check input variables.                                                   */
  /****************************************************************************/
  assert(matrix_data != NULL);
  assert(root != NULL);
  assert(num_generators > 0);
  assert(a >= 0);
  assert(a < num_generators);
  
  /****************************************************************************/
  /* Loop through each of the coefficients in the root and perform the scalar */
  /* product of a.ii multiplying it by the root coefficient. Sum all these.   */
  /****************************************************************************/
  for (ii = 0; ii < num_generators; ii++)
  {
    answer += root->coefficients[ii] * matrix_data->scalar_products[ii][a]; 
  }
  
  return(answer);
}

/******************************************************************************/
/* Function: fill_scalar_product_matrix                                       */
/*                                                                            */
/* Returns: One of FILL_SCALAR_PRODUCT_MATRIX_RET_CODES.                      */
/*                                                                            */
/* Parameters: IN/OUT matrix_data - Precalculated information about the group */
/*                                  which will include a matrix of scalar     */
/*                                  products after this function.             */
/*             IN     num_generators - The number of group generators.        */
/*                                                                            */
/* Operation: Allocate memory and fill in the matrix of coefficients using    */
/*            the fact that the scalar product is symmetric to reduce the     */
/*            number of calculations.                                         */
/******************************************************************************/
int fill_scalar_product_matrix(MATRIX_DATA *matrix_data,
                               int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int row;
  int column;
  int ret_code = FILL_SCALAR_PRODUCT_MATRIX_OK;
  
  /****************************************************************************/
  /* If the matrix used to store scalar products has not been created then do */
  /* that.                                                                    */
  /****************************************************************************/
  matrix_data->scalar_products = (double **) malloc(sizeof(double *) * 
                                                                num_generators);
  if ((matrix_data->scalar_products) == NULL)
  {
    ret_code = FILL_SCALAR_PRODUCT_MATRIX_MEM_ERR;
    goto EXIT_LABEL;
  }
  
  for (row = 0; row < num_generators; row++)
  {
    matrix_data->scalar_products[row] = (double *) malloc(sizeof(double) * 
                                                                num_generators);
    if (matrix_data->scalar_products[row] == NULL)
    {
      ret_code = FILL_SCALAR_PRODUCT_MATRIX_MEM_ERR;
      goto EXIT_LABEL;
    }
  }
  
  /****************************************************************************/
  /* Fill the matrix in with the scalar products of the generators.           */
  /****************************************************************************/
  for (row = 0; row < num_generators; row++)
  {
    matrix_data->scalar_products[row][row] = 1;
    
    for (column = row + 1; column < num_generators; column++)
    {
      /************************************************************************/
      /* The scalar product is symmetric so matrix of scalar products is as   */
      /* well.                                                                */
      /************************************************************************/
      matrix_data->scalar_products[row][column] = 
                                                 cox_scalar_product(matrix_data,
                                                                    row,
                                                                    column);
      matrix_data->scalar_products[column][row] = 
                                      matrix_data->scalar_products[row][column];
    }
  }
  
EXIT_LABEL:
  
  return(ret_code);
}

/******************************************************************************/
/* Function: cox_action                                                       */
/*                                                                            */
/* Returns: The value of the coefficient of a after the coxeter action r_a(v) */
/*                                                                            */
/* Parameters: IN     coxeter_matrix - Matrix of group coefficients.          */
/*             IN/OUT scalar_products - Scalar products of the generators.    */
/*             IN     a                                                       */
/*             IN     v                                                       */
/*                                                                            */
/* Operation: Return the value of the group action of a on v according to the */
/* formula given inline below.                                                */
/******************************************************************************/
double cox_action(MATRIX_DATA *matrix_data,
                  int num_generators, 
                  int a, 
                  int v)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  double cox_action_a_v;
  
  /****************************************************************************/
  /* Check input variables are valid. The values of a and v correspond to     */
  /* generators so must be less than the total number of generators.          */
  /****************************************************************************/
  assert(matrix_data != NULL);
  assert(num_generators > 0);
  assert(a < num_generators);
  assert(v < num_generators);
  
  /****************************************************************************/
  /* Calculate the action of a on v according to the formula:                 */
  /* r_a(v) = v - 2(v.a)a                                                     */
  /****************************************************************************/
  cox_action_a_v = ((double) -2.0) * matrix_data->scalar_products[v][a];
  
  return(cox_action_a_v);
}

/******************************************************************************/
/* Function: fill_cox_action_matrix                                           */
/*                                                                            */
/* Returns: One of FILL_COX_ACTION_MATRIX_RET_CODES.                          */
/*                                                                            */
/* Parameters: IN     matrix_data - Will be returned with the array of simple */
/*                                  action calculations filled in.            */
/*             IN     num_generators - The number of group generators.        */
/*                                                                            */
/* Operation: Allocate the memory necessary for a 2D matrix of coefficients.  */
/*            Fill in the matrix symmetrically.                               */
/******************************************************************************/
int fill_cox_action_matrix(MATRIX_DATA *matrix_data, int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ii;
  int jj;
  int ret_code = FILL_COX_ACTION_MATRIX_OK;
  double curr_action_val;
  
  /****************************************************************************/
  /* If the array of simple action results is NULL then we allocate the       */
  /* memory for it and fill it in with the result of r_a(b) for all elements  */
  /* of the generating set.                                                   */
  /****************************************************************************/
  if (matrix_data->simple_action_results == NULL)
  {
    matrix_data->simple_action_results = (double **) 
                                      malloc(sizeof(double *) * num_generators);
    if (matrix_data->simple_action_results == NULL)
    {
      ret_code = FILL_COX_ACTION_MATRIX_MEM_ERR;
      goto EXIT_LABEL;
    }
    for (ii = 0; ii < num_generators; ii++)
    {
      matrix_data->simple_action_results[ii] = (double *)
                                       malloc(sizeof(double ) * num_generators);
      if (matrix_data->simple_action_results[ii] == NULL)
      {
        ret_code = FILL_COX_ACTION_MATRIX_MEM_ERR;
        goto EXIT_LABEL;
      }
    }
    
    /**************************************************************************/
    /* Compute the value of r_ii(jj) and fill in the table accordingly.       */
    /* The table is symmetric and the diagonal is not necessary to be filled. */
    /**************************************************************************/
    for (ii = 0; ii < num_generators; ii++)
    {
      for (jj = ii + 1; jj < num_generators; jj++)
      {
        curr_action_val = cox_action(matrix_data,  
                                     num_generators, 
                                     ii, 
                                     jj);
        
        matrix_data->simple_action_results[ii][jj] = curr_action_val;
        matrix_data->simple_action_results[jj][ii] = curr_action_val;
      }
    }
  }
  
EXIT_LABEL:
  
  return(ret_code);
}

/******************************************************************************/
/* Function: cox_action_on_root                                               */
/*                                                                            */
/* Returns: One of COX_ACTION_ON_ROOT_RET_CODES.                              */
/*                                                                            */
/* Parameters: IN     matrix - The coxeter matrix for the group.              */
/*             IN/OUT scalar_products - The matrix of scalar products. If it  */
/*                                      is empty on input then it will be     */
/*                                      filled in inside this function.       */
/*             IN/OUT simple_action_results - A matrix containing the results */
/*                                            of r_a(b) for all a, b.         */
/*             IN     num_generators - The number of generators in the group. */
/*             IN     root - The input root that is being used to calculate.  */
/*             OUT    returned_root - This will contain the coefficients that */
/*                                    are calculated as r_a(root).            */
/*             OUT    new_root_exists - Returns as true if the root has been  */
/*                                      calculated before. Otherwise false.   */
/*                                                                            */
/* Operation: For each of the coefficients in the input root. Check that it   */
/*            is not zero. If it isn't then calculate the action of a on this */
/*            simple root. Add together all of these and return that as the   */
/*            returned root.                                                  */
/******************************************************************************/
int cox_action_on_root(MATRIX_DATA *matrix_data,
                       int num_generators,
                       int a,
                       ROOT *root,
                       ROOT **returned_root,
                       ROOT_TABLE *root_table,
                       bool *new_root_exists)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  double curr_coefficient;
  double curr_action_val;
  ROOT *existing_root;
  int ii;
  int ret_val;
  int ret_code = COX_ACTION_ON_ROOT_OK;
  
  /****************************************************************************/
  /* Check input parameters.                                                  */
  /****************************************************************************/
  assert(matrix_data != NULL);
  assert(matrix_data->coxeter_matrix != NULL);
  assert(num_generators > 0);
  
  /****************************************************************************/
  /* If this calculation has already been performed then there will be an     */
  /* entry in the next_roots table for the root passed in (with index a).     */
  /* When this is the case simply return a pointer to the answer.             */
  /****************************************************************************/
  if (root->next_roots[a] != NULL)
  {
    (*returned_root) = root->next_roots[a];
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* Create the root object.                                                  */
  /****************************************************************************/
  ret_val = init_root(num_generators, returned_root);
  if (ret_val != INIT_ROOT_OK)
  {
    if (ret_val == INIT_ROOT_MEM_ERR)
    {
      ret_code = COX_ACTION_ON_ROOT_MEM_ERR_SUB_FUNC;
      goto EXIT_LABEL;
    }
  }
  
  /****************************************************************************/
  /* Loop through the roots coefficient array. For each of those calculate    */
  /* the action of a on the simple root and multiply the result by the        */
  /* coefficient.                                                             */
  /****************************************************************************/
  for (ii = 0; ii < num_generators; ii++)
  {
    curr_coefficient = root->coefficients[ii];

    if (fabs(curr_coefficient) > EPSILON_COMP_VAL)
    {
      /************************************************************************/
      /* The solution to r_a(a) is always -a regardless of which simple root  */
      /* a is.                                                                */
      /************************************************************************/
      if (ii == a)
      {
        (*returned_root)->coefficients[ii]-= curr_coefficient;
      }
      else
      {
        curr_action_val = matrix_data->simple_action_results[ii][a];
        
        /**********************************************************************/
        /* Add the returned action value to the root that is being calculated.*/
        /**********************************************************************/
        (*returned_root)->coefficients[a] += curr_action_val * curr_coefficient;
        (*returned_root)->coefficients[ii] += curr_coefficient;
      }
    }
  }
  
  /****************************************************************************/
  /* Check whether that root already exists in the list. If it does then set  */
  /* it to point to the existing one and free the memory used.                */
  /****************************************************************************/
  if (root_in_list(root_table, *returned_root, &existing_root, num_generators))
  {
    free_root(*returned_root);
    *new_root_exists = true;
    *returned_root = existing_root;
  }
  else
  {
    *new_root_exists = false;
  }
  
  /****************************************************************************/
  /* Add an entry pointing to the root just created into the next_roots array */
  /* for the current root (index a). This prevents calculations happening too */
  /* often.                                                                   */
  /****************************************************************************/
  root->next_roots[a] = *returned_root;
  
EXIT_LABEL:
  
  return(ret_code);
}

/******************************************************************************/
/* Function: cox_action_on_root_list                                          */
/*                                                                            */
/* Returns: One of COX_ACTION_ON_ROOT_LIST_RET_CODES.                         */
/*                                                                            */
/* Parameters: IN     first - The first element in the old list.              */
/*             OUT    new - The first element in the new list. This must be   */
/*                          NULL.                                             */
/*             IN     generator - The generator that is being applied to the  */
/*                                root list.                                  */
/*             IN     num_generators - The number of group generators.        */
/*             IN/OUT matrix_data - Precalculated data about the group.       */
/*                                                                            */
/* Operation: Walk through the root list that was passed in.                  */
/*            For each root perform r_generator(root) and store the result in */
/*            the new list. Ignore duplications.                              */
/*            Discard those roots which are not positive minimal.             */
/******************************************************************************/
int cox_action_on_root_list(ROOT_TABLE *root_table,
                            ROOT_TABLE **new,
                            int generator,
                            int num_generators,
                            MATRIX_DATA *matrix_data)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  ROOT_TABLE_ELEMENT *current_table_element = root_table->first;
  ROOT_TABLE_ELEMENT *new_table_element;
  int ret_code = COX_ACTION_ON_ROOT_LIST_OK;
  int ret_val;
  bool root_exists;
  
  /****************************************************************************/
  /* Walk through the list doing the coxeter action on each root in turn and  */
  /* inserting the result into the new list.                                  */
  /****************************************************************************/
  while (current_table_element != NULL)
  {
    /**************************************************************************/
    /* Create a new table element to hold the newly created root.             */
    /**************************************************************************/
    ret_val = init_root_table_element(&new_table_element);
    if (ret_val != INIT_ELEMENT_OK)
    {
      if (ret_val == INIT_ELEMENT_MEM_ERR)
      {
        printf("There was a memory allocation error creating new root elements.\n");
        ret_code = COX_ACTION_ON_ROOT_LIST_MEM_ERR;
        goto EXIT_LABEL;
      }
    }
    
    /**************************************************************************/
    /* Apply the action to the current root.                                  */
    /**************************************************************************/
    ret_val = cox_action_on_root(matrix_data,
                                 num_generators, 
                                 generator, 
                                 current_table_element->root,
                                 (&new_table_element->root),
                                 NULL,
                                 &root_exists);
    if (ret_val != COX_ACTION_ON_ROOT_OK)
    {
      if (ret_val == COX_ACTION_ON_ROOT_MEM_ERR || 
                                            COX_ACTION_ON_ROOT_MEM_ERR_SUB_FUNC)
      {
        printf("Memory allocation error attempting to apply root to list.\n");
        ret_code = COX_ACTION_ON_ROOT_LIST_MEM_ERR;
        goto EXIT_LABEL;
      }
      else
      {
        printf("Unhandled error trying to create new root list.\n");
      }
    }
    
    /**************************************************************************/
    /* Insert the newly created root into the new table if and only if it is  */
    /* positive minimal.                                                      */
    /**************************************************************************/
    if (new_table_element->root->positive_minimal)
    {
      ret_val = insert_in_table(&new_table_element, new, num_generators);
      if (ret_val != INSERT_IN_TABLE_OK)
      {
        if (ret_val == INSERT_IN_TABLE_MEM_ERR)
        {
          printf("There was a memory allocation error adding root to table.\n");
          ret_code = COX_ACTION_ON_ROOT_LIST_MEM_ERR;
          goto EXIT_LABEL;
        }
      }
    }
    else
    {
      free_root_table_element(new_table_element);
    }
    
    /**************************************************************************/
    /* Move onto the next element in the table.                               */
    /**************************************************************************/
    current_table_element = current_table_element->next;
    
  }
  
EXIT_LABEL:
  
  return(ret_code);
}
