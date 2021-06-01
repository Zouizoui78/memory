#include "Logger.hpp"
#include "Renderer.hpp"

#include <iostream>
#include <date/date.h>

int main(int argc, char*argv[])
{
    SDL_Event event;
    bool quit = false;

    Renderer r;
    if(!r.init())
    {
        return -1;
    }

    SDL_Texture* cards = r.loadImage("res/cards.bmp");
    SDL_Rect dst;

    SDL_Texture* card;
    dst.h = 92;
    dst.w = 64;
    dst.x = 0;
    dst.y = 0;
    r.CropTexture(cards, card, &dst);

    // dst.h = r.getHeight() / 2;
    // dst.w = r.getWidth() / 2;
    // dst.x = 0;
    // dst.y = 0;

    // r.renderTexture(cards, &dst, &dst);
    r.renderTexture(card, &dst);
    r.refresh();
    //Main loop.
    while(!quit){

        //Event loop.
        while(SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                logInfo("Game closed.");
                quit = true;
            }
        }
        SDL_Delay(20);
    }

    //Quit SDL.
    SDL_DestroyTexture(cards);
    r.stop();

    return 0;
}