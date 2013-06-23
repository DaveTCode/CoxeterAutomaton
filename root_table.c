#include "cox_prot.h"

/******************************************************************************/
/* Function: init_root                                                        */
/*                                                                            */
/* Returns: One of INIT_ROOT_RET_CODES.                                       */
/*                                                                            */
/* Parameters: IN     num_generators - Used as the size of the coefficient.   */
/*                                     array.                                 */
/*             OUT    root - The newly created root to be returned.           */
/*                                                                            */
/* Operation: Allocate memory for root object.                                */
/*            Allocate memory for root coefficients.                          */
/*            Initialise the coefficients to all 0.                           */
/******************************************************************************/
int init_root(int num_generators, ROOT **root)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ret_code = INIT_ROOT_OK;

  /****************************************************************************/
  /* Allocate required amount of memory and return a failure if memory not    */
  /* available.                                                               */
  /****************************************************************************/
  (*root) = (ROOT *) malloc(sizeof(ROOT));
  if (*root == NULL)
  {
    ret_code = INIT_ROOT_MEM_ERR;
    goto EXIT_LABEL;
  }

  /****************************************************************************/
  /* Allocate the necessary memory to the array of coefficients and init them */
  /* to zero.                                                                 */
  /****************************************************************************/
  (*root)->coefficients = (double *) calloc(num_generators, sizeof(double));
  if ((*root)->coefficients == NULL)
  {
    ret_code = INIT_ROOT_MEM_ERR;
    goto EXIT_LABEL;
  }

  /****************************************************************************/
  /* Create memory for the array of pointers to next roots and initialise it  */
  /* to 0 so that the elements can be tested for NULL.                        */
  /****************************************************************************/
  (*root)->next_roots = (ROOT **) calloc(num_generators, sizeof(ROOT *));
  if ((*root)->next_roots == NULL)
  {
    ret_code = INIT_ROOT_MEM_ERR;
    goto EXIT_LABEL;
  }

  /****************************************************************************/
  /* All roots are initially assumed to be positive minimal.                  */
  /****************************************************************************/
  (*root)->positive_minimal = true;

EXIT_LABEL:

  return(ret_code);
}

/******************************************************************************/
/* Function: free_root                                                        */
/*                                                                            */
/* Returns: Nothing                                                           */
/*                                                                            */
/* Parameters: IN     root - The root to be freed.                            */
/*                                                                            */
/* Operation: First free the array of coefficients and then the root so that  */
/*            all memory is given back to the system. This should be called   */
/*            whenever a root is not going to be used again.                  */
/******************************************************************************/
void free_root(ROOT *root)
{
  /****************************************************************************/
  /* Free all the memory associated with this root but not the roots it       */
  /* points to.                                                               */
  /****************************************************************************/
  free(root->coefficients);
  free(root->next_roots);
  free(root);

  return;
}

/******************************************************************************/
/* Function: init_root_table_element                                          */
/*                                                                            */
/* Returns: One of INIT_ELEMENT_RET_CODES.                                    */
/*                                                                            */
/* Parameters: OUT    element - The element to be initialised.                */
/*             IN     num_generators - The number of generators in the group. */
/*                                                                            */
/* Operation: Allocate memory for each of the structures and set the list     */
/*            pointer to the next element to NULL.                            */
/******************************************************************************/
int init_root_table_element(ROOT_TABLE_ELEMENT **element)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ret_code = INIT_ELEMENT_OK;
  int ret_val;

  /****************************************************************************/
  /* Allocate the necessary amount of memory.                                 */
  /****************************************************************************/
  (*element) = (ROOT_TABLE_ELEMENT *) malloc(sizeof(ROOT_TABLE_ELEMENT));
  if ((*element) == NULL)
  {
    ret_code = INIT_ELEMENT_MEM_ERR;
    goto EXIT_LABEL;
  }

  /****************************************************************************/
  /* Set the next pointers to NULL initially.                                 */
  /****************************************************************************/
  (*element)->next = NULL;

EXIT_LABEL:

  return(ret_code);
}

