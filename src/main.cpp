#include <time.h>

#include "Logger.hpp"
#include "Renderer.hpp"
#include "Memory.hpp"

int main(int argc, char*argv[])
{
    srand(time(NULL));
    SDL_Event event;
    bool quit = false;

    Renderer r;
    if(!r.init())
    {
        return -1;
    }
    
    TTF_Font* font = r.loadFont("res/olivier.ttf", 45);
    r.setDefaultFont(font);

    SDL_Texture* cardSpriteSheet = r.loadImage("res/cards.bmp");
    SDL_Texture* background = r.loadImage("res/background.bmp");
    Memory memory(&r, cardSpriteSheet, background);

    //Main loop.
    while(!quit)
    {
        r.clear();

        //Event loop.
        while(SDL_PollEvent(&event) != 0)
        {
            if( event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT)
            {
                memory.click();
            }
            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_ESCAPE)
                {
                    logInfo("[Main] Closing.");
                    quit = true;
                }
                else
                {
                    memory.key(event.key.keysym.sym);
                }
            }
        }

        memory.motion();
        memory.render();
        r.refresh();
        SDL_Delay(100);
    }

    //Quit SDL.
    //Destroying textures
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(cardSpriteSheet);
    
    TTF_CloseFont(font);

    r.stop();
    return 0;
}
