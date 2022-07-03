//
// Created by Yue on 2022-06-23.
//
#include "astar.h"
#include <cmath>
#include <iostream>


AStar::AStar(int grid_width, int grid_height)
    : grid_width(grid_width),
      grid_height(grid_height) {
    nodes_.resize(grid_width);
    for (int i = 0; i < grid_width; i++) {
        nodes_[i].resize(grid_height);
        for (int j = 0; j < grid_height; j += 1) {
            nodes_[i][j].state = State::kOpen;
            nodes_[i][j].x = i;
            nodes_[i][j].y = j;
        }
    }
}

void AStar::update_obs(std::vector<SDL_Point> &obstacles) {
    for (auto &ob: obstacles) {
        nodes_[ob.x][ob.y].state = State::kBlocked;
    }
}


bool AStar::CheckValidNode(int x, int y) {
    if (x < 0 || x >= grid_width || y < 0 || y >= grid_height) {
        return false;
    }
    if (nodes_[x][y].state == State::kVisited || nodes_[x][y].state == State::kBlocked) {
        return false;
    }
    return true;
}

float AStar::CalculateHValue(const Node *current_node) {
    float dist = std::sqrt(std::pow(current_node->x - end_pos.x, 2) + std::pow(current_node->y - end_pos.y, 2));
    return dist;
}

void AStar::AddNeighbors(Node *current_node) {
    int new_x;
    int new_y;
    for (auto det: delta_) {
        new_x = current_node->x + det[0];
        new_y = current_node->y + det[1];
        if (CheckValidNode(new_x, new_y)) {
            nodes_[new_x][new_y].state = State::kVisited;
            nodes_[new_x][new_y].g_value = current_node->g_value + 1;
            nodes_[new_x][new_y].h_value = CalculateHValue(current_node);
            nodes_[new_x][new_y].parent = current_node;
            open_list.emplace_back(&nodes_[new_x][new_y]);
        }
    }
}

Node *AStar::NextNode() {
    std::sort(open_list.begin(), open_list.end(), [this](Node *a, Node *b) { return a->g_value + a->h_value > b->g_value + b->h_value; });
    auto p = open_list.back();
    open_list.pop_back();
    return p;
}

void AStar::ConstructFinalPath(Node *current_node) {
    // Implement your solution here.
    path.clear();
    SDL_Point point;
    point.x = current_node->x;
    point.y = current_node->y;
    path.push_back(point);
    while (current_node->parent != nullptr) {
        Node *p = current_node->parent;
        // std::cout << current_node->x << " " << current_node->y << '\n';
        point.x = (*p).x;
        point.y = (*p).y;
        path.push_back(point);
        current_node = p;
    }
    // not reverse path_found
    // std::reverse(path_found.begin(), path_found.end());
}


void AStar::AStarSearch(SDL_Point &start, SDL_Point &food) {
    end_pos.x = food.x;
    end_pos.y = food.y;

    int x = start.x;
    int y = start.y;
    Node *current_node = &nodes_[x][y];
    open_list.emplace_back(current_node);
    current_node->state = State::kVisited;

    while (!open_list.empty()) {
        current_node = NextNode();
        if (current_node->x == end_pos.x && current_node->y == end_pos.y) {
            ConstructFinalPath(current_node);
            break;
        }
        AddNeighbors(current_node);
    }
}
