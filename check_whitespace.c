#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Strips spaces from both the front and back of a string,
 * leaving any internal spaces alone. Note that this
 * function returns a string which is allocated on the
 * heap and therefore must be freed.
 */
char const *strip(char const *str) {
  int size = strlen(str);

  // This counts the number of leading and trailing spaces
  // so we can figure out how big the result array should be.
  int num_spaces = 0;
  int first_non_space = 0;
  while (first_non_space < size && str[first_non_space] == ' ') {
    ++num_spaces;
    ++first_non_space;
  }

  int last_non_space = size-1;
  while (last_non_space >= 0 && str[last_non_space] == ' ') {
    ++num_spaces;
    --last_non_space;
  }

  // If num_spaces >= size then that means that the string
  // consisted of nothing but spaces, so we'll return the
  // empty string.
  if (num_spaces >= size) {
    // Note that if we just returned an empty string here,
    // we would not be able to say we always return a heap
    // allocated string, so we instead allocate the single
    // byte on the heap which is just the null character.
    char* result = (char*) calloc(1, sizeof(char));
    *result = 0;
    return result;
  }

  // Allocate a slot for all the "saved" characters
  // plus one extra for the null terminator.
  char* result = (char*) calloc(size-num_spaces+1, sizeof(char));

  // Copy in the "saved" characters.
  int i;
  for (i = first_non_space; i <= last_non_space; ++i) {
    result[i-first_non_space] = str[i];
  }
  // Place the null terminator at the end of the result string.
  result[i-first_non_space] = '\0';

  return result;
}

/*
 * Return true (1) if the given string is "clean", i.e., has
 * no spaces at the front or the back of the string.
 */
int is_clean(char const *str) {
  // We check if it's clean by calling strip and seeing if the
  // result is the same as the original string.
  char const *cleaned = strip(str);

  // strcmp compares two strings, returning a negative value if
  // the first is less than the second (in alphabetical order),
  // 0 if they're equal, and a positive value if the first is
  // greater than the second.
  int result = strcmp(str, cleaned);
  
  // The cleaned string was allocated on the heap, so we need to
  // free it.
  free((void*)cleaned);   
 
  return result == 0;
}
