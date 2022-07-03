#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <fstream>
#include <iostream>

int main() {

    std::ofstream myfile("users_info.txt", std::ios::app); // std::ios::out override the file. std::ios::app append to the file
    std::cout << "Please input your name:" << '\n';
    std::string name;
    std::cin >> name;
    myfile << name << ' ';

    constexpr std::size_t kFramesPerSecond{60};
    constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    constexpr std::size_t kScreenWidth{640};
    constexpr std::size_t kScreenHeight{640};
    constexpr std::size_t kGridWidth{32};
    constexpr std::size_t kGridHeight{32};
    constexpr std::size_t kGridNumOb{40};


    Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
    Controller controller;

    Game game(kGridWidth, kGridHeight, kGridNumOb);
    game.Run(controller, renderer, kMsPerFrame);
    std::cout << "Game has terminated successfully!\n";
    std::cout << "Score: " << game.GetScore() << "\n";
    std::cout << "Size: " << game.GetSize() << "\n";

    myfile << game.GetScoreH() << "\n";
    myfile.close();
    return 0;
}