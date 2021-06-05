#include "Logger.hpp"
#include "Renderer.hpp"
#include "TextField.hpp"
#include "MouseHandler.hpp"

void callbackTest(Node* node)
{
    logInfo(node->getName() + " clicked !");
}

void textCallback(Node* node)
{
    ((TextField*)node)->setText("Clicked !");
}

int main(int argc, char*argv[])
{
    SDL_Event event;
    bool quit = false;

    Renderer r;
    if(!r.init())
    {
        return -1;
    }
    
    MouseHandler mouseHandler;

    SDL_Texture* cards = r.loadImage("res/cards.bmp");
    SDL_Texture* background = r.loadImage("res/background.bmp");
    
    TTF_Font* font = r.loadFont("res/olivier.ttf", 90);
    r.setDefaultFont(font);
    
    SDL_Texture* cardTexture;
    SDL_Rect* dst = new SDL_Rect;
    dst->h = 94;
    dst->w = 69;
    dst->x = 0;
    dst->y = 0;
    r.cropTexture(cards, cardTexture, dst);
    Node* card = new Node(&r, "card", cardTexture, dst);
    
    SDL_Texture* cardTexture2;
    SDL_Rect* dst2= new SDL_Rect;
    dst2->h = 94;
    dst2->w = 69;
    dst2->x = 69;
    dst2->y = 0;
    r.cropTexture(cards, cardTexture2, dst2);
    dst2->x = r.getWidth() / 2 - 94 / 2;
    dst2->y = r.getHeight() / 2 - 69 / 2;
    Node* card2 = new Node(&r, "card2", cardTexture2, dst2);

    SDL_Rect* dst3 = new SDL_Rect;
    dst3->h = r.getHeight();
    dst3->w = r.getWidth() * 0.8;
    dst3->x = 0;
    dst3->y = 0;
    Node* board = new Node(&r, "board", background, dst3);
    board->addChild(card);
    board->addChild(card2);

    TextField* text = new TextField(&r, "text", "prout");
    text->setX(100);
    text->setY(200);
    board->addChild(text);
    
    text->setText("Ceci est un test !", font);
    text->setClickable(true);
    text->setClickCallback(&textCallback);
    mouseHandler.addSubscriber(text);

    SDL_Rect* menudst = new SDL_Rect;
    menudst->h = r.getHeight();
    menudst->w = r.getWidth();
    menudst->x = r.getWidth() * 0.8;
    menudst->y = 0;
    Node* menu = new Node(&r, "menu", nullptr, menudst);
    TextField* button = new TextField(&r, "button", "Bouton");
    menu->addChild(button);

    card->setClickable(true);
    mouseHandler.addSubscriber(card);
    card2->setClickable(true);
    card2->setClickCallback(&callbackTest);
    mouseHandler.addSubscriber(card2);

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
                mouseHandler.click();
            }
            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_ESCAPE)
                {
                    logInfo("closing");
                    quit = true;
                }
                else if(event.key.keysym.sym == SDLK_SPACE)
                {
                    // card2->setVisible(!card2->isVisible());
                    board->removeChild(card2);
                    mouseHandler.removeSubscriber(card2);
                    delete card2;
                }
            }
        }

        mouseHandler.motion();
        board->render();
        menu->render();
        r.refresh();
        SDL_Delay(100);
    }

    //Quit SDL.
    delete board;
    delete menu;
    logInfo("[Main] Board removed.");
    TTF_CloseFont(font);
    SDL_DestroyTexture(cards);
    r.stop();
    logInfo("[Main] Game closed.");

    return 0;
}
