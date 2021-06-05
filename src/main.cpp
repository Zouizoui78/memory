#include "Logger.hpp"
#include "Renderer.hpp"
#include "TextField.hpp"
#include "MouseHandler.hpp"
#include "Card.hpp"

#include <map>

typedef std::map<uint8_t, std::map<uint8_t, SDL_Texture*>> Deck;

void callbackTest(Node* node)
{
    logInfo(node->getName() + " clicked !");
}

void textCallback(Node* node)
{
    ((TextField*)node)->setText("Clicked !");
}

void cardCallback(Node* node)
{
    ((Card*)node)->flip();
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

    SDL_Texture* cardSpriteSheet = r.loadImage("res/cards.bmp");
    Deck deck;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 14; j++)
        {
            SDL_Rect rect;
            rect.h = 94;
            rect.w = 69;
            rect.x = 69 * j;
            rect.y = 94 * i;
            SDL_Texture* texture = r.createBlankRenderTarget(69, 94);
            r.cropTexture(cardSpriteSheet, texture, &rect);
            deck[i][j] = texture;
        }
    }
    Card::setBackTexture(deck[Card::CLUBS][Card::SPECIAL]);

    SDL_Texture* background = r.loadImage("res/background.bmp");
    
    TTF_Font* font = r.loadFont("res/olivier.ttf", 60);
    r.setDefaultFont(font);

    SDL_Rect* dst = new SDL_Rect;
    dst->h = r.getHeight();
    dst->w = r.getWidth() * 0.8;
    dst->x = 0;
    dst->y = 0;
    Node* board = new Node(&r, "board", background, dst);

    SDL_Rect* cardDst = new SDL_Rect;
    cardDst->h = 94;
    cardDst->w = 69;
    cardDst->x = 100;
    cardDst->y = 100;
    Card* card = new Card(&r, Card::DIAMONDS, Card::KING, deck[Card::DIAMONDS][Card::KING], cardDst);
    board->addChild(card);
    card->setClickable(true);
    card->setClickCallback(&cardCallback);
    mouseHandler.addSubscriber(card);

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
                    button->setText("space");
                    board->removeChild("diamonds_king");
                    delete card;
                    mouseHandler.removeSubscriber(card);
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
    SDL_DestroyTexture(cardSpriteSheet);
    // for (size_t i = 0; i < 4; i++)
    // {
    //     for (size_t j = 0; j < 13; j++)
    //     {
    //         SDL_DestroyTexture(deck[i][j]);
    //     }
    // }
    r.stop();
    logInfo("[Main] Game closed.");

    return 0;
}
