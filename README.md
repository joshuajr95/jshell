# jshell

My own implementation of a shell. The actual output binary is called `josh` for Josh's shell (like bash for bourne again shell). The shell is written in C++ instead of plain C because I want to practice using some of the aspects of C++ not found in C such as standard library elements (vector, map, etc.), templates, smart pointers, and references. For now, the shell does not have the full capabilities of a true shell interpreter, but it does have the ability to execute pipelines, builtins, and do input, output, and error redirection.



## Future Developments

Future developments of this shell may include

1. Building a full shell interpreter that uses a more sophisticated parsing algorithm and formally specifies the grammar. This would allow for keywords such as `export`, `source`, and others. It would also allow for shell constructs like `for` and `if`.
2. Subshells. This would allow the shell to run scripts as well as do command substitution.
3. Multiple output, input, and error redirection. This would also necessitate much more complex process management than it currently has.
