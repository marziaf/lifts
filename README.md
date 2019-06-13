# Instructions

## Compile

```
make

```
or

```
gcc -O2 -o lift main.c controller.c queue.c
```

## Execute

```
./lift <file> [-v]
```
#### File
`<file>` is the name of the file with the commands to be given in input to the program.
Some files can be found in `test/`. These tests are thought to be used in verbose mode, as they exploit verbose feature of 'time step & print' (see discussion about `-v` below). More infos about these sample files and generation of new ones can be found [here](tests/FILES_DESCRIPTION.md).

#### Verbose mode
`-v` option activates the verbose mode:
- After each command is required user's input to continue, so that each command's effect can be observed live
- Commands are repeated to user to keep track of what is happening
- After each time step a print status is performed
- When program terminates, memory is freed while printing the deletion progress

#### Example
```
./lift tests/random -v
```
