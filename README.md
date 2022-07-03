# CPPND: Capstone Snake Game 

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="new_snake_game.gif"/>

In this project, I extend this Snake game to the following directions:
1. add obstacles (shown as grey rectangles) to the map
2. add another snake which follows the path (shown as green grids) calculated from A* algorithm.
3. allow user to enter their names before playing game
4. log user's score in a file named `users_info.txt` located in `build` folder.

The above is a short video to illustrate the extended game.

The most time-consuming part when I implemented the extensions is to determine the direction for each move according to the path from A* algorithm.
The moving speed is not accurate 0.1 so this cause some trouble to predict the accurate position after each move.
Through careful observations, I realize we can use `static_cast<int>(head_x/y)` instead of `head_x/y` because we only care about the head's integer position in rendering and determining whether it arrives at the food.
So I use `static_cast<int>(head_x/y)` to [infer the moving direction](https://github.com/ruanyyyyyyy/Udacity-CppND-Capstone-Snake-Game/blob/6ff9da71850e3e3b91d964df7d404ec7a46159a4/src/game.cpp#L123) and determine whether we should [pop out the previous goal (last grid in path) dynamically](https://github.com/ruanyyyyyyy/Udacity-CppND-Capstone-Snake-Game/blob/6ff9da71850e3e3b91d964df7d404ec7a46159a4/src/game.cpp#L117).

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
