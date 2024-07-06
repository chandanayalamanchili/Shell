# Custom Shell Documentation

## Overview

This is a custom shell program with basic commands renamed for functionality. It includes support for system commands, I/O redirection, piping, signal handling, process management, and command-specific options.

## Commands

- `warp`: Equivalent to `cd`, changes directory.
- `peek`: Equivalent to `ls`, lists directory contents with flags support.
- `seek`: Equivalent to `find`, searches for files with options.
- `proclore`: Equivalent to `ps`, displays process status.
- `pastevents`: Displays command history.
- `activities`: Shows spawned activities.
- `neonate`: Retrieves latest process ID.
- `iMan`: Retrieves command manuals using internet.

## Shell Features

- **Prompt**: Displays username, system name, and current directory (`~` for home).
- **I/O Redirection**: Supports `<`, `>`, `>>` for input, output, and append.
- **Pipes**: Allows chaining commands using `|`.
- **Signal Handling**: Manages `Ctrl + Z`, `Ctrl + C`, `Ctrl + D`.
- **Process Management**: Supports foreground (`fg`) and background (`bg`) processes.
- **Manual (`iMan`)**: Fetches command manuals online.
- **Compilation**: Use `make` to compile; generates `a.out` executable.
- **Exit**: Type `exit` or `Ctrl + D` to quit the shell.

## Assumptions

- Limited flags for commands.
- Consistent file formats and permissions.
- Commands like `seek`, `fg`, `warp`, `activities` cannot be used in pipes.
- Input formats are straightforward.
- Requires internet access for `iMan`.

## Usage

1. Compile: Execute `make` in main directory.
2. Run: Start with `./a.out` in terminal.
3. Exit: Type `exit` or use `Ctrl + D`.
4. Interrupt: Use `Ctrl + C` for foreground process.
5. Background: Use `Ctrl + Z` to suspend and move to background.

---


