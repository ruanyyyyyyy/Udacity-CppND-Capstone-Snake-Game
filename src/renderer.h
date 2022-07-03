#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"
#include "astar.h"
#include "snake.h"
#include <vector>

class Renderer {
public:
    Renderer(const std::size_t screen_width, const std::size_t screen_height,
             const std::size_t grid_width, const std::size_t grid_height);
    ~Renderer();

    void Render(Snake const snake, Snake const snakeH, SDL_Point const &food, std::vector<SDL_Point> &obstacles, std::vector<SDL_Point> &path_found);
    void UpdateWindowTitle(int score, int scoreH, int fps);

private:
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;

    const std::size_t screen_width;
    const std::size_t screen_height;
    const std::size_t grid_width;
    const std::size_t grid_height;
};

#endif