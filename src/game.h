#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "astar.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <random>
#include <vector>

class Game {
public:
    Game(std::size_t grid_width, std::size_t grid_height, std::size_t num_ob);
    void Run(Controller const &controller, Renderer &renderer,
             std::size_t target_frame_duration);
    int GetScore() const;
    int GetSize() const;
    int GetScoreH() const;
    int GetSizeH() const;
    SDL_Point goal;
    AStar astar;

private:
    Snake snake;
    Snake snakeH;
    SDL_Point food;
    std::vector<SDL_Point> obstacles;
    std::vector<SDL_Point> path;

    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;
    std::uniform_int_distribution<int> random_ob;

    int score{0};
    int scoreH{0};

    void PlaceFood();
    void Update();
    void PlaceObstacles();
    bool CheckObstacles(int x, int y);
    void UpdatePathAstar();

    int grid_width;
    int grid_height;
};

#endif