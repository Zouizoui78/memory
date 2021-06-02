#include "Logger.hpp"
#include "Renderer.hpp"

#include "Node.hpp"

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
    SDL_Texture* background = r.loadImage("res/background.bmp");
    
    SDL_Texture* cardTexture;
    SDL_Rect dst;
    dst.h = 94;
    dst.w = 69;
    dst.x = 0;
    dst.y = 0;
    r.cropTexture(cards, cardTexture, &dst);
    Node* card = new Node("card", cardTexture, &dst);
    
    SDL_Texture* cardTexture2;
    SDL_Rect dst2;
    dst2.h = 94;
    dst2.w = 69;
    dst2.x = 69;
    dst2.y = 0;
    r.cropTexture(cards, cardTexture2, &dst2);
    dst2.h = 94;
    dst2.w = 69;
    dst2.x = r.getWidth() / 2 - 94 / 2;
    dst2.y = r.getHeight() / 2 - 69 / 2;
    Node* card2 = new Node("card2", cardTexture2, &dst2);

    SDL_Rect dst3;
    dst3.h = r.getHeight() / 2;
    dst3.w = r.getWidth() / 2;
    dst3.x = 0;
    dst3.y = 0;
    Node* parent = new Node("parent", background, &dst3);
    parent->addChild(card);
    parent->addChild(card2);
    parent->render(&r);

    dst3.x = r.getWidth() / 2;
    dst3.y = r.getHeight() / 2;
    parent->render(&r);

    r.refresh();

    //Main loop.
    while(!quit){

        //Event loop.
        while(SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                quit = true;
            }
            else if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_RETURN)
                {
                    quit = true;
                }
                if(event.key.keysym.sym == SDLK_SPACE)
                {
                    r.clear();
                    parent->removeChild("card");
                    parent->render(&r);
                    r.refresh();
                }
            }
        }
        SDL_Delay(20);
    }

    //Quit SDL.
    delete parent;
    logInfo("[Main] Parent removed.");
    SDL_DestroyTexture(cards);
    r.stop();
    logInfo("[Main] Game closed.");

    return 0;
}