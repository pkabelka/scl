# scl

This is a collection of some simple single-file header C libraries in public
domain.

| Library                            | Description                                                                                                         | Example program                              |
| ---------------------------------- | ------------------------------------------------------------------------------------------------------------------- | -------------------------------------------- |
| [sstr](sstr.h)                     | String library (*NOTE*: a few functions require `memmem` function which can be specified using `SSTR_MEMMEM` macro) | [example](examples/sstr_example.c)           |
| [smemmem](smemmem.h)               | A few `memmem` implementations                                                                                      | [example](examples/smemmem_example.c)        |
| [sdll](sdll.h)                     | Double-linked list                                                                                                  | [example](examples/sdll_example.c)           |
| [sbintree](sbintree.h)             | Binary tree (*NOTE*: requires [sdll](sdll.h))                                                                       | [example](examples/sbintree_example.c)       |
| [strie](strie.h)                   | Trie (prefix tree)                                                                                                  | [example](examples/strie_example.c)          |
| [sfft](sfft.h)                     | Fast Fourier Transform (link with `-lm`)                                                                            | [example](examples/sfft_example.c)           |
| [sfft_recursive](sfft_recursive.h) | Recursive in-place Fast Fourier Transform (link with `-lm`)                                                         | [example](examples/sfft_recursive_example.c) |
| [sdarray](sdarray.h)               | Dynamic array (WIP)                                                                                                 | [example](examples/sdarray_example.c)        |
| [shashtab](shashtab.h)             | Hash table using *FNV 1a 32* hash algorithm (*NOTE*: requires [sbintree](sbintree.h) and [sdll](sdll.h))            | [example](examples/shashtab_example.c)       |

## Usage

Similar to [nothings' stb](https://github.com/nothings/stb). Define the desired
library implementation only once somewhere in your program and include the
header like so:

```c
#define SSTR_IMPLEMENTATION
#include "sstr.h"
```

In other files you can just use the include.

## LICENSE

This project is in public domain, see [LICENSE](LICENSE).
