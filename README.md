#Project structure

```bash
├── asset # folder that contain the additional document for the project
│   └── file.txt # the default compiling file
├── build       # the executable folder
├── include     # folder contain sthe header files
├── src         # folder that contain .the implementation of the header file + the software logic
│    ├── main.c # the software entry point
│    └── ...
├── Makefile    # facilate the project compiling and  running g
```

#How to use this project

if the first time u open this project only write make on ur terminal to run compile it and run it

```bash
make # to compile the project and run it
```

and to clean the executable folder _build_ try:

```bash
make clean
```

finally to clean compile and run try:

```bash
make test
```
# Eyrn-compiler
