#include <time.h>

#include "Logger.hpp"
#include "Renderer.hpp"
#include "Memory.hpp"

int main(int argc, char*argv[])
{
    srand(time(NULL));
    SDL_Event event;

    logInit();

    Renderer r;
    if(!r.init())
    {
        return -1;
    }
    
    TTF_Font* font = r.loadFont("res/olivier.ttf", 45);
    if(font == nullptr)
        return -1;

    r.setDefaultFont(font);

    SDL_Texture* cardSpriteSheet = r.loadImage("res/cards.bmp");
    if(cardSpriteSheet == nullptr)
        return -1;

    SDL_Texture* background = r.loadImage("res/background.bmp");
    if(background == nullptr)
        return -1;

    Memory memory(&r, cardSpriteSheet, background);

    //Main loop.
    while(!memory.getQuit())
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
                    memory.quit();
                }
                else
                {
                    memory.keypress(event.key.keysym.sym);
                }
            }
        }

        memory.update();
        memory.render();
        r.refresh();
        SDL_Delay(10);
    }

    //Quit SDL.
    //Destroying textures
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(cardSpriteSheet);
    
    TTF_CloseFont(font);

    r.stop();
    return 0;
}
