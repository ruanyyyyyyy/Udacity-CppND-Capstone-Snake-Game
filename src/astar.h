//
// Created by Yue on 2022-06-23.
//

#ifndef ASTAR_H
#define ASTAR_H

#include "SDL.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

// Set state for each node
// kOpen: accessible node and not visited
// kVisited: accessible node and already visited
// kBlocked: not accessible. an obstacle
// kPath: a node on the path. Not necessary for this algorithm
enum class State { kOpen,
                   kVisited,
                   kBlocked };// kPath
struct Node {
    int x = 0;
    int y = 0;
    float h_value = std::numeric_limits<int>::max();
    float g_value = 0;
    State state = State::kOpen;
    Node *parent = nullptr;
};

class AStar {
public:
    AStar(int grid_width, int grid_height);

    Node end_pos;
    std::vector<std::vector<Node>> nodes_;
    std::vector<SDL_Point> path;

    float CalculateHValue(const Node *node);
    void ConstructFinalPath(Node *current_node);
    void AStarSearch(SDL_Point &start, SDL_Point &food);
    void update_obs(std::vector<SDL_Point> &obstacles);

    Node *NextNode();
    void AddNeighbors(Node *current_node);

private:
    int grid_width;
    int grid_height;

    std::vector<Node *> open_list;
    constexpr static int delta_[4][2]{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    bool CheckValidNode(const int x, const int y);
};

#endif