/******************************************************************************/
/* Function: free_root_table_element                                          */
/*                                                                            */
/* Returns: Nothing                                                           */
/*                                                                            */
/* Parameters: IN     root_table_element - The object to be freed.            */
/*                                                                            */
/* Operation: In order to avoid unwanted side effects only the table element  */
/*            is freed by this function. A call should be made to free_root   */
/*            before this if the root is no longer in use as well.            */
/******************************************************************************/
void free_root_table_element(ROOT_TABLE_ELEMENT *root_table_element)
{
  /****************************************************************************/
  /* Free the object itself.                                                  */
  /****************************************************************************/
  free(root_table_element);

  return;
}

/******************************************************************************/
/* Function: init_root_table                                                  */
/*                                                                            */
/* Returns: One of INIT_ROOT_TABLE_RET_CODES.                                 */
/*                                                                            */
/* Parameters: IN     root_table - Will be returned with memory allocated and */
/*                                 preset to zero.                            */
/*                                                                            */
/* Operation: Allocate memory and fail if error.                              */
/******************************************************************************/
int init_root_table(ROOT_TABLE **root_table)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ret_code = INIT_ROOT_TABLE_OK;

  /****************************************************************************/
  /* Allocate the required amount of memory for the root table object setting */
  /* it to zero initally so that the length is zero and the first root in the */
  /* list is NULL.                                                            */
  /****************************************************************************/
  (*root_table) = (ROOT_TABLE *) calloc(1, sizeof(ROOT_TABLE));
  if (*root_table == NULL)
  {
    ret_code = INIT_ROOT_TABLE_MEM_ERR;
    goto EXIT_LABEL;
  }

EXIT_LABEL:

  return(ret_code);
}

/******************************************************************************/
/* Function: free_root_table                                                  */
/*                                                                            */
/* Returns: Nothing.                                                          */
/*                                                                            */
/* Parameters: IN     root_table - The table of roots to be freed.            */
/*             IN     not_roots - Set to true if only the table elements are  */
/*                                to be freed and false if the roots          */
/*                                themselves should be freed.                 */
/*                                                                            */
/* Operation: Loop through the table freeing roots and then the elements.     */
/******************************************************************************/
void free_root_table(ROOT_TABLE *root_table, bool not_roots)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  ROOT_TABLE_ELEMENT *curr_element = root_table->first;
  ROOT_TABLE_ELEMENT *next_element = NULL;

  /****************************************************************************/
  /* Loop through until there are no more elements freeing the memory for the */
  /* underlying root (if it exists) and then the element itself.              */
  /****************************************************************************/
  while(curr_element != NULL)
  {
    next_element = curr_element->next;

    if (!not_roots)
    {
      free_root(curr_element->root);
    }
    free_root_table_element(curr_element);

    curr_element = next_element;
  }

  free(root_table);

  return;
}

/******************************************************************************/
/* Function: compare_roots                                                    */
/*                                                                            */
/* Returns: 1 if a is bigger than b.                                          */
/*          0 if they are identical in coefficient values.                    */
/*          -1 if b is bigger than a.                                         */
/*                                                                            */
/* Parameters: IN    a - The first root.                                      */
/*             IN    b - The second root.                                     */
/*             IN    num_generators - The number of generators in the group.  */
/*                                                                            */
/* Operation: Loop through each of the coefficients. On the first that is     */
/*            different check which root is larger as in comment below.       */
/*            Two roots are considered the same if they differ by less than a */
/*            small constant. This is to account for rounding errors.         */
/******************************************************************************/
int compare_roots(ROOT *a, ROOT *b, int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int result;
  int ii = 0;

  /****************************************************************************/
  /* Check input parameters. If any are invalid then fail program as these    */
  /* can be controlled in code and are not user dependent.                    */
  /****************************************************************************/
  assert(a != NULL);
  assert(a->coefficients != NULL);
  assert(b != NULL);
  assert(b->coefficients != NULL);
  assert(num_generators > 0);

  /****************************************************************************/
  /* Go through each coefficient in turn comparing a to b. For the first      */
  /* coefficient that is different; if a is larger than b set result to 1, if */
  /* b is larger than a then set result to -1. If no difference is found then */
  /* set result to 0.                                                         */
  /****************************************************************************/
  while((ii < num_generators) && (fabs(a->coefficients[ii] - b->coefficients[ii])
                                                            < EPSILON_COMP_VAL))
  {
    ii++;
  }

  if (ii == num_generators)
  {
    result = COMPARE_ROOTS_EQUAL;
  }
  else if (a->coefficients[ii] > b->coefficients[ii])
  {
    result = COMPARE_ROOTS_GREATER;
  }
  else
  {
    result = COMPARE_ROOTS_SMALLER;
  }

  return(result);
}

