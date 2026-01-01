#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <ctime>

const int WIDTH = 800;
const int HEIGHT = 600;

const int OBJECT_SIZE = 4;

const int ROWS = HEIGHT / OBJECT_SIZE;
const int COLUMNS = WIDTH / OBJECT_SIZE;

const int blank = 0;
const int sand = 1;
const int water = 2;
const int rock = 3;
int selectedObject = 1;
char *textContent = (char *)"SAND";

SDL_Color yellowColor = {235, 200, 50, 255};
SDL_Color blueColor = {0, 140, 255, 255};
SDL_Color greyColor = {145, 145, 145, 255};

SDL_Texture *texture = NULL;
SDL_FRect textRect = {10, 10, 0, 0};

std::vector<std::vector<int>> grid(COLUMNS, std::vector<int>(ROWS, 0));

void updateText(SDL_Renderer *renderer, TTF_Font *font, char *textContent, SDL_Color *color)
{
    if (texture != NULL)
    {
        SDL_DestroyTexture(texture);
    }
    SDL_Surface *tempSurface = TTF_RenderText_Solid(font, textContent, 0, *color);
    if (tempSurface)
    {
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        textRect.w = (float)tempSurface->w;
        textRect.h = (float)tempSurface->h;

        SDL_DestroySurface(tempSurface);
    }
}

