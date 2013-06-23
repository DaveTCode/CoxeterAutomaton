#include "cox_prot.h"

/******************************************************************************/
/* Function: flush_stdin                                                      */
/*                                                                            */
/* Returns:  Nothing.                                                         */
/*                                                                            */
/* Parameters: None.                                                          */
/*                                                                            */
/* Operation: Read characters from stdin one at a time until a new line char, */
/*            a NULL character or an EOF character are found.                 */
/******************************************************************************/
void flush_stdin (void)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  int character;
  
  /****************************************************************************/
  /* Read characters in one at a time until either a newline character, a     */
  /* character or an EOF character are found.                                 */
  /****************************************************************************/
  do
  {
    character = fgetc(stdin);
  } while (character != '\n' && character != EOF);
  
  return;
}

/******************************************************************************/
/* Function: input_string                                                     */
/*                                                                            */
/* Returns:  One of STRING_INPUT_ERR_CODE.                                    */
/*                                                                            */
/* Parameters: IN     max_string_size - The maximum buffer size for the input.*/
/*             OUT    user_input - A pointer to a string with the user typed  */
/*                                 input.                                     */
/*                                                                            */
/* Operation: Retrieve input from the user and check that it is not too long. */
/*            Allocate the requried memory to the return variable and then    */
/*            copy the user typed string into the output variable.            */
/******************************************************************************/
int input_string(int max_string_size, char **user_input)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  char input[max_string_size];
  int ret_code = STRING_INPUT_OK;
  
  /****************************************************************************/
  /* Get input from the user and check it does not overrun the maximum input  */
  /* size.                                                                    */
  /****************************************************************************/
  fgets(input, max_string_size, stdin);
  if (input[strlen(input) - 1] != '\n')
  {
    flush_stdin();
    printf("The input entered was longer than the maximum size allowed.\n");
    ret_code = STRING_INPUT_INVALID;
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* Allocate the requried amount of memory to the user_input variable. This  */
  /* is simply going to be the length of the input string.                    */
  /****************************************************************************/
  (*user_input) = (char *) malloc(strlen(input));
  if ((*user_input) == NULL)
  {
    printf("Memory could not be allocated for the string.\n");
    ret_code = STRING_INPUT_MEM_ERR;
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* Copy the input string into the user_input variable so that it can be     */
  /* can be returned to the calling function. It must also be null terminated */
  /* as fgets puts a new line character at the end of the input.              */
  /****************************************************************************/
  strncpy((*user_input), input, strlen(input));
  (*user_input)[strlen(input) - 1] = '\0';
  
EXIT_LABEL:
  
  return(ret_code);
}

/******************************************************************************/
/* Function: user_input_word                                                  */
/*                                                                            */
/* Returns: One of WORD_INPUT_RET_CODES.                                      */
/*                                                                            */
/* Parameters: IN     grp_max_generators - The number of generators in the    */
/*                                         group is used to make sure that    */
/*                                         only valid characters are allowed. */
/*             IN     grp_max_word_len - The maximum length of a word.        */
/*                                                                            */
/* Operation: The parsing algorithm used is rather complicated and makes use  */
/*            of a stack of strings. Essentially, each time an open bracket   */
/*            is found we push the current string onto the stack.             */
/*            Each time a close bracket is found we pop the stack to get the  */
/*            last string and add exponent number of copies of the current    */
/*            string to it. This is robust and tests that the string does not */
/*            get too long.                                                   */
/******************************************************************************/
int user_input_word(int grp_max_generators,
                    int grp_max_word_len,
                    char **word)
{
  /****************************************************************************/
  /* Local variables.                                                         */
  /****************************************************************************/
  char subword[grp_max_word_len];
  int subword_index = 0;
  char *popped_word;
  int ii;
  int jj;
  int input_ret_code;
  int exponent;
  char exponent_string[grp_max_word_len];
  int exp_string_index = 0;
  char *user_input;
  int ret_code = WORD_INPUT_OK;
  bool parsing_exponent = false;
  bool hat_found = false;
  bool valid_exp = false;
  STRING_STACK_ELEMENT *top = NULL;
  int nest_depth = 0;
  
  /****************************************************************************/
  /* Check input parameters.                                                  */
  /****************************************************************************/
  assert(grp_max_word_len < MAX_WORD_LEN);
  assert(grp_max_generators < MAX_GENERATORS);
  
  /****************************************************************************/
  /* Ask the user for input and flush the output buffer to make sure the user */
  /* sees the request.                                                        */
  /****************************************************************************/
  printf("Enter a word for the group loaded.\n");
  fflush(stdout);
  
  /****************************************************************************/
  /* Call the input_string method to retrieve a string containing the word.   */
  /****************************************************************************/
  input_ret_code = input_string(MAX_WORD_LEN, &user_input);
  if (input_ret_code != STRING_INPUT_OK)
  {
    ret_code = WORD_INPUT_INVALID;
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* If the input string is longer than the maximum word size then return.    */
  /****************************************************************************/
  if (strlen(user_input) > grp_max_word_len)
  {
    printf("The word inputted is too long.\n");
    free(user_input);
    ret_code = WORD_INPUT_TOO_LONG;
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* Set the first character in the subword to be NULL to start. This means   */
  /* that even if there are no characters in the user input subword contains  */
  /* a well defined string.                                                   */
  /****************************************************************************/
  subword[0] = '\0';
  
  /****************************************************************************/
  /* Go through the word one character at a time parsing it.                  */
  /****************************************************************************/
  for (ii = 0; ii < strlen(user_input); ii++)
  {
    if (parsing_exponent)
    {
      if (((int) user_input[ii]) == (int) '^')
      {
        /**********************************************************************/
        /* The first time that ^ is found after a bracket we can start        */
        /* looking for digits.                                                */
        /**********************************************************************/
        hat_found = true;
      }
      else if ((((int) user_input[ii]) >= (int) '0') &&
               (((int) user_input[ii]) <= (int) '9'))
      {
        /**********************************************************************/
        /* Exponents must be of the form ^x where x is a digit. If the ^ is   */
        /* not found then it is not a valid exponent and the word fails.      */
        /**********************************************************************/
        if (!hat_found)
        {
          printf("The word was invalid as a close brack was not followed by ^.\n");
          ret_code = WORD_INPUT_INVALID;
          empty_string_stack(top);
          free(user_input);
          goto EXIT_LABEL;
        }
        
        /**********************************************************************/
        /* As soon as a digit is found after a ^ we have a valid exponent.    */
        /**********************************************************************/
        valid_exp = true;
        
        /**********************************************************************/
        /* We generate a substring which contains the exponent.               */
        /**********************************************************************/
        exponent_string[exp_string_index] = user_input[ii];
        exp_string_index++;
      }
      else if ((((int) user_input[ii]) >= (int)'a' && 
                ((int) user_input[ii]) <= (int)'a' + grp_max_generators - 1) ||
               (((int) user_input[ii]) == (int) '(') ||
               (((int) user_input[ii]) == (int) ')'))
      {
        /**********************************************************************/
        /* It is only valid that a non-exponent valid character was found if  */
        /* the exponent found so far is valid. If not then the word fails.    */
        /**********************************************************************/
        if (!valid_exp)
        {
          printf("A valid exponent was not found because there were no digits after a ^ character.\n");
          ret_code = WORD_INPUT_INVALID;
          empty_string_stack(top);
          free(user_input);
          goto EXIT_LABEL;
        }
        
        /**********************************************************************/
        /* At this point we know we have a valid exponent so we turn it into  */
        /* a number.                                                          */
        /**********************************************************************/
        exponent_string[exp_string_index] = '\0';
        exponent = atoi(exponent_string);
        popped_word = string_stack_pop(&top);
        
        /**********************************************************************/
        /* Add exponent many copies of the current subword to the word popped */
        /* off the stack.                                                     */
        /* The result is the new subword.                                     */
        /**********************************************************************/
        for (jj = 0; jj < exponent; jj++)
        {
          strncat(popped_word, subword, strlen(subword));
        }
        strncpy(subword, popped_word, strlen(popped_word));
        subword[strlen(popped_word)] = '\0';
        free(popped_word);
                
        /**********************************************************************/
        /* Return to parsing the word normally.                               */
        /**********************************************************************/
        parsing_exponent = false;
        hat_found = false;
        valid_exp = false;
        exp_string_index = 0;
        subword_index = strlen(subword);
      }
      else
      {
        printf("An invalid character was found (%c) whilst parsing exponent.\n",
               user_input[ii]);
      }
    }
    
    if (!parsing_exponent)
    {
      if (((int) user_input[ii]) >= (int)'a' && 
          ((int) user_input[ii]) <= (int)'a' + grp_max_generators - 1)
      {
        subword[subword_index] = user_input[ii];
        subword_index++;
        
        /**********************************************************************/
        /* The next character in the current subword should always be set to  */
        /* a null character in case there are no more brackets.               */
        /**********************************************************************/
        subword[subword_index] = '\0';
      }
      else if (((int) user_input[ii]) == (int) '(')
      {
        /**********************************************************************/
        /* When we hit an open bracket we put the current string onto the     */
        /* stack and start again with an empty string.                        */
        /**********************************************************************/
        string_stack_push(subword, &top);
        subword_index = 0;
        
        /**********************************************************************/
        /* To keep a track of the number of brackets opened we increment a    */
        /* depth counter.                                                     */
        /**********************************************************************/
        nest_depth++;
      }
      else if (((int) user_input[ii]) == (int) ')')
      {
        /**********************************************************************/
        /* If the nest depth counter is not greater than 0 then there are no  */
        /* brackets to be closed and the word is not valid.                   */
        /**********************************************************************/
        if (nest_depth <= 0)
        {
          printf("A bracket was closed without being opened.\n");
          ret_code = WORD_INPUT_INVALID;
          empty_string_stack(top);
          free(user_input);
          goto EXIT_LABEL;
        }
        
        /**********************************************************************/
        /* When a close bracket is hit we insist that the next few characters */
        /* are in a particular form. To force this we set a boolean which     */
        /* tells the program that it is parsing an exponent until it stops.   */
        /**********************************************************************/
        parsing_exponent = true;
        subword_index = 0;
        nest_depth--;
      }
      else
      {
        printf("There is an invalid character (%c) in the word.\n", 
               user_input[ii]);
        ret_code = WORD_INPUT_INVALID;
        empty_string_stack(top);
        free(user_input);
        goto EXIT_LABEL;
      }
    }
  }
  
  /****************************************************************************/
  /* After the end of the word has been found the nest depth should be zero   */
  /* indicating that all brackets have been closed.                           */
  /****************************************************************************/
  if (nest_depth != 0)
  {
    printf("More brackets were opened than were closed in the word.\n");
    ret_code = WORD_INPUT_INVALID;
    empty_string_stack(top);
    free(user_input);
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* If the string ends whilst still parsing the exponent but without having  */
  /* finished then the exponent is not valid and therefore the word isn't.    */
  /* e.g. w = abd(ca)^ is not a valid word and will hit this branch.          */
  /****************************************************************************/
  if (parsing_exponent && !valid_exp)
  {
    printf("The word ended without a complete exponent.\n");
    ret_code = WORD_INPUT_INVALID;
    empty_string_stack(top);
    free(user_input);
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* If the last character in the user input string was an exponent then we   */
  /* deal with it by popping the last string off the stack and adding the     */
  /* current subword as usual.                                                */
  /****************************************************************************/
  if (valid_exp)
  {
    exponent_string[exp_string_index] = '\0';
    exponent = atoi(exponent_string);
    popped_word = string_stack_pop(&top);
    
    /**************************************************************************/
    /* Add exponent many copies of the current subword to the word popped off */
    /* the stack.                                                             */
    /* The result is the new subword.                                         */
    /**************************************************************************/
    for (jj = 0; jj < exponent; jj++)
    {
      strncat(popped_word, subword, strlen(subword));
    }
    strncpy(subword, popped_word, strlen(popped_word) + 1);
    free(popped_word);
  }
  
  /****************************************************************************/
  /* Allocate the required amount of memory for the returned word. Then put   */
  /* the parsed word in to the new memory and return it.                      */
  /****************************************************************************/
  (*word) = (char *) malloc(sizeof(char) * strlen(subword) + 1);
  if (word == NULL)
  {
    printf("Memory allocation error creating word.");
    ret_code = WORD_INPUT_MEM_FAIL;
    goto EXIT_LABEL;
  }
  strncpy(*word, subword, strlen(subword) + 1);
  
  free(user_input);

EXIT_LABEL:
  
  return(ret_code);
}

/******************************************************************************/
/* Function: user_input_file                                                  */
/*                                                                            */
/* Returns:  One of FILE_INPUT_ERR_CODE.                                      */
/*                                                                            */
/* Parameters: OUT    input_file - The file referred to by the user typed     */
/*                                 filename.                                  */
/*             IN     mode - 'w' or 'r' depending on whether the file is to   */
/*                           be used for input or output.                     */
/*                                                                            */
/* Operation: Read characters from stdin one at a time until a new line char, */
/*            a NULL character or an EOF character are found.                 */
/*            The calling function MUST close the file when it is done.       */
/******************************************************************************/
int user_input_file(char **input_filename)
{
  /****************************************************************************/
  /* Local Variables.                                                         */
  /****************************************************************************/
  char *user_input;
  int ret_code = FILE_INPUT_OK;
  int input_ret_code;
  FILE *temp_file_ptr;
  
  /****************************************************************************/
  /* As the user for input and flush the output buffer.                       */
  /****************************************************************************/
  printf("Please enter a filename:\n");
  fflush(stdout);
  
  /****************************************************************************/
  /* Call the input_string function to retrieve a user inputted string.       */
  /****************************************************************************/
  input_ret_code = input_string(MAX_FILENAME_LEN, &user_input);
  if (input_ret_code != STRING_INPUT_OK)
  {
    ret_code = FILE_INPUT_INVALID;
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* Attempt to open the file. If this does not succeed then we do not return */
  /* the filename.                                                            */
  /****************************************************************************/
  temp_file_ptr = fopen(user_input, "r");
  if (temp_file_ptr == NULL)
  {
    printf("File could not be opened.\n");
    free(user_input);
    ret_code = FILE_INPUT_INVALID;
    goto EXIT_LABEL;
  }
  
  /****************************************************************************/
  /* If the file open succeeded then close the file.                          */
  /****************************************************************************/
  fclose(temp_file_ptr);
  
  /****************************************************************************/
  /* Set the output variable to the filename.                                 */
  /****************************************************************************/
  (*input_filename) = (char *) malloc(strlen(user_input) + 1);
  if ((*input_filename) == NULL)
  {
    printf("Failure allocating memory for output filename.\n");
    free(user_input);
    ret_code = FILE_INPUT_MEM_ERR;
    goto EXIT_LABEL;
  }
  strncpy((*input_filename), user_input, strlen(user_input) + 1);
  free(user_input);
  
EXIT_LABEL:
  
  return(ret_code);
}