/******************************************************************************/
/* Function: insert_in_table                                                  */
/*                                                                            */
/* Returns: One of INSERT_IN_TABLE_ERR_CODES                                  */
/*                                                                            */
/* Parameters: IN/OUT element - The element to be added to the table.         */
/*             IN/OUT first - The first element in the table. NULL for an     */
/*                            empty table.                                    */
/*             IN     num_generators - The number of generators in the group. */
/*                                                                            */
/* Operation: If first is NULL then set the new element to be the first.      */
/*            Otherwise, scan through the ordered list and insert the element */
/*            where it is larger then everything below it according to the    */
/*            compare_roots function.                                         */
/*            If it is already in the list then this function does nothing    */
/*            but set the element to NULL so that the calling function can    */
/*            handle things.                                                  */
/******************************************************************************/
int insert_in_table(ROOT_TABLE_ELEMENT **element,
                    ROOT_TABLE **table,
                    int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ret_code = INSERT_IN_TABLE_OK;
  int compare_val;
  int ret_val;
  ROOT_TABLE_ELEMENT *curr;
  ROOT_TABLE_ELEMENT *prev = NULL;

  /****************************************************************************/
  /* If the table itself is NULL then it must be created and then the new     */
  /* element can simply be added as the first element.                        */
  /****************************************************************************/
  if (*table == NULL)
  {
    ret_val = init_root_table(table);
    if (ret_val != INIT_ROOT_TABLE_OK)
    {
      if (ret_val == INIT_ROOT_TABLE_MEM_ERR)
      {
        ret_code = INSERT_IN_TABLE_MEM_ERR;
        goto EXIT_LABEL;
      }
      else
      {
        printf("An unhandled error occured creating a root table.\n");
      }
    }
  }

  /****************************************************************************/
  /* If the first element is NULL then this is the empty table and we simply  */
  /* set the new element to be the first.                                     */
  /****************************************************************************/
  if ((*table)->first == NULL)
  {
    (*table)->first = *element;
    (*table)->length++;
  }
  else
  {
    curr = (*table)->first;
    compare_val = compare_roots(curr->root, (*element)->root, num_generators);

    /**************************************************************************/
    /* Loop through until the root is larger than one in the list or we reach */
    /* the end of the list.                                                   */
    /**************************************************************************/
    while ((compare_val < 0) && (curr->next != NULL))
    {
      prev = curr;
      curr = curr->next;
      compare_val = compare_roots(curr->root, (*element)->root, num_generators);
    }

    if (compare_val == 0)
    {
      /************************************************************************/
      /* This branch is used if the root already exists in the table. If so   */
      /* then set the return code for the function to inform the calling      */
      /* function.                                                            */
      /************************************************************************/
      ret_code = INSERT_IN_TABLE_ROOT_EXISTS;
    }
    else if ((curr->next == NULL) && (compare_val < 0))
    {
      /************************************************************************/
      /* This branch is used if the root is being added to the bottom of the  */
      /* list.                                                                */
      /************************************************************************/
      curr->next = (*element);
      (*table)->length++;
    }
    else
    {
      /************************************************************************/
      /* This branch is used if the new element needs to be inserted in the   */
      /* list somewhere which is not the bottom.                              */
      /************************************************************************/
      (*element)->next = curr;
      (*table)->length++;
      if (prev != NULL)
      {
        prev->next = *element;
      }
      else
      {
        (*table)->first = *element;
      }
    }
  }

EXIT_LABEL:

  return(ret_code);
}

