#include "game.h"
#include "SDL.h"
#include <iostream>


Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t num_ob)
    : grid_width(grid_width),
      grid_height(grid_height),
      snake(grid_width, grid_height),
      snakeH(grid_width, grid_height),
      astar(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      random_ob(1, static_cast<int>(num_ob - 1)) {
    PlaceFood();
    PlaceObstacles();
    astar.update_obs(obstacles);
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    int frame_count = 0;
    bool running = true;

    SDL_Point start_point;
    start_point.x = static_cast<int>(snake.head_x);
    start_point.y = static_cast<int>(snake.head_y);
    astar.AStarSearch(start_point, food);
    path = astar.path;

    while (running) {
        frame_start = SDL_GetTicks();

        // Input, Update, Render - the main game loop.
        controller.HandleInput(running, snakeH);
        Update();
        renderer.Render(snake, snakeH, food, obstacles, path);

        frame_end = SDL_GetTicks();

        // Keep track of how long each loop through the input/update/render cycle
        // takes.
        frame_count++;
        frame_duration = frame_end - frame_start;

        // After every second, update the window title.
        if (frame_end - title_timestamp >= 1000) {
            renderer.UpdateWindowTitle(score, scoreH, frame_count);
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
    SDL_Point ob;
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

bool Game::CheckObstacles(int x, int y) {
    for (auto &ob: obstacles) {
        if (ob.x == x && ob.y == y) {
            return true;
        }
    }
    return false;
}
void Game::PlaceFood() {
    int x, y;
    while (true) {
        x = random_w(engine);
        y = random_h(engine);

        // Check that the location is not occupied by a snake item before placing
        // food.
        if (!snake.SnakeCell(x, y) && !CheckObstacles(x, y)) {
            food.x = x;
            food.y = y;
            return;
        }
    }
}



void Game::Update() {
    if (!snake.alive) return;
    if (!snakeH.alive) return;

    // determine if the snake head has arrived goal
    if ((snake.direction == Snake::Direction::kLeft || snake.direction == Snake::Direction::kRight) && static_cast<int>(snake.head_x) == goal.x || (snake.direction == Snake::Direction::kUp || snake.direction == Snake::Direction::kDown) && static_cast<int>(snake.head_y) == goal.y) {
        path.pop_back();// will pop back the start position automatically
    }

    goal = path.back();
    // determine the future direction based on goal
    if (static_cast<int>(snake.head_x) > goal.x) {
        snake.direction = Snake::Direction::kLeft;
    } else if (goal.x > static_cast<int>(snake.head_x)) {
        snake.direction = Snake::Direction::kRight;
    } else if (static_cast<int>(snake.head_y) > goal.y) {
        snake.direction = Snake::Direction::kUp;
    } else if (goal.y > static_cast<int>(snake.head_y)) {
        snake.direction = Snake::Direction::kDown;
    }

    snake.Update();
    snakeH.Update();

    int new_x = static_cast<int>(snake.head_x);
    int new_y = static_cast<int>(snake.head_y);

    // Check if there's food over here
    if (food.x == new_x && food.y == new_y) {
        score++;
        PlaceFood();

        // Grow snake and increase speed.
        snake.GrowBody();
        // snake.speed += 0.02; will cause wrong move after speed > 1.0

        // Update path for a* algorithm
        UpdatePathAstar();

    }

    // Check if there's obstacles over here
    for (auto &ob: obstacles) {
        if (ob.x == new_x && ob.y == new_y) {
            snake.alive = false;
        }
    }


    int new_xH = static_cast<int>(snakeH.head_x);
    int new_yH = static_cast<int>(snakeH.head_y);

    // Check if there's food over here
    if (food.x == new_xH && food.y == new_yH) {
        scoreH++;
        PlaceFood();

        // Grow snake and increase speed.
        snakeH.GrowBody();

        UpdatePathAstar();
    }

    // Check if there's obstacles over here
    for (auto &ob: obstacles) {
        if (ob.x == new_xH && ob.y == new_yH) {
            snakeH.alive = false;
        }
    }
}

void Game::UpdatePathAstar() {
    // update path
    astar = AStar(grid_width, grid_height);
    // add snake body to obstacles
    for (auto &item: snake.body) {
        SDL_Point ob;
        ob.x = item.x;
        ob.y = item.y;
        obstacles.emplace_back(std::move(ob));
    }
    astar.update_obs(obstacles);

    // get new path
    SDL_Point start_point;
    start_point.x = static_cast<int>(snake.head_x);
    start_point.y = static_cast<int>(snake.head_y);
    astar.AStarSearch(start_point, food);
    path = astar.path;

    // remove snake body from obstacles
    int ob_cnt = snake.body.size();
    for (int i = 0; i < ob_cnt; i++) {
        obstacles.pop_back();
    }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

int Game::GetScoreH() const { return scoreH; }
int Game::GetSizeH() const { return snakeH.size; }