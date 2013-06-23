/******************************************************************************/
/* GROUP: FILE_INPUT_ERR_CODE                                                 */
/*                                                                            */
/* These are the possible error codes that can be returned by the file input  */
/* function when it is attempting to read a matrix.                           */
/******************************************************************************/
#define FILE_INPUT_ERR_NONE      0
#define FILE_INPUT_ERR_OPEN      1
#define FILE_INPUT_ERR_INFO_LINE 2
#define FILE_INPUT_ERR_MAT_DIM   3
#define FILE_INPUT_ERR_INFO_MEM  4
#define FILE_INPUT_ERR_MAT_DATA  5

/******************************************************************************/
/* The structure of the matrix file information. These fields will be stored  */
/* in the first line of the matrix file and are seperated by a space.         */
/******************************************************************************/
typedef struct matrix_file_info
{
  int version;
  long width;
  long depth;
} MATRIX_FILE_INFO;