/******************************************************************************/
/* Function: root_in_list                                                     */
/*                                                                            */
/* Returns: true if the root is in the list and false if not.                 */
/*                                                                            */
/* Parameters: IN     first - The first element in the list. Can be NULL.     */
/*             IN     root - The root that is being searched for.             */
/*             OUT    existing_root - If the root is in the list then this is */
/*                                    the one which is found.                 */
/*             IN     num_generators - The number of group generators.        */
/*                                                                            */
/* Operation: Iterate through the list checking each root against the one     */
/*            passed in.                                                      */
/******************************************************************************/
bool root_in_list(ROOT_TABLE *table,
                  ROOT *root,
                  ROOT **existing_root,
                  int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  ROOT_TABLE_ELEMENT *current = table->first;
  bool is_in_list = false;
  int result;
  
  /****************************************************************************/
  /* Iterate through the list checking whether the current element is the one */
  /* which we are looking for. If it is then return true.                     */
  /****************************************************************************/
  while (current != NULL)
  {
    /**************************************************************************/
    /* Check the current root against the one passed in to the function.      */
    /**************************************************************************/
    result = compare_roots(current->root, root, num_generators);
    if (result == 0)
    {
      is_in_list = true;
      (*existing_root) = current->root;
      goto EXIT_LABEL;
    }

    current = current->next;
  }

EXIT_LABEL:

  return(is_in_list);
}

