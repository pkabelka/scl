# scl

## sstr

`sstr` stands for *simple string*. It is a single header file implementation of
a very basic C string library.

### Usage

Similar to [nothings' stb](https://github.com/nothings/stb):

```c
#define SSTR_IMPLEMENTATION
#include "sstr.h"
```

*NOTE*: the functions `sstr_replace` and `sstr_count` require `memmem`, so you
need to define it e.g. in an include or specify a custom definition via the
`SSTR_MEMMEM` macro. You can also use a function from [smemmem.h](smemmem.h).

An example program can be found [here](examples/sstr_example.c).

## smemmem

`smemmem` stands for *simple memmem*. It is a single header file implementation
of a several `memmem` functions.

### Usage

Similar to [nothings' stb](https://github.com/nothings/stb):

```c
#define SMEMMEM_IMPLEMENTATION
#include "smemmem.h"
```

An example program can be found [here](examples/smemmem_example.c).

## sdll

`sdll` stands for *simple double linked list*. It is a single header file
implementation of a very basic double linked list.

### Usage

```c
#define SDLL_IMPLEMENTATION
#include "sdll.h"
```

An example program can be found [here](examples/sdll_example.c).

## sbintree

`sbintree` stands for *simple binary tree*. It is a single header file
implementation of a very basic binary search tree.

### Usage

`sbintree.h` also requires `sdll.h` and therefore `#define SDLL_IMPLEMENTATION`
too.

```c
#define SBINTREE_IMPLEMENTATION
#define SDLL_IMPLEMENTATION
#include "sbintree.h"
```

An example program can be found [here](examples/sbintree_example.c).

## strie

`strie` stands for *simple trie*. It is a single header file
implementation of a very basic trie (prefix tree).

### Usage

```c
#define STRIE_IMPLEMENTATION
#include "strie.h"
```

An example program can be found [here](examples/strie_example.c).

## sfft

`sfft` stands for *simple Fast Fourier transform*. It is a single header file
implementation of a Fast Fourier transform.

### Usage

```c
#define SFFT_IMPLEMENTATION
#include "sfft.h"
```

The library uses `math.h` and therefore you need to link with `-lm` when
compiling.

An example program can be found [here](examples/sfft_example.c).

## sfft_recursive

`sfft_recursive` stands for *simple recursive Fast Fourier transform*. It is a
single header file implementation of a recursive Fast Fourier transform.

### Usage

```c
#define SFFT_RECURSIVE_IMPLEMENTATION
#include "sfft_recursive.h"
```

The library uses `math.h` and therefore you need to link with `-lm` when
compiling.

An example program can be found [here](examples/sfft_recursive_example.c).

## sdarray

`sdarray` stands for *simple dynamic array*. It is a single header file
implementation of a very basic dynamic array.

### Usage

```c
#define SDARRAY_IMPLEMENTATION
#include "sdarray.h"
```

An example program can be found [here](examples/sdarray_example.c).

## shashtab

`shashtab` stands for *simple hash table*. It is a single header file
implementation of a very basic hash table using FNV 1a 32 hash algorithm.

### Usage

`shashtab.h` also requires `sbintree.h` and therefore `#define
SBINTREE_IMPLEMENTATION` and `#define SDLL_IMPLEMENTATION` too.

```c
#define SHASHTAB_IMPLEMENTATION
#define SBINTREE_IMPLEMENTATION
#define SDLL_IMPLEMENTATION
#include "shashtab.h"
```

An example program can be found [here](examples/shashtab_example.c).

## LICENSE

This project is in public domain, see [LICENSE](LICENSE).
