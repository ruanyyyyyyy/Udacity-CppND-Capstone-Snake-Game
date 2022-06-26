//
// Created by Yue on 2022-06-23.
//
#include "computer_snake.h"
#include <cmath>
#include <iostream>


void ComputerSnake::update_food(SDL_Point food){
    // set end_node to food position
    end_node = std::make_pair(food.x, food.y);

    start_node = std::make_pair(static_cast<int>(head_x), static_cast<int>(head_y));
    AStarSearch();
    std::cout << "a star search" << std::endl;

}

void ComputerSnake::update_obstacles(std::vector<SDL_Point> &obstacles) {
    // put obstacles position to visited

    for(auto& ob : obstacles) {
        coord arr(ob.x, ob.y);
        visited.insert(arr);
    }
}


coord ComputerSnake::NextNode() {
    std::sort(open_list.begin(), open_list.end(), [this](coord& a, coord& b){return g_value[a] + h_value[a] > g_value[b] + h_value[b];});
    auto p = open_list.back();
    open_list.pop_back();
    return p;
}

std::vector<coord> ComputerSnake::FindNeighbors(const coord& current) {
    std::vector<coord> neighbors;
    float x = current.first;
    float y = current.second;
    if(x-1 >= 0) {
        neighbors.emplace_back(std::make_pair(x-1, y));
    }
    if(x+1 < grid_width) {
        neighbors.emplace_back(std::make_pair(x+1, y));
    }
    if(y-1 >= 0) {
        neighbors.emplace_back(std::make_pair(x, y-1));
    }
    if(y+1 < grid_height) {
        neighbors.emplace_back(std::make_pair(x, y+1));
    }
    return neighbors;
}

void ComputerSnake::AddNeighbors(coord& current_node){
    std::vector<coord> neighbors = FindNeighbors(current_node);
    for(auto neighbor : neighbors){
        if(visited.find(neighbor)==visited.end()){
            parent[neighbor] = current_node;
            h_value[neighbor] = CalculateHValue(neighbor);
            g_value[neighbor] = g_value[current_node] + speed;
            visited.insert(neighbor);
            open_list.push_back(neighbor);
        }
    }
}

std::vector<coord> ComputerSnake::ConstructFinalPath(coord *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<coord> path_found;

    // Implement your solution here.
    path_found.push_back(*current_node);
    while(parent.find(*current_node)!=parent.end()) {
        coord p = parent[*current_node];
        path_found.push_back(p);
        current_node = &p;
    }
    // TODO not reverse path_found
    // std::reverse(path_found.begin(), path_found.end());
    return path_found;

}



float ComputerSnake::CalculateHValue(const coord& node) {
    float dist = std::sqrt(std::pow(node.first-end_node.first, 2) + std::pow(node.second-end_node.second, 2));
    return dist;
}

void ComputerSnake::AStarSearch() {
    open_list.clear();
    visited.clear();
    g_value.clear();
    h_value.clear();

    coord current_node = start_node;
    open_list.emplace_back(current_node);
    visited.insert(current_node);
    g_value[current_node] = 0;
    h_value[current_node] = CalculateHValue(current_node);

    while(!open_list.empty()){
        current_node = NextNode();
        if(current_node==end_node){
            path = ConstructFinalPath(&end_node);
        }
        AddNeighbors(current_node);
    }
}


void ComputerSnake::Update() {
    SDL_Point prev_cell{
            static_cast<int>(head_x),
            static_cast<int>(
                    head_y)};  // We first capture the head's cell before updating.
    UpdateHead();
    std::cout << "computer snake: update head" << '\n';
    SDL_Point current_cell{
            static_cast<int>(head_x),
            static_cast<int>(head_y)};  // Capture the head's cell after updating.

    // Update all of the body vector items if the snake head has moved to a new
    // cell.
    if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
        UpdateBody(current_cell, prev_cell);
        std::cout << "computer snake: update body" << '\n';
    }
}

void ComputerSnake::UpdateHead() {
    // current_node = std::make_pair(static_cast<int>(head_x), static_cast<int>(head_y));
    auto next_node = path.back();

    if(head_x < next_node.first) {
        direction = Direction::kRight;
    } else if(head_x > next_node.first) {
        direction = Direction::kLeft;
    } else if (head_y < next_node.first) {
        direction = Direction::kDown;
    } else if (head_y > next_node.second) {
        direction = Direction::kUp;
    }


    switch (direction) {
        case Direction::kUp:
            head_y -= speed;
            if(head_y <= next_node.second) path.pop_back();
            break;

        case Direction::kDown:
            head_y += speed;
            if(head_y >= next_node.second) path.pop_back();
            break;

        case Direction::kLeft:
            head_x -= speed;
            if(head_x <= next_node.first) path.pop_back();
            break;

        case Direction::kRight:
            head_x += speed;
            if(head_x >= next_node.first) path.pop_back();
            break;
    }

    // Wrap the Snake around to the beginning if going off of the screen.
    head_x = fmod(head_x + grid_width, grid_width);
    head_y = fmod(head_y + grid_height, grid_height);
}



void ComputerSnake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
    // Add previous head location to vector
    body.push_back(prev_head_cell);

    if (!growing) {
        // Remove the tail from the vector.
        body.erase(body.begin());
    } else {
        growing = false;
        size++;
    }

    // Check if the snake has died.
    for (auto const &item : body) {
        if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
            alive = false;
        }
    }
}

void ComputerSnake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool ComputerSnake::SnakeCell(int x, int y) {
    if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
        return true;
    }
    for (auto const &item : body) {
        if (x == item.x && y == item.y) {
            return true;
        }
    }
    return false;
}