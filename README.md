# Ștefan MĂGIRESCU

## File System

Functions description:
- **freeSystem**() frees the whole graph in a depth first manner, recursively
- **tabPrinter**() prints spaces for the tree() function
- **root**() initializes the root directory
- **parser**() parses the input instructions, splitting them into 3 strings: *command, arg1 and arg2*
- the rest of the functions do exactly what they are supposed to do in the tasks, with no extra features (comments are provided)
- **touch**() checks if name already exists in both linked lists and if not, alloc's memory for new element and adds it to corresponding list
- **mkdir**() same as touch()
- **ls**() traverses both lists and prints the nodes
- **rm**() finds file and removes its corresponding node
- **rmdir**() removes directory and its children reccursively
- **cd**() changes the value of the currentDir pointer
- **pwd**() goes up in the system and concatenates to the path variable each node it passes through
- **stop**() frees all memory and quits program
- **free**() function that is used to free memory reccursively
- **mv**() changes file/dir name if it doesn't already exist (comments provided)