/******************************************************************************/
/* Function: generate_root_table                                              */
/*                                                                            */
/* Returns: One of GENERATE_ROOT_TABLE_RET_CODES.                             */
/*                                                                            */
/* Parameters: IN     matrix_data - Precalculated group information.          */
/*                                                                            */
/* Operation:                                                                 */
/******************************************************************************/
int generate_root_table(MATRIX_DATA *matrix_data,
                        ROOT_TABLE **root_table,
                        ROOT_TABLE **minimal_root_table,
                        int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ret_code = GENERATE_ROOT_TABLE_OK;
  int ii;
  int ret_val;
  int ret_val_next_root;
  int ret_val_insert_in_table;
  ROOT *simple_root;
  ROOT *existing_simple_root;
  ROOT_TABLE_ELEMENT *simple_root_element;
  ROOT_TABLE_ELEMENT *simple_root_element_minimal;

  /****************************************************************************/
  /* Fill the scalar product matrix if necessary.                             */
  /****************************************************************************/
  if (matrix_data->scalar_products == NULL)
  {
    ret_val = fill_scalar_product_matrix(matrix_data, num_generators);
    if (ret_val != FILL_SCALAR_PRODUCT_MATRIX_OK)
    {
      if (ret_val == FILL_SCALAR_PRODUCT_MATRIX_MEM_ERR)
      {
        printf("There was an error allocating memory for matrix of scalar products.\n");
        ret_code = GENERATE_ROOT_TABLE_MEM_ERR;
        goto EXIT_LABEL;
      }
      else
      {
        printf("Unhandled error creating matrix of scalar products.\n");
      }
    }
  }

  /****************************************************************************/
  /* Fill the matrix of simple root action.                                   */
  /****************************************************************************/
  if (matrix_data->simple_action_results == NULL)
  {
    ret_val = fill_cox_action_matrix(matrix_data, num_generators);
    if (ret_val != FILL_COX_ACTION_MATRIX_OK)
    {
      if (ret_val == FILL_COX_ACTION_MATRIX_MEM_ERR)
      {
        printf("There was an error allocating memory for matrix of coxeter actions.\n");
        ret_code = GENERATE_ROOT_TABLE_MEM_ERR;
        goto EXIT_LABEL;
      }
      else
      {
        printf("Unhandled error creating matrix of coxeter actions.\n");
      }
    }
  }

  /****************************************************************************/
  /* Create the root table objects with initial length 0.                     */
  /****************************************************************************/
  ret_val = init_root_table(root_table);
  if (ret_val != INIT_ROOT_TABLE_OK)
  {
    if (ret_val == INIT_ROOT_TABLE_MEM_ERR)
    {
      printf("There was an error allocating memory for the root table.\n");
      ret_code = GENERATE_ROOT_TABLE_MEM_ERR;
      goto EXIT_LABEL;
    }
    else
    {
      printf("There was an unhandled exception allocating memory for the root table.\n");
    }
  }
  ret_val = init_root_table(minimal_root_table);
  if (ret_val != INIT_ROOT_TABLE_OK)
  {
    if (ret_val == INIT_ROOT_TABLE_MEM_ERR)
    {
      printf("There was an error allocating memory for the minimal root table.\n");
      ret_code = GENERATE_ROOT_TABLE_MEM_ERR;
      goto EXIT_LABEL;
    }
    else
    {
      printf("There was an unhandled exception allocating memory for the minimal root table.\n");
    }
  }

  /****************************************************************************/
  /* Loop through each of the simple roots creating a root for each of them.  */
  /****************************************************************************/
  for (ii = 0; ii < num_generators; ii++)
  {
    /**************************************************************************/
    /* Create a root which will be the simple root for this generator.        */
    /**************************************************************************/
    ret_val = init_root(num_generators, &simple_root);
    if (ret_val != INIT_ROOT_OK)
    {
      if (ret_val == INIT_ROOT_MEM_ERR)
      {
        printf("There was a memory error allocation creating simple root.\n");
        ret_code = GENERATE_ROOT_TABLE_MEM_ERR;
        goto EXIT_LABEL;
      }
      else
      {
        printf("An unhandled error occured initialising simple root.\n");
      }
    }
    simple_root->coefficients[ii] = 1;

    if (!root_in_list(*minimal_root_table,
                      simple_root,
                      &existing_simple_root,
                      num_generators))
    {
      /************************************************************************/
      /* Add the simple root to the table of simple roots in the matrix data. */
      /************************************************************************/
      matrix_data->simple_roots[ii] = simple_root;

      /************************************************************************/
      /* Create a table element for the simple root.                          */
      /************************************************************************/
      ret_val = init_root_table_element(&simple_root_element);
      if (ret_val != INIT_ELEMENT_OK)
      {
        if (ret_val == INIT_ELEMENT_MEM_ERR)
        {
          printf("There was a memory allocation error creating simple root element.\n");
          ret_code = GENERATE_ROOT_TABLE_MEM_ERR;
          goto EXIT_LABEL;
        }
        else
        {
          printf("An unhandled error occured creating simple root table element.\n");
        }
      }
      simple_root_element->root = simple_root;

      /************************************************************************/
      /* Add the simple root into the table of roots.                         */
      /************************************************************************/
      ret_val = insert_in_table(&simple_root_element,
                                root_table,
                                num_generators);
      if (ret_val != INSERT_IN_TABLE_OK)
      {
        if (ret_val == INSERT_IN_TABLE_MEM_ERR)
        {
          printf("A memory allocation error occured adding simple root to table of roots.\n");
          ret_code = GENERATE_ROOT_TABLE_MEM_ERR;
          goto EXIT_LABEL;
        }
        else if (ret_val == INSERT_IN_TABLE_ROOT_EXISTS)
        {
          free_root_table_element(simple_root_element);
        }
      }

      /************************************************************************/
      /* Create a table element for the simple root in the minimum root table.*/
      /************************************************************************/
      ret_val = init_root_table_element(&simple_root_element_minimal);
      if (ret_val != INIT_ELEMENT_OK)
      {
        if (ret_val == INIT_ELEMENT_MEM_ERR)
        {
          printf("There was a memory allocation error creating simple root element.\n");
          ret_code = GENERATE_ROOT_TABLE_MEM_ERR;
          goto EXIT_LABEL;
        }
        else
        {
          printf("An unhandled error occured creating simple root table element.\n");
        }
      }
      simple_root_element_minimal->root = simple_root;

      /************************************************************************/
      /* Check the return value. If the root was not already in the table then*/
      /* call generate_next_root for each generator.                          */
      /************************************************************************/
      if (ret_val == INSERT_IN_TABLE_OK)
      {
        /**********************************************************************/
        /* Add the simple root into the table of minimal roots.               */
        /**********************************************************************/
        ret_val_insert_in_table = insert_in_table(&simple_root_element_minimal,
                                                  minimal_root_table,
                                                  num_generators);
        if (ret_val_insert_in_table != INSERT_IN_TABLE_OK)
        {
          if (ret_val_insert_in_table == INSERT_IN_TABLE_MEM_ERR)
          {
            printf("A memory allocation error occured adding simple root to table of minimum roots.\n");
            ret_code = GENERATE_ROOT_TABLE_MEM_ERR;
            goto EXIT_LABEL;
          }
          else if (ret_val_insert_in_table == INSERT_IN_TABLE_ROOT_EXISTS)
          {
            /******************************************************************/
            /* Branch left for debugging use.                                 */
            /******************************************************************/
            free_root_table_element(simple_root_element_minimal);
          }
          else
          {
            printf("An unhandled exception occured when inserting simple root into minimum root table.\n");
          }
        }

        /**********************************************************************/
        /* Generate the next roots from the current simple root.              */
        /**********************************************************************/
        ret_val_next_root = generate_next_root(matrix_data,
                                               num_generators,
                                               simple_root,
                                               root_table,
                                               minimal_root_table);
        if (ret_val_next_root != GENERATE_NEXT_ROOT_OK)
        {
          if (ret_val == GENERATE_NEXT_ROOT_MEM_ERR)
          {
            printf("Memory error during next root generation.\n");
            ret_code = GENERATE_ROOT_TABLE_MEM_ERR;
            goto EXIT_LABEL;
          }
          else
          {
            printf("There was an unhandled error generating the next root.\n");
          }
        }
      }
      else if (ret_val == INSERT_IN_TABLE_MEM_ERR)
      {
        printf("There was a memory allocation error adding a simple root to the root table.\n");
        ret_code = GENERATE_ROOT_TABLE_MEM_ERR;
        goto EXIT_LABEL;
      }
      else if (ret_val == INSERT_IN_TABLE_ROOT_EXISTS)
      {
        /**********************************************************************/
        /* An empty branch. Left in case any debug information is wanted for  */
        /* roots which already exist in the table.                            */
        /**********************************************************************/
      }
      else
      {
        printf("There was an unhandled error inserting a simple root into the root table.\n");
      }

    }
    else
    {
      /************************************************************************/
      /* Add the existing root that was found to the table of simple roots.   */
      /************************************************************************/
      matrix_data->simple_roots[ii] = existing_simple_root;
      
      /************************************************************************/
      /* Free the memory allocated for the simple root.                       */
      /************************************************************************/
      free_root(simple_root);
    }
  }

EXIT_LABEL:

  return(ret_code);
}

