# SPL

SPL (simple programming language) is a homemade dynamically-typed programming language made for the fun of it. 

## Features

This language currently supports variables, functions, integer expressions, floating-point expressions, and boolean expressions.

## Syntax
Notes:
* The program execution starts at the first line of the file.
* Functions are defined with `fun`.
* Booleans and integers are currently supported.
* Semicolons and curly braces are required.
* In the below code segment, `result` and `equality` are two variables declared with an inferred type. `result` is an integer and `equality` is a boolean.
* `myFunction` is also a variable that holds a function.

```kt
fun myFunction(arg1, arg2) {
    return (arg1 + arg2) * 5;
}

result = myFunction(2.5, 3);
equality = result == 27.5;
```

`equality` will be `true` in this case.

## Contributing

Contributions are welcome! Feel free to open an issue or submit a pull request. For larger changes, please open an issue first to discuss the changes.