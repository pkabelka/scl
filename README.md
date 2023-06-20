# scl

## sstr

`sstr` stands for *simple string*. It is a single header file implementation of
a very basic C string library.

### Usage

Similar to [nothings' stb](https://github.com/nothings/stb):

```
#define SSTR_IMPLEMENTATION
#include "sstr.h"
```

You can adjust the size of additionally allocated memory when the string's
capacity overflows. You can define it before `#include "sstr.h"` in the same
file where `SSTR_IMPLEMENTATION` is defined with:

```
#define SSTR_ALLOC_SIZE 32
```

An example program can be found [here](examples/sstr_example.c).

## sdll

`sdll` stands for *simple double linked list*. It is a single header file
implementation of a very basic double linked list.

### Usage

```
#define SDLL_IMPLEMENTATION
#include "sdll.h"
```

An example program can be found [here](examples/sdll_example.c).

## LICENSE

This project is in public domain, see [LICENSE](LICENSE).