int main(int argc, char *argv[])
{
    srand(time(0));

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL could not start: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!SDL_CreateWindowAndRenderer("Sand Simulation", WIDTH, HEIGHT, 0, &window, &renderer))
    {
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (!TTF_Init())
    {
        SDL_Log("Couldn't initialize SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    TTF_Font *font = TTF_OpenFont("ARIAL.TTF", 12);
    if (!font)
    {
        std::cerr << "Font load error: " << SDL_GetError() << std::endl;
        return -1;
    }

    bool running = true;
    bool scanLeftToRight = true;
    SDL_Event event;

    updateText(renderer, font, textContent, &yellowColor);

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (event.button.button == SDL_BUTTON_MIDDLE)
                {
                    selectedObject++;
                    if (selectedObject == 4)
                    {
                        selectedObject = 1;
                    }
                    if (selectedObject == 1)
                    {
                        textContent = (char *)"SAND";
                        updateText(renderer, font, textContent, &yellowColor);
                    }
                    else if (selectedObject == 2)
                    {
                        textContent = (char *)"WATER";
                        updateText(renderer, font, textContent, &blueColor);
                    }
                    else
                    {
                        textContent = (char *)"ROCK";
                        updateText(renderer, font, textContent, &greyColor);
                    }
                }
            }
        }

        float mouseX, mouseY;
        SDL_MouseButtonFlags mouseState = SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseState & SDL_BUTTON_LMASK)
        {
            int x = (int)mouseX / OBJECT_SIZE;
            int y = (int)mouseY / OBJECT_SIZE;

            if (y >= 0 && y < ROWS && x >= 0 && x < COLUMNS)
            {
                if (selectedObject == 1)
                    grid[x][y] = sand;
                else if (selectedObject == 2)
                    grid[x][y] = water;
                else
                    grid[x][y] = rock;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        scanLeftToRight = !scanLeftToRight;

        // Physics
        for (int y = ROWS - 1; y >= 0; y--)
        {
            for (int i = 0; i < COLUMNS; i++)
            {
                int x = scanLeftToRight ? i : (COLUMNS - 1 - i);

                if (grid[x][y] == sand)
                {
                    if (y + 1 < ROWS)
                    {
                        int below = grid[x][y + 1];

                        if (below == blank)
                        {
                            grid[x][y + 1] = sand;
                            grid[x][y] = blank;
                            continue;
                        }

                        if (below == water)
                        {
                            grid[x][y + 1] = sand;

                            bool pushRight = (rand() % 2 == 0);

                            if (pushRight)
                            {
                                if (x + 1 < COLUMNS && grid[x + 1][y] == blank)
                                {
                                    grid[x + 1][y] = water;
                                    grid[x][y] = blank;
                                }
                                else if (x - 1 >= 0 && grid[x - 1][y] == blank)
                                {
                                    grid[x - 1][y] = water;
                                    grid[x][y] = blank;
                                }
                                else
                                    grid[x][y] = water;
                            }
                            else
                            {
                                if (x - 1 >= 0 && grid[x - 1][y] == blank)
                                {
                                    grid[x - 1][y] = water;
                                    grid[x][y] = blank;
                                }
                                else if (x + 1 < COLUMNS && grid[x + 1][y] == blank)
                                {
                                    grid[x + 1][y] = water;
                                    grid[x][y] = blank;
                                }
                                else
                                    grid[x][y] = water;
                            }
                            continue;
                        }
                    }

                    bool tryRight = (rand() % 2 == 0);

                    if (tryRight)
                    {
                        if (y + 1 < ROWS && x + 1 < COLUMNS && (grid[x + 1][y + 1] == blank || grid[x + 1][y + 1] == water) && grid[x + 1][y] != rock)
                        {
                            int target = grid[x + 1][y + 1];
                            grid[x + 1][y + 1] = sand;
                            grid[x][y] = target;
                            continue;
                        }
                        if (y + 1 < ROWS && x - 1 >= 0 && (grid[x - 1][y + 1] == blank || grid[x - 1][y + 1] == water) && grid[x - 1][y] != rock)
                        {
                            int target = grid[x - 1][y + 1];
                            grid[x - 1][y + 1] = sand;
                            grid[x][y] = target;
                            continue;
                        }
                    }
                    else
                    {
                        if (y + 1 < ROWS && x - 1 >= 0 && (grid[x - 1][y + 1] == blank || grid[x - 1][y + 1] == water) && grid[x - 1][y] != rock)
                        {
                            int target = grid[x - 1][y + 1];
                            grid[x - 1][y + 1] = sand;
                            grid[x][y] = target;
                            continue;
                        }
                        if (y + 1 < ROWS && x + 1 < COLUMNS && (grid[x + 1][y + 1] == blank || grid[x + 1][y + 1] == water) && grid[x + 1][y] != rock)
                        {
                            int target = grid[x + 1][y + 1];
                            grid[x + 1][y + 1] = sand;
                            grid[x][y] = target;
                            continue;
                        }
                    }
                }
                else if (grid[x][y] == water)
                {
                    if (y + 1 < ROWS && grid[x][y + 1] == blank)
                    {
                        grid[x][y + 1] = water;
                        grid[x][y] = blank;
                        continue;
                    }
                    bool flowRight = (rand() % 2 == 0);

                    // sliding
                    if (flowRight)
                    {
                        if (y + 1 < ROWS && x + 1 < COLUMNS && grid[x + 1][y + 1] == blank && grid[x + 1][y] != rock)
                        {
                            grid[x + 1][y + 1] = water;
                            grid[x][y] = blank;
                            continue;
                        }
                        else if (y + 1 < ROWS && x - 1 >= 0 && grid[x - 1][y + 1] == blank && grid[x - 1][y] != rock)
                        {
                            grid[x - 1][y + 1] = water;
                            grid[x][y] = blank;
                            continue;
                        }
                    }
                    else
                    {
                        if (y + 1 < ROWS && x - 1 >= 0 && grid[x - 1][y + 1] == blank && grid[x - 1][y] != rock)
                        {
                            grid[x - 1][y + 1] = water;
                            grid[x][y] = blank;
                            continue;
                        }
                        if (y + 1 < ROWS && x + 1 < COLUMNS && grid[x + 1][y + 1] == blank && grid[x + 1][y] != rock)
                        {
                            grid[x + 1][y + 1] = water;
                            grid[x][y] = blank;
                            continue;
                        }
                    }

                    // spreading
                    if (flowRight)
                    {
                        if (x + 1 < COLUMNS && grid[x + 1][y] == blank)
                        {
                            grid[x + 1][y] = water;
                            grid[x][y] = blank;
                        }
                        else if (x - 1 >= 0 && grid[x - 1][y] == blank)
                        {
                            grid[x - 1][y] = water;
                            grid[x][y] = blank;
                        }
                    }
                    else
                    {
                        if (x - 1 >= 0 && grid[x - 1][y] == blank)
                        {
                            grid[x - 1][y] = water;
                            grid[x][y] = blank;
                        }
                        else if (x + 1 < COLUMNS && grid[x + 1][y] == blank)
                        {
                            grid[x + 1][y] = water;
                            grid[x][y] = blank;
                        }
                    }
                }
            }
        }

        for (int x = 0; x < COLUMNS; x++)
        {
            for (int y = 0; y < ROWS; y++)
            {
                if (grid[x][y] == sand)
                {
                    SDL_FRect rect;
                    rect.x = x * OBJECT_SIZE;
                    rect.y = y * OBJECT_SIZE;
                    rect.w = OBJECT_SIZE;
                    rect.h = OBJECT_SIZE;
                    SDL_SetRenderDrawColor(renderer, yellowColor.r, yellowColor.g, yellowColor.b, yellowColor.a);

                    SDL_RenderFillRect(renderer, &rect);
                }
                else if (grid[x][y] == water)
                {
                    SDL_FRect rect;
                    rect.x = x * OBJECT_SIZE;
                    rect.y = y * OBJECT_SIZE;
                    rect.w = OBJECT_SIZE;
                    rect.h = OBJECT_SIZE;
                    SDL_SetRenderDrawColor(renderer, blueColor.r, blueColor.g, blueColor.b, blueColor.a);

                    SDL_RenderFillRect(renderer, &rect);
                }
                else if (grid[x][y] == rock)
                {
                    SDL_FRect rect;
                    rect.x = x * OBJECT_SIZE;
                    rect.y = y * OBJECT_SIZE;
                    rect.w = OBJECT_SIZE;
                    rect.h = OBJECT_SIZE;
                    SDL_SetRenderDrawColor(renderer, greyColor.r, greyColor.g, greyColor.b, greyColor.a);

                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
        if (texture)
        {
            SDL_RenderTexture(renderer, texture, NULL, &textRect);
        }

        SDL_RenderPresent(renderer);
    }
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    SDL_Quit();

    return 0;
}