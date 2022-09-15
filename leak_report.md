# Leak report

## `check_whitespace`

### Leak Description

The memory leak in this executable occurs on every call to `is_clean`. The allocated memory which stores the result is that which is leaked.

### Correction

There are two corrections which I made in `check_whitespace.c`.

The first is to directly combat the memory leak, looking at the source for `is_clean` reveals that a call to `strip` is made which does not free the memory it returns. As such, the function was edited to store the string which was returned, and then free that memory.

```c
...
  // strcmp compares two strings, returning a negative value if
  // the first is less than the second (in alphabetical order),
  // 0 if they're equal, and a positive value if the first is
  // greater than the second.
  int result = strcmp(str, cleaned);
  
  // The cleaned string was allocated on the heap, so we need to
  // free it.
  free((void*)cleaned);   
...
```

In addition, to prevent such problems in the future, the documentation comment on `strip` is updated to reflect the fact that it returns memory which is allocated on the heap.

```c
...
/*
 * Strips spaces from both the front and back of a string,
 * leaving any internal spaces alone. Note that this
 * function returns a string which is allocated on the
 * heap and therefore must be freed.
 */
char const *strip(char const *str) {
...
```

However, this causes another error to occur, an invalid free. This happens because of the early return in `strip`. It causes the function to return a pointer to static memory initialized with the null string. As such, when `is_clean` was called with an empty string, it would attempt to free this static memory, which produces the error. To prevent this, the `strip` function was updated, with the early return being corrected to properly return memory allocated on the heap.

```c
...
  if (num_spaces >= size) {
    // Note that if we just returned an empty string here,
    // we would not be able to say we always return a heap
    // allocated string, so we instead allocate the single
    // byte on the heap which is just the null character.
    char* result = (char*) calloc(1, sizeof(char));
    *result = 0;
    return result;
  }
...
```

## `check_whitespace_test`

### Leak Description

The memory leak in this executable occurs with every test which tests `strip` except that checking against the empty string. Similarly, it is the memory allocated to store the result which is leaked

### Correction

There are five corrections made in `check_whitespace_test.cpp`, however each is a repeat of the same correction, so only the first will be discussed in detail, the rest are identical to the first.

Each of the tests which call `strip` do so with something resembling

```cpp
TEST(strip, EmptyString) {
    ASSERT_STREQ("", strip(""));
}
```

However, this silently drops the pointer to the returned string, not giving the chance to free the memory. This could be mitigated with another macro, however that is rather messy, and would ideally be something found in `gtest`. So instead, the return result is stored in a variable and freed after the comparison is made. The above was changed into:

```cpp
TEST(strip, EmptyString) {
    const char* result = strip("");
    ASSERT_STREQ("", result);
    free((void*) result);
}
```