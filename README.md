# Thread Scheduler

A lightweight C++ thread scheduler and task manager designed to efficiently distribute and execute tasks with dependency management. Ideal as a foundational component for game engines or other multithreaded applications.

## Features

- Automatically detects and uses available hardware threads.
- Supports task dependencies to ensure correct execution order.
- Simple FIFO task scheduling with future plans for more advanced algorithms.
- Thread-safe task assignment and worker management.
- Easy to extend and integrate into larger projects.

## Getting Started

### Prerequisites

- C++11 compatible compiler  
- CMake (for building)  

### Building

```bash
mkdir build
cd build
cmake ..
make

##Usage

###Create tasks with optional dependencies and add them to the task manager:

task t = makeTask();
t.task = yourFunction;
t.dependencies = { /* task IDs */ };
taskManager tsk;
tsk.addTask(t);
tsk.distributeTasks();

## Roadmap

- Improve task scheduling algorithms
- Implement more robust dependency resolution
- Add comprehensive unit tests
- Provide better shutdown and cleanup mechanisms

## Contributing

Contributions, bug reports, and feature requests are welcome! Please open issues or pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
