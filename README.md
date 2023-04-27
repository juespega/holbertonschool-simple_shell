# Simple Shell

This is a simple shell project, written in C, created as part of the curriculum for the Holberton School. The goal of this project is to implement a basic UNIX shell that can handle commands and arguments, as well as some built-in shell commands.

|.     ![](https://www.iberdrola.com/documents/20125/40414/vr_746x419.jpg/d3221355-c367-f6b4-07dc-d893ac61c985?t=1627362779952)

## Usage

The shell program can be run in both interactive and non-interactive mode.

In interactive mode, simply run the executable with no arguments:

```
$ ./hsh
($) /bin/ls
hsh main.c shell.c
($) exit
$
```
In non-interactive mode, pipe commands into the executable:

```
$ echo "/bin/ls" | ./hsh
hsh main.c shell.c test_ls_2
$
$ cat test_ls_2 | ./hsh
hsh main.c shell.c test_ls_2
hsh main.c shell.c test_ls_2
$
```

## Task

#	Requirement content	Status 🔨
1	Write a C program simple-shell.c that implements above specifications

2	You will submit this assignment on Moodle. A skeleton of simple-shell.c program has also been added on Moodle

3	You can download and run this example simple-shell to get an idea what is expected from you in this assignment. This example program has been built and tested in Linux Debian machines available in the lab. Run this program in other kind of machines at your own risks

4	Make sure your program compiles and runs error and warning free

5	Test your program to make sure your code has fulfilled the specifications

6	Organize and comment your code to make it easy to understand

### List of allowed functions and system calls

* all functions from strings.h
* access (man 2 access)
* chdir (man 2 chdir)
* close (man 2 close)
* closedir (man 3 closedir)
* execve (man 2 execve)
* exit (man 3 exit)
* _exit (man 2 _exit)
* fflush (man 3 fflush)
* fork (man 2 fork)
* free (man 3 free)
* getcwd (man 3 getcwd)
* getline (man 3 getline)
* getpid (man 2 getpid)
* isatty (man 3 isatty)
* kill (man 2 kill)
* malloc (man 3 malloc)
* open (man 2 open)
* opendir (man 3 opendir)
* perror (man 3 perror)
* printf (man 3 printf)
* fprintf (man 3 fprintf)
* vfprintf (man 3 vfprintf)
* sprintf (man 3 sprintf)
* putchar (man 3 putchar)
* read (man 2 read)
* readdir (man 3 readdir)
* signal (man 2 signal)
* stat (__xstat) (man 2 stat)
* lstat (__lxstat) (man 2 lstat)
* fstat (__fxstat) (man 2 fstat)
* strtok (man 3 strtok)
* wait (man 2 wait)
* waitpid (man 2 waitpid)
* wait3 (man 2 wait3)
* wait4 (man 2 wait4)
* write (man 2 write)


## Features

The shell program can handle the following features:

* Execution of commands in $PATH
* Handling of command line arguments
* Handling of the following built-in shell commands:
    * ls: list of files
    * env: print environment variables
    * exit: exit the shell program
* Error handling, including handling of non-existent commands and permission errors

## Files

* shell.c - contains the main shell program logic
* shell.h - contains function prototypes and structure declarations
* prompt.c - contains shell commands



## Authors

This shell program was written by 

* Ana Maria Cardona Botero
* Julian Esteban Perez Gallego
* Ruben Dario Florez Betancur

It is based on the work of the Holberton School staff and contributors.
