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

class ComputerSnake {
public:
    ComputerSnake(int grid_width, int grid_height)
        : grid_width(grid_width),
          grid_height(grid_height),
          head_x(grid_width / 2),
          head_y(grid_height / 2) {
        current_node = std::make_pair(head_x, head_y);
    }

    void Update();

    void GrowBody();
    bool SnakeCell(int x, int y);

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

    // Hash function
    struct hashFunction
    {
        size_t operator()(const std::pair<int ,
                                          int> &x) const
        {
            return x.first ^ x.second;
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
    coord end_node;

    std::vector<coord> open_list;
    std::pair<int,int> NextNode(const coord& current);
    std::vector<coord> FindNeighbors(const coord& current);
    void AddNeighbors(coord& current_node);
    std::unordered_set<coord, hashFunction> visited;
    std::map<coord, int> h_value;
    std::map<coord, int> g_value;

};

#endif //COMPUTER_SNAKE_H
