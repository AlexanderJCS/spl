* Control flow statements (`continue`, `break`, `return`) should not be std::runtime_errors that are caught by when evaluating the AST. This is really dumb.
* Create a CLI so code can be written in a text file
* Improve performance by a lot (it's currently several hundred times slower than Python...)
* String bug: `" world"` string is not tokenized correctly
* Implement a print function
* Implement stdlib for basic math functions
* Implement stdlib for basic string manipulation functions
    * `len`
    * `substr`
    * `concat`
* Implement file io (including `readallat` and `writeallat`)
    * Implement a `file` type (or maybe a more general `stream` type?)