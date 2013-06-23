/******************************************************************************/
/* Defines the maximum length of the string which can be inputted as a word.  */
/* There is no harm in increasing this number within reasonable bounds.       */
/******************************************************************************/
#define MAX_WORD_LEN 200

/******************************************************************************/
/* The maximum number of generators in a group. This is set to 10 as the      */
/* automaton for groups with more than 10 generators is likely to be very     */
/* large and uncomputable. Can be increased or lowered as necessary.          */
/******************************************************************************/
#define MAX_GENERATORS 10

/******************************************************************************/
/* The maximum length of the string which can be inputted as the file name of */
/* the groups matrix. Increasing or decreasing will not affect the code too   */
/* much.                                                                      */
/******************************************************************************/
#define MAX_FILENAME_LEN 200

/******************************************************************************/
/* When printing the root table for the group we limit the length of each     */
/* element using the following constant. There is no harm in increasing this. */
/******************************************************************************/
#define MAX_ROOT_OUTPUT_LENGTH 255

/******************************************************************************/
/* DAT - The maximum nest size in inputted words. If this was set to 2 then   */
/* a(bc(de)^2)^3 would be valid but a(bc(de(cd)^2)^5)^3 would not as the      */
/* first has nest size 2 and the second has nest size 3. This can be made     */
/* larger if necessary, however it is limited to make the word input routine  */
/* smoother.                                                                  */
/******************************************************************************/
#define MAX_NESTED_WORD_LEN 10