/******************************************************************************/
/* Function: root_positive                                                    */
/*                                                                            */
/* Returns: true if the root is positive or 0 and false otherwise.            */
/*                                                                            */
/* Parameters: IN     root - The root to be checked.                          */
/*             IN     num_generators - The number of generators in the group. */
/*                                                                            */
/* Operation: Check each coefficient in turn until a non-zero one is found.   */
/*            If that is negative return false otherwise return true. A root  */
/*            either has all negative or all positive coefficients.           */
/******************************************************************************/
bool root_positive(ROOT *root, int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ii = 0;

  /****************************************************************************/
  /* Loop through until a non-zero coefficient is found.                      */
  /****************************************************************************/
  while ((ii < num_generators) &&
                              (fabs(root->coefficients[ii]) < EPSILON_COMP_VAL))
  {
    ii++;
  }

  /****************************************************************************/
  /* If the coefficient is less than 0 then return false. Otherwise return    */
  /* true.                                                                    */
  /****************************************************************************/
  if (root->coefficients[ii] < EPSILON_COMP_VAL)
  {
    return(false);
  }
  else
  {
    return(true);
  }
}

/******************************************************************************/
/* Function: generate_next_root                                               */
/*                                                                            */
/* Returns: One of GENERATE_NEXT_ROOT_RET_CODES.                              */
/*                                                                            */
/* Parameters: IN     matrix_data - Pre calculated information on the group.  */
/*             IN     num_generatots - The number of group generators.        */
/*             IN     root - The root from which we are generating the next   */
/*                           set of roots.                                    */
/*             IN     root_table - A table of all calculated roots.           */
/*             IN     minimal_root_table - A table of all roots which are     */
/*                                         positive minimal.                  */
/*                                                                            */
/* Operation:                                                                 */
/******************************************************************************/
int generate_next_root(MATRIX_DATA *matrix_data,
                       int num_generators,
                       ROOT *root,
                       ROOT_TABLE **root_table,
                       ROOT_TABLE **minimal_root_table)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ret_code = GENERATE_NEXT_ROOT_OK;
  int ret_val;
  double dominance_value;
  int ii;
  ROOT *new_root;
  ROOT_TABLE_ELEMENT *new_element;
  ROOT_TABLE_ELEMENT *new_element_minimal;
  bool new_root_exists;

  /****************************************************************************/
  /* Loop through each of the possible next roots (one per generator).        */
  /* For each possibility perform the action of that generator on the root.   */
  /* Check if that is a new root. If it is then store it in the root list and */
  /* call this function again with the new root as a parameter.               */
  /****************************************************************************/
  for (ii = 0; ii < num_generators; ii++)
  {
    ret_val = cox_action_on_root(matrix_data,
                                 num_generators,
                                 ii,
                                 root,
                                 &new_root,
                                 *root_table,
                                 &new_root_exists);
    if (ret_val != COX_ACTION_ON_ROOT_OK)
    {
      if (ret_val == COX_ACTION_ON_ROOT_MEM_ERR ||
                                 ret_val == COX_ACTION_ON_ROOT_MEM_ERR_SUB_FUNC)
      {
        printf("Memory allocation error performing action on root.\n");
        ret_code = GENERATE_NEXT_ROOT_MEM_ERR;
        goto EXIT_LABEL;
      }
      else
      {
        printf("An unhandled error occured whilst performing action on root.\n");
        ret_code = GENERATE_NEXT_ROOT_UNHANDLED_ERR;
        goto EXIT_LABEL;
      }
    }

    if (!new_root_exists)
    {
      /************************************************************************/
      /* Add the new root to the root table regardless of whether it is going */
      /* to be used to generate the state tree. This is so that we can look   */
      /* roots up in this table and save on calculations.                     */
      /************************************************************************/
      /************************************************************************/
      /* Create a new root list element for the new root.                     */
      /************************************************************************/
      ret_val = init_root_table_element(&new_element);
      if (ret_val != INIT_ELEMENT_OK)
      {
        if (ret_val == INIT_ELEMENT_MEM_ERR)
        {
          printf("Memory allocation error creating new root element.\n");
          ret_code = GENERATE_NEXT_ROOT_MEM_ERR;
          goto EXIT_LABEL;
        }
        else
        {
          printf("An unhandled error occured whilst creating new root element.\n");
          ret_code = GENERATE_NEXT_ROOT_UNHANDLED_ERR;
          goto EXIT_LABEL;
        }
      }
      new_element->root = new_root;

      /************************************************************************/
      /* Add the new element to the root table.                               */
      /************************************************************************/
      ret_val = insert_in_table(&new_element, root_table, num_generators);
      if (ret_val != INSERT_IN_TABLE_OK)
      {
        if (ret_val == INSERT_IN_TABLE_ROOT_EXISTS)
        {
          /********************************************************************/
          /* As we have already checked if the root exists this branch is not */
          /* going to be hit. Left for debugging purposes.                    */
          /********************************************************************/
        }
        else if (ret_val == INSERT_IN_TABLE_MEM_ERR)
        {
          printf("A memory error occured adding root to table.\n");
          ret_code = GENERATE_NEXT_ROOT_MEM_ERR;
          goto EXIT_LABEL;
        }
        else
        {
          printf("An unhandled error occured adding root to table.\n");
        }
      }

      /************************************************************************/
      /*                         DOMINANCE CRITERIA                           */
      /* Find the value of ii.root. If this is >= 1 then the new root         */
      /* dominates ii and thus is not a member of the minimum root tree.      */
      /************************************************************************/
      dominance_value = cox_scalar_product_root(matrix_data,
                                                ii,
                                                new_root,
                                                num_generators);

      if (root_positive(new_root, num_generators) &&
                                     (dominance_value < 1.0 - EPSILON_COMP_VAL))
      {
        new_root->positive_minimal = true;
      }
      else
      {
        new_root->positive_minimal = false;
      }

      /************************************************************************/
      /* Only continue if the root created is positive.                       */
      /************************************************************************/
      if (new_root->positive_minimal)
      {
        /**********************************************************************/
        /* Create a new root list element for the new root.                   */
        /**********************************************************************/
        ret_val = init_root_table_element(&new_element_minimal);
        if (ret_val != INIT_ELEMENT_OK)
        {
          if (ret_val == INIT_ELEMENT_MEM_ERR)
          {
            printf("Memory allocation error creating new root element.\n");
            ret_code = GENERATE_NEXT_ROOT_MEM_ERR;
            goto EXIT_LABEL;
          }
          else
          {
            printf("An unhandled error occured whilst creating new root element.\n");
            ret_code = GENERATE_NEXT_ROOT_UNHANDLED_ERR;
            goto EXIT_LABEL;
          }
        }
        new_element_minimal->root = new_root;

        /**********************************************************************/
        /* Add the new root to the minimal root table.                        */
        /**********************************************************************/
        ret_val = insert_in_table(&new_element_minimal,
                                  minimal_root_table,
                                  num_generators);
        if (ret_val != INSERT_IN_TABLE_OK)
        {
          if (ret_val == INSERT_IN_TABLE_MEM_ERR)
          {
            printf("There was a memory allocation error attmepting to insert root into table.\n");
            ret_code = GENERATE_NEXT_ROOT_MEM_ERR;
            goto EXIT_LABEL;
          }
          else if (ret_val == INSERT_IN_TABLE_ROOT_EXISTS)
          {
            /******************************************************************/
            /* We have already checked whether the root exists so this branch */
            /* should never be reached. Left for debugging purposes.          */
            /******************************************************************/
          }
          else
          {
            printf("There was an unhandled error adding root to table.\n");
            ret_code = GENERATE_NEXT_ROOT_UNHANDLED_ERR;
            goto EXIT_LABEL;
          }
        }

        /**********************************************************************/
        /* We only need to continue generating new roots if the current one   */
        /* was not already in the root table (as otherwise it will already    */
        /* have been processed).                                              */
        /**********************************************************************/
        generate_next_root(matrix_data,
                           num_generators,
                           new_root,
                           root_table,
                           minimal_root_table);
      }
    }
  }

