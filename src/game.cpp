#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t num_ob)
    : snake(grid_width, grid_height),
      computer_snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      random_ob(1, static_cast<int>(num_ob - 1)) {
  PlaceFood();
  PlaceDiffFood();
  PlaceObstacles();

  computer_snake.update_food(food);
  computer_snake.update_obstacles(obstacles);
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, computer_snake, food, obstacles);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceObstacles() {
  int x, y;
  SDL_Point  ob;
  int num = random_ob(engine);
  int cnt = 0;
  while (cnt < num) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // obstacles.
    if (!snake.SnakeCell(x, y)) {
      ob.x = x;
      ob.y = y;
      obstacles.emplace_back(std::move(ob));
      cnt += 1;
    }
  }
}

void Game::PlaceFood() {
    int x, y;
    while (true) {
        x = random_w(engine);
        y = random_h(engine);
        // Check that the location is not occupied by a snake item before placing
        // food.
        if (!snake.SnakeCell(x, y)) {
            food.x = x;
            food.y = y;
            return;
        }
    }
}

void Game::PlaceDiffFood() {
    int x, y;
    while (true) {
        x = random_w(engine);
        y = random_h(engine);
        // Check that the location is not occupied by a snake item before placing
        // food.
        if (!snake.SnakeCell(x, y) && x!=food.x && x!=food.y) {
            diff_food.x = x;
            diff_food.y = y;
            return;
        }
    }
}

void Game::Update() {
  if (!snake.alive or !computer_snake.alive) return;

  snake.Update();
  computer_snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  int comp_new_x = static_cast<int>(computer_snake.head_x);
  int comp_new_y = static_cast<int>(computer_snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    computer_snake.update_food(food);
    computer_snake.update_obstacles(obstacles);

    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }

  /*// Check if there's another kind of food over here
  if (diff_food.x == new_x && diff_food.y == new_y) {
      score+= 2;
      PlaceDiffFood();

      // Grow snake and increase speed.
      snake.GrowBody();
      snake.speed += 0.04;
  }*/

  // Check if there's obstacles over here
  for(auto& ob : obstacles) {
      if (ob.x == new_x && ob.y == new_y) {
          snake.alive = false;
      }
  }

  // Check if there's food over here
  if (food.x == comp_new_x && food.y == comp_new_y) {
      PlaceFood();
      computer_snake.update_food(food);
      computer_snake.update_obstacles(obstacles);

      // Grow snake and increase speed.
      computer_snake.GrowBody();
      // computer_snake.speed += 0.02; TODO speed up later
  }

}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }