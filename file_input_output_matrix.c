#include "cox_prot.h"

/******************************************************************************/
/* Function: load_matrix_from_file                                            */
/*                                                                            */
/* Returns: One of group FILE_INPUT_ERR_CODE.                                 */
/*                                                                            */
/* Params: IN     filename - The filename (including path) of the file        */
/*                           containing the matrix.                           */
/*         IN     max_width - The maximum allowed width for the matrix.       */
/*         IN     max_width - The maximum allowed depth for the matrix.       */
/*         OUT    matrix - The matrix from the file. NULL if an error occurs. */
/*         OUT    matrix_info - The info line structure from the file. Memory */
/*                              for this is allocated in this function.       */
/*                                                                            */
/* Operation:                                                                 */
/******************************************************************************/
int load_matrix_from_file(char *filename,
                          long max_width,
                          long max_depth,
                          long ***matrix,
                          MATRIX_FILE_INFO **matrix_info)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  FILE *group_file;
  char line[MAX_GENERATORS * 2 + 1];
  char *line_part;
  int row;
  int column;
  int ret_code = FILE_INPUT_ERR_NONE;

  /****************************************************************************/
  /* Check input variables have valid values.                                 */
  /****************************************************************************/
  assert(filename != NULL);
  assert(max_width > 0);
  assert(max_depth > 0);

  /****************************************************************************/
  /* Attempt to open the file at filename. If this returns NULL then the file */
  /* could not be opened.                                                     */
  /****************************************************************************/
  group_file = fopen(filename, "r");
  if (group_file == NULL)
  {
    printf("Could not open file %s\n", filename);
    ret_code = FILE_INPUT_ERR_OPEN;
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* We process the file line by line. The first line should contain basic    */
  /* information about the dimensions of the Coxeter matrix and the program   */
  /* version.                                                                 */
  /* Each line thereafter should have n integers seperated by spaces.         */
  /****************************************************************************/
  if (fgets(line, MAX_GENERATORS * 2 + 1, group_file) == NULL)
  {
    printf("The input file was in an incorrect format. Check file.\n");
    ret_code = FILE_INPUT_ERR_INFO_LINE;
    goto EXIT_LABEL;
  }

  /****************************************************************************/
  /* Allocate memory for the matrix information line structure.               */
  /****************************************************************************/
  *matrix_info = (MATRIX_FILE_INFO *) malloc(sizeof(MATRIX_FILE_INFO));
  if (*matrix_info == NULL)
  {
    printf("Memory could not be allocated for the information structure.\n");
    ret_code = FILE_INPUT_ERR_INFO_MEM;
    goto EXIT_LABEL;
  }

  /****************************************************************************/
  /* The format of the first line in the file is as follows:                  */
  /* VERSION_NUM MATRIX_WIDTH MATRIX_DEPTH                                    */
  /* Store those now for future reference. Checking is not done at this level.*/
  /****************************************************************************/
  line_part = strtok(line, " ");
  if (line_part == NULL)
  {
    printf("Version number not found in file.\n");
    ret_code = FILE_INPUT_ERR_INFO_LINE;
    goto EXIT_LABEL;
  }
  else
  {
    (*matrix_info)->version = (int) strtol(line_part, NULL, 0);
  }
  line_part = strtok(NULL, " ");
  if (line_part == NULL)
  {
    printf("Width of matrix not found in file.\n");
    ret_code = FILE_INPUT_ERR_INFO_LINE;
    goto EXIT_LABEL;
  }
  else
  {
    (*matrix_info)->width = strtol(line_part, NULL, 0);
  }
  line_part = strtok(NULL, " ");
  if (line_part == NULL)
  {
    printf("Depth of matrix not found in file.\n");
    ret_code = FILE_INPUT_ERR_INFO_LINE;
    goto EXIT_LABEL;
  }
  else
  {
    (*matrix_info)->depth = strtol(line_part, NULL, 0);
  }
  /****************************************************************************/
  /* We do not check that there are no extra parameters here as there is      */
  /* scope for improving the functionality here somewhat.                     */
  /****************************************************************************/

  /****************************************************************************/
  /* Check that the width and depth of the array fall inside the bounds       */
  /* passed to this function.                                                 */
  /****************************************************************************/
  if ((*matrix_info)->width > max_width || (*matrix_info)->depth > max_depth)
  {
    printf("The matrix in the file is larger than the maximum size.\n");
    ret_code = FILE_INPUT_ERR_MAT_DIM;
    goto EXIT_LABEL;
  }

  /****************************************************************************/
  /* We can now allocate the required amount of memory to the matrix array as */
  /* we know the dimensions.                                                  */
  /****************************************************************************/
  *matrix = (long **) malloc(sizeof(long *) * (*matrix_info)->depth);
  if (*matrix == NULL)
  {
    printf("Could not allocate memory for matrix.\n");
    ret_code = FILE_INPUT_ERR_INFO_MEM;
    goto EXIT_LABEL;
  }

  for (row = 0; row < (*matrix_info)->depth; row++)
  {
    (*matrix)[row] = (long *) malloc(sizeof(long) * (*matrix_info)->width);
    if ((*matrix)[row] == NULL)
    {
      printf("Could not allocate memory for matrix.\n");
      ret_code = FILE_INPUT_ERR_INFO_MEM;
      goto EXIT_LABEL;
    }
  }

  /****************************************************************************/
  /* Each line after the information line contains elements of a matrix.      */
  /****************************************************************************/
  row = 0;
  while (fgets(line, MAX_GENERATORS * 2 + 1, group_file) != NULL)
  {
    /**************************************************************************/
    /* If the number of rows is too large then abort the input routine.       */
    /**************************************************************************/
    if (row == (*matrix_info)->depth)
    {
      printf("There are too many rows in the input file.\n");
      ret_code = FILE_INPUT_ERR_MAT_DATA;
      goto EXIT_LABEL;
    }

    /**************************************************************************/
    /* The elements on a line are seperated by spaces. Convert them to long   */
    /* integers and store them in the matrix.                                 */
    /**************************************************************************/
    line_part = strtok(line, " ");
    column = 0;
    while (line_part != NULL)
    {
      (*matrix)[row][column] = strtol(line_part, NULL, 0);
      column++;
      line_part = strtok(NULL, " ");
    }

    /**************************************************************************/
    /* If one of the rows does not have the full number of entries then abort */
    /* the input routine.                                                     */
    /**************************************************************************/
    if (column < (*matrix_info)->width)
    {
      printf("Row %i in the matrix was the incorrect length.\n", row);
      ret_code = FILE_INPUT_ERR_MAT_DATA;
      goto EXIT_LABEL;
    }

    row++;
  }

  /****************************************************************************/
  /* If there are not enough rows in the matrix then abort the input routine. */
  /****************************************************************************/
  if (row < (*matrix_info)->depth)
  {
    printf("Not enough rows (%i found) in matrix.\n", row);
    ret_code = FILE_INPUT_ERR_MAT_DATA;
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* The file is no longer needed so close it.                                */
  /****************************************************************************/
  fclose(group_file);
  
EXIT_LABEL:

  return(ret_code);
}

/******************************************************************************/
/* Function: free_file_info                                                   */
/*                                                                            */
/* Returns: Nothing.                                                          */
/*                                                                            */
/* Parameters: IN     file_info - The information to be freed.                */
/*                                                                            */
/* Operation:                                                                 */
/******************************************************************************/
void free_file_info(MATRIX_FILE_INFO *file_info)
{
  free(file_info);
  
  return;
}
