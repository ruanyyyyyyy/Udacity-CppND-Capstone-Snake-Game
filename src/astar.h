//
// Created by Yue on 2022-06-23.
//

#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <cmath>
#include "SDL.h"
#include <iostream>
#include <limits>

enum class State {kOpen, kClosed, kBlocked, kPath};
struct Node {
    int x = 0;
    int y = 0;
    float h_value=std::numeric_limits<int>::max();
    float g_value=0;
    State state = State::kOpen;
    bool visited = false;
    Node* parent = nullptr;
};

class AStar {
public:
    AStar(int grid_width, int grid_height, std::vector<SDL_Point> & obstacles);

    Node end_pos;
    std::vector<std::vector<Node>> nodes_;
    std::vector<Node> path;
    std::vector<SDL_Point> path_found;

    // void update_map(SDL_Point& food, std::vector<SDL_Point> & obstacles); TODO implement later
    float CalculateHValue(const Node* node);
    void ConstructFinalPath(Node* current_node);
    void AStarSearch(SDL_Point& start, SDL_Point& food);

    Node* NextNode();
    void AddNeighbors(Node* current_node);

private:
    int grid_width;
    int grid_height;

    std::vector<Node*> open_list;
    constexpr static int delta_[4][2]{ {-1,0}, {1,0}, {0,-1}, {0,1} };
    bool CheckValidNode(const int x, const int y);
};

#endif
