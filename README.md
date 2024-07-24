<div align="center">

# Threaded Producer Consumer
### An example of a threaded producer / consumer system in C.
##

[Intro](#introduction) • [Build it](#how-to-build-it) • [Run it](#how-to-run-it) • [Breakdown](#how-it-works)

</div>

### Introduction
This is a proof-of-concept of a working producer / consumer system, utilizing threads.

This is a demonstration of the concepts used in a project made just before graduating from college.

### How to Build it
To build this project, it is necessary to install the following packages: `gcc` `make`

Then, simply run the `make` command inside the project directory to build the executable `Threads` file.

### How to Run it
To run the program, simply execute it inside a terminal window like so: `./Threads`

### How it Works
The project demonstrates the application of several programming concepts, such as Pointers, Structures, Queues, Threads, Mutex & I/O, all implemented in C.

Several functions are used to manipulate and write the contents of a simulated JSON file and write it to the current directory.

The `main` funcion first calls upon the function that initializes the Queue, which is made out of several structures. Then, the mutex is initialized. A total of 3 threads are started, the producer, consumer and manager threads.

<details>
<summary><b> Manager Thread </b></summary>
  
The manager thread is responsible for constantly changing the variable that represents a valid internet connection.

</details>

<details>
<summary><b> Producer Thread </b></summary>

The producer thread then tests the variable for the internet connection, and depending on its state, takes the following acitons:
- If the connection variable equals false:
  - It first tries to transfer any Node structures from the Queue in memory and write it to disk, if any exist.
  - It then allocates a new Node to the Queue in memory, and adds a new JSON data structure to said Node.
- If the connection variable equals true:
  - It simply allocates a new Node to the Queue in memory, and adds a new JSON data structure to said Node.

</details>

<details>
<summary><b> Consumer Thread </b></summary>

The consumer thread in turn, checks the variable for the internet connection and the variable that tracks how many files are stored in disk and takes the following acitons:
- If there is a connection & any stored files:
  - It then reads the data of the latest of the stored files on disk & simulates the process of sending it, by printing out a prompt. The file is then deleted from disk.
- If there is a connection, but no stored files:
  - It reads the data of the latest of the allocated Nodes in the Queue, and simulates the process of sending it, by printing out a prompt. The Node is then freed from the Queue.

</details>

The threads all work together in a cicle, and no thread accesses the files on disk at the same time, thanks to the implemented Mutex.

##
<div align="center">

[![Back to the Top Badge](https://custom-icon-badges.demolab.com/badge/Back_to_the_Top-black?logo=chevron-up)](#threaded-producer-consumer)

</div>