EXIT_LABEL:

  return(ret_code);
}

/******************************************************************************/
/* Function:                                                                  */
/*                                                                            */
/* Returns:                                                                   */
/*                                                                            */
/* Parameters:                                                                */
/*                                                                            */
/* Operation:                                                                 */
/******************************************************************************/
int output_root_table(FILE *output_file,
                      ROOT_TABLE *root_table,
                      int num_generators)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int ret_code = OUTPUT_ROOT_TABLE_OK;
  int ii;
  int ret_val;
  ROOT_TABLE_ELEMENT *curr = root_table->first;
  char output_line[MAX_ROOT_OUTPUT_LENGTH];
  char temp_line[MAX_ROOT_OUTPUT_LENGTH];

  /****************************************************************************/
  /* Loop through the table until we hit a NULL pointer printing out each     */
  /* root as we go.                                                           */
  /****************************************************************************/
  while (curr != NULL)
  {
    snprintf(output_line,
             MAX_ROOT_OUTPUT_LENGTH,
             "%f",
             curr->root->coefficients[0]);

    /**************************************************************************/
    /* Print out the root as a string of the form: n_1*a + n_2*b...+ n_m*x    */
    /**************************************************************************/
    for (ii = 1; ii < num_generators; ii++)
    {
      strncat(output_line, " + ", 3);
      snprintf(temp_line,
               MAX_ROOT_OUTPUT_LENGTH,
               "%f",
               curr->root->coefficients[ii]);
      strncat(output_line, temp_line, strlen(temp_line));
    }

    strncat(output_line, "\n", 1);

    /**************************************************************************/
    /* Output the line to the file pointer passed in.                         */
    /**************************************************************************/
    ret_val = fprintf(output_file, output_line);
    if (ret_val != strlen(output_line))
    {
      if (ret_val < 0)
      {
        ret_code = OUTPUT_ROOT_TABLE_FILE_FAILURE;
        goto EXIT_LABEL;
      }
      else
      {
        printf("Only outputted %i bytes from root.\n", ret_val);
      }
    }

    curr = curr->next;
  }

EXIT_LABEL:

  return(ret_code);
}
