#include <iostream>
#include <fstream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  /*
  std::ofstream myfile("users_info.txt", std::ios::out);
  std::cout << "Please input your name:" << '\n';
  std::string name;
  std::cin >> name;
  myfile << name << ' ';
*/

  /*std::cout << "Please select the initial speed. Input 0 to get slow speed and input 1 to get fast speed:" << '\n';
  int init_speed;
  std::cin >> init_speed;
  if(init_speed==0) {
      std::size_t kFramesPerSecond{30};
      std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  } else {
      std::size_t kFramesPerSecond{60};
      std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  }*/

  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  constexpr std::size_t kGridNumOb{10};


  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;

  Game game(kGridWidth, kGridHeight, kGridNumOb);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";

  /*myfile << game.GetScore() << "\n";
  myfile.close();*/
  return 0;
}