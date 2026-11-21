# NucleiShell

A POSIX-style command interpreter written in C using raw system calls. Built from scratch for an Operating Systems course (FAST NUCES, Spring 2026). It does not use `system()` anywhere.

## Features

- **External commands**: fork / execvp / waitpid, with PATH lookup
- **Built-ins**: `cd`, `pwd`, `export`, `echo` (with `$VAR` expansion), `jobs`, `fg`, `history`, `exit`
- **I/O redirection**: `<`, `>`, `>>`
- **Multi-stage pipes**: n commands connected by n-1 anonymous pipes
- **Background execution**: `&` with a job list and SIGCHLD zombie reaping
- **Foreground bring-back**: `fg #PID`
- **Command history**: `history`, `history N`, `!!`, `!N`, `!-N`, `!str`
- **Signal handling**: Ctrl+C terminates the running command, not the shell

## Build and Run

Requires Linux (or WSL on Windows) and GCC.

```bash
make
./nucleishell
```

Compiled with `-Wall -Wextra -std=c99 -pedantic`.

## Usage Examples

```
My_Shell$ ls -la
My_Shell$ ls | grep .c | wc -l
My_Shell$ ls > out.txt
My_Shell$ cat < out.txt
My_Shell$ echo Hello >> out.txt
My_Shell$ export NAME=Noman
My_Shell$ echo $NAME
My_Shell$ sleep 10 &
[1] 4504
My_Shell$ jobs
My_Shell$ fg #4504
My_Shell$ history
My_Shell$ !!
My_Shell$ exit
```

## Project Structure

| File | Responsibility |
|------|----------------|
| `main.c` | REPL loop, history recall, dispatch |
| `parser.c / .h` | Tokenizer, detects `\|`, `<`, `>`, `>>`, `&` |
| `executor.c / .h` | fork/exec, redirection, pipeline execution |
| `builtins.c / .h` | cd, pwd, export, echo, fg, jobs, history |
| `history.c / .h` | In-memory ring buffer and `!` recall |
| `jobs.c / .h` | Background job list |
| `signals.c / .h` | SIGINT and SIGCHLD handlers via `sigaction()` |
| `defs.h` | Shared constants |
| `Makefile` | Build rules |

## OS Concepts Demonstrated

- Process lifecycle: `fork()`, `execvp()`, `waitpid()`
- File descriptors and `dup2()` for redirection
- Anonymous pipes (`pipe()`) and closing unused descriptors
- Job control with `setsid()` and `kill(pid, 0)`
- Signal handling with `sigaction()`, `SA_RESTART`, and zombie prevention via `SIGCHLD` + `WNOHANG`
- Environment variables with `getenv()` / `setenv()`

## Authors

- Ghulam Murtaza (K240720)
- Muhammad Yahya Khan (K241030)
- Noman Ali (K241027)

## Academic Note

This project was written for coursework. Please do not copy it for your own submissions.
