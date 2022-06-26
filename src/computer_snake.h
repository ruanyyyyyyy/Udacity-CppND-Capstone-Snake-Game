//
// Created by Yue on 2022-06-23.
//

#ifndef COMPUTER_SNAKE_H
#define COMPUTER_SNAKE_H

#include <vector>
#include <cmath>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include "SDL.h"
#include <iostream>
#include <utility>
typedef std::pair<int, int> coord;
typedef std::map<coord, coord> map;

class ComputerSnake {
public:
    enum class Direction { kUp, kDown, kLeft, kRight };

    ComputerSnake(int grid_width, int grid_height)
        : grid_width(grid_width),
          grid_height(grid_height),
          head_x(grid_width / 2),
          head_y(grid_height / 2) {
        current_node = std::make_pair(head_x, head_y);
        cnt = 0;
    }

    void Update();

    void GrowBody();
    bool SnakeCell(int x, int y);

    Direction direction = Direction::kUp;

    float speed{0.1f};
    int size{1};
    bool alive{true};
    float head_x;
    float head_y;
    std::vector<SDL_Point> body;

    // additional functions for computer snake
    float CalculateHValue(const coord& node);
    void update_food(SDL_Point food);
    void update_obstacles(std::vector<SDL_Point> & obstacles);
    std::vector<coord> ConstructFinalPath(coord* current_node);
    void AStarSearch();
    float GetDistance() const {return distance;}
    coord NextNode();
    std::vector<coord> FindNeighbors(const coord& current);
    void AddNeighbors(coord& current_node);
    std::vector<coord> path;
    int cnt;

    // Hash function
    struct hashFunction
    {
        size_t operator()(const std::pair<int, int> &x) const
        {
            return x.first ^ x.second;
        }
    };

    // Hash function
    struct hashFunction2
    {
        std::size_t operator()(const coord &s) const noexcept
        {
            std::size_t h1 = std::hash<float>{}(s.first);
            std::size_t h2 = std::hash<float>{}(s.second);
            return h1 ^ (h2 << 1); // or use boost::hash_combine
        }
    };



private:
    void UpdateHead();
    void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

    bool growing{false};
    int grid_width;
    int grid_height;

    // additional variable and functions for computer snake
    coord current_node; // equal to head_x and head_y in Snake
    coord start_node;
    coord end_node;

    std::vector<coord> open_list;

    std::unordered_set<coord, hashFunction> visited;
    std::map<coord, float> h_value;
    std::map<coord, float> g_value;
    map parent;
    float distance = 0.0f;


};

#endif //COMPUTER_SNAKE_H
