# CPPND: Program a Concurrent Traffic Simulation

<img src="data/traffic_simulation.gif"/>

This is final project for the fourth course in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213): Concurrency.

The project is a multithreaded traffic simulator using real urban map. Each vehicle runs on a separate thread. Each intersection is run on a seperate thread as well, which manages traffic flow and enables collision avoidance.

During the project development, a thread-safe communication protocol between vehicles and intersections has been established. Usage of multitude concurrent programming concepts has been trained (such as tasks, promises, futures, mutexes, locks and message queues).

As the course was focused on concurrency, some starter code had been provided before as a student I was envolved in the development. That includes mainly the graphical layer of the project, and overal structure of the program (such as base classes).

## Dependencies for Running Locally

- cmake >= 2.8
  - All OSes: [click here for installation instructions](https://cmake.org/install/)
- make >= 4.1 (Linux, Mac), 3.81 (Windows)
  - Linux: make is installed by default on most Linux distros
  - Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  - Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
- OpenCV >= 4.1
  - The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
- gcc/g++ >= 5.4
  - Linux: gcc / g++ is installed by default on most Linux distros
  - Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  - Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./traffic_simulation`.
