#include "Memory.hpp"
#include "Logger.hpp"

#include <algorithm>

Memory::Memory(Renderer* renderer, SDL_Texture* spriteSheet, SDL_Texture* background) : Node(renderer, "root")
{
    _background = background;

    this->loadTextures(spriteSheet);
    Card::setBackTexture(_textureSet[Card::CLUBS][Card::SPECIAL]);

    SDL_Rect dst;
    dst.h = renderer->getHeight();
    dst.w = renderer->getWidth() * 0.8;
    dst.x = 0;
    dst.y = 0;
    _board = new Node(renderer, "board", background, dst);
    this->addChild(_board);
    _cardMouseHandler.setActionArea(dst);

    _mainMenu = this->createMainMenu();
    if(_mainMenu != nullptr)
    {
        TextField* pairs = new TextField(_renderer, "textfield_pairs", std::to_string(_pairs) + " paires");
        _mainMenu->addChild(pairs);
        pairs->centerX();
        pairs->setY(_mainMenu->getHeight() * 0.5);
        this->addChild(_mainMenu);
    }
    _buttonMouseHandler.setActionArea(_mainMenu->getGlobalDestination());
}

Memory::~Memory() {}

//====================
// Init functions
//====================

Node* Memory::createMainMenu()
{
    std::vector<std::string> texts = { "1 joueur", "2 joueurs", "+", "-", "Demarrer", "Quitter" };
    std::vector<double> yFactors = { 0.1, 0.2, 0.6, 0.6, 0.8, 0.9 };

    Node* menu = this->createMenu("main_menu", _mainMenuButtonsNames, texts, yFactors);
    if(menu != nullptr)
    {
        Node* buttonInc = menu->findChild("button_inc_pairs");
        buttonInc->setX(buttonInc->getX() + buttonInc->getWidth());

        Node* buttonDec = menu->findChild("button_dec_pairs");
        buttonDec->setX(buttonDec->getX() - buttonInc->getWidth());
    }
    return menu;
}

Node* Memory::createGameMenu()
{
    std::vector<std::string> texts = { "Menu", "Quitter" };
    std::vector<double> yFactors = { 0.8, 0.9 };

    Node* menu =  this->createMenu("game_menu", _gameMenuButtonsNames, texts, yFactors);
    _gameMenu = menu;
    return menu;
}

Node* Memory::createMenu(
    std::string menuName,
    std::vector<std::string> buttonNames,
    std::vector<std::string> buttonTexts,
    std::vector<double> buttonYFactors
)
{
    if(!(buttonNames.size() == buttonTexts.size() && buttonTexts.size() == buttonYFactors.size()))
    {
        logError("[Memory] Cannot create menu, provided vectors don't all have the same size.");
        return nullptr;
    }
    
    if(menuName == "")
    {
        logWarning("[Memory] Menu name is empty !");
        return nullptr;
    }

    SDL_Rect dst;
    dst.h = _renderer->getHeight();
    dst.w = _renderer->getWidth() * 0.2;
    dst.x = _renderer->getWidth() * 0.8;
    dst.y = 0;
    Node* menu = new Node(_renderer, menuName, nullptr, dst);

    for(size_t i = 0 ; i < buttonNames.size() ; ++i)
    {
        TextField* button = new TextField(_renderer, buttonNames[i], buttonTexts[i]);

        if(!menu->addChild(button))
        {
            logError("[Memory] Failed to add button " + button->getName() + " to " + menu->getName());
        }

        button->centerX();
        button->setY(menu->getHeight() * buttonYFactors[i]);
        button->setClickable(true);
        _buttonMouseHandler.addSubscriber(button);
    }

    return menu;
}

bool Memory::loadTextures(SDL_Texture* spriteSheet)
{
    logInfo("[Memory] Loading cards textures from sprite sheet.");
    bool ok = true;
    unsigned int loadedCount = 0;
    unsigned int total = 0;
    for (size_t i = 0; i <= Card::DIAMONDS; i++)
    {
        for (size_t j = 0; j <= Card::SPECIAL; j++)
        {
            SDL_Rect rect;
            rect.h = Card::getCardHeight();
            rect.w = Card::getCardWidth();
            rect.x = Card::getCardWidth() * j;
            rect.y = Card::getCardHeight() * i;
            SDL_Texture* texture = _renderer->createBlankRenderTarget(Card::getCardWidth(), Card::getCardHeight());
            bool subok = true;
            if(texture == nullptr)
            {
                logError("[Memory] Failed to create blank texture for card " + Card::getRankName(j) + " of " + Card::getSuitName(i));
                ok = false;
                subok = false;
            }

            if(!_renderer->cropTexture(spriteSheet, texture, &rect))
            {
                logError("[Memory] Failed to crop texture for card " + Card::getRankName(j) + " of " + Card::getSuitName(i));
                ok = false;
                subok = false;
            }

            if(subok)
                ++loadedCount;
            
            ++total;
            _textureSet[i][j] = texture;
        }
    }
    if(ok)
        logInfo("[Memory] Successfully loaded " + std::to_string(loadedCount) + "/" + std::to_string(total) + " textures.");
    else
        logWarning("[Memory] Failed to load some card texture.");
    return ok;
}

Card* Memory::randomCard()
{
    Card* card = nullptr;
    int i, j;
    bool duplicate = true;
    while(duplicate)
    {
        duplicate = false;
        i = rand() % (Card::DIAMONDS + 1);
        j = rand() % Card::SPECIAL;
        card = new Card(_renderer, i, j, _textureSet[i][j]);
        unsigned int key = card->getKey();

        //Loop through already rendered cards to check if the new one is a duplicate.
        for(auto card : _pickedCard)
        {
            //Check if duplicate.
            if(key == card->getKey())
                duplicate = true;
        }
    }

    //If not duplicate return the card.
    return card;
}

SDL_Rect Memory::randomDestination(int w, int h, int maxX, int maxY)
{
    SDL_Rect destination;
    destination.w = w ; destination.h = h;

    bool overlap = true;
    while(overlap)
    {
        overlap = false;
        
        destination.x = rand() % maxX;
        destination.y = rand() % maxY;

        //Loop through already rendered rectangles to check if the new one is overlapping with one.
        for(auto card : _pickedCard)
        {
            SDL_Rect currentRect = card->getDestination();
            //Check overlapping.
            if(SDL_HasIntersection(&destination, &currentRect))
            {
                overlap = true;
            }
        }
    }

    //If no overlapping assign the rectangle to the card.
    return destination;
}

void Memory::createPairs()
{
    for(unsigned int i = 0 ; i < _pairs ; ++i)
    {
        Card* card = this->randomCard();
        if(card == nullptr)
        {
            logError("[Memory] Failed to get a random card.");
            return;
        }

        Card* card2 = new Card(_renderer, card->getSuit(), card->getRank(), card->getTexture());

        this->prepareCard(card);
        this->prepareCard(card2);
    }
}

void Memory::prepareCard(Card* card)
{
    card->setDestination(this->randomDestination(
        card->getWidth(),
        card->getHeight(),
        _board->getWidth() - card->getWidth(),
        _board->getHeight() - card->getHeight()
    ));
    card->setClickable(true);
    _cardMouseHandler.addSubscriber(card);
    // card->flip();
    _pickedCard.push_back(card);
    _board->addChild(card);
}

void Memory::removeCard(Card* card)
{
    _board->removeChild(card);
    _cardMouseHandler.removeSubscriber(card);
    auto search = std::find(_pickedCard.begin(), _pickedCard.end(), card);
    _pickedCard.erase(search);
    delete card;
}


//====================
// Buttons functions
//====================

void Memory::setPlayers(int players)
{
    _playersNb = players;
    logInfo("[Memory] Players set to " + std::to_string(players));
}

void Memory::onePlayer()
{
    this->setPlayers(1);
}

void Memory::twoPlayers()
{
    this->setPlayers(2);
}

void Memory::changePairs(int offset)
{
    _pairs += offset;
    Node* pairs = _mainMenu->findChild("textfield_pairs");
    ((TextField*)pairs)->setText(std::to_string(_pairs) + " paires");

    Node* incButton = _mainMenu->findChild("button_inc_pairs");
    Node* decButton = _mainMenu->findChild("button_dec_pairs");
    if(_pairs == 52)
        incButton->setVisible(false);
    else if(_pairs != 52 && !incButton->isVisible())
        incButton->setVisible(true);
    else if(_pairs == 3)
        decButton->setVisible(false);
    else if(_pairs != 3 && !decButton->isVisible())
        decButton->setVisible(true);
}

void Memory::incPairs()
{
    this->changePairs(1);
    logInfo("[Memory] Increased pairs to " + std::to_string(_pairs));
}

void Memory::decPairs()
{
    this->changePairs(-1);
    logInfo("[Memory] Decreased pairs to " + std::to_string(_pairs));
}

/**
 * @brief Start a new game with the setting
 * from the main menu.
 * 
 * @return Ok or not.
 */
bool Memory::start()
{
    Node* menu = this->createGameMenu();
    if(menu == nullptr)
    {
        logError("[Memory] Cannot start game, game menu = nullptr.");
        return false;
    }
    this->addChild(menu);
    this->_mainMenu->setVisible(false);
    this->createPairs();

    return true;
}

/**
 * @brief Reset the game state.
 * Destroy the running game
 * and brings back the main menu.
 */
void Memory::newGame()
{
    this->_mainMenu->setVisible(true);
    for(auto node : this->_gameMenu->getChildren())
        _buttonMouseHandler.removeSubscriber(node);
    this->removeChild("game_menu", true);

    for(Node* node : _board->getChildren())
    {
        this->removeCard((Card*)node);
    }
    _pickedCard.clear();

    _state = 0;
}

void Memory::quit() {}


//==========================
// Input handling
//==========================

void Memory::key(int keycode)
{
    if(keycode == SDLK_SPACE)
    {
        Node* node = this->_mainMenu;
        node->setVisible(!node->isVisible());
    }
}

void Memory::motion()
{
    _cardMouseHandler.motion();
    _buttonMouseHandler.motion();
}

void Memory::click()
{
    _cardMouseHandler.click<Memory, Card>(this, &Memory::callback);
    _buttonMouseHandler.click<Memory, TextField>(this, &Memory::callback);
}

void Memory::callback(Card* clicked)
{
    if(clicked == nullptr)
        return;

    logInfo("[Memory] Card " + clicked->getName() + " clicked.");
    
    if(_state == 0)
    {
        clicked->flip();
        _revealedCards.first = clicked;
        clicked->setClickable(false);
        _state = 1;
    }

    else if(_state == 1)
    {
        clicked->flip();
        _revealedCards.second = clicked;
        if(_revealedCards.first != clicked && _revealedCards.first->getKey() == clicked->getKey())
        {
            _state = 3;
        }
        else
        {
            _state = 2;
        }
    }

    else if(_state == 2)
    {
        _revealedCards.first->flip();
        _revealedCards.first->setClickable(true);
        _revealedCards.second->flip();
        _revealedCards = { nullptr, nullptr };
        _state = 0;
    }

    else if(_state == 3)
    {
        this->removeCard(_revealedCards.first);
        this->removeCard(_revealedCards.second);
        _revealedCards = { nullptr, nullptr };
        _state = 0;
    }
}

void Memory::callback(TextField* clicked)
{
    if(clicked == nullptr)
        return;

    std::string name = clicked->getName();
    logInfo("[Memory] Button " + name + " clicked.");

    if(name == _mainMenuButtonsNames[0])
        onePlayer();

    else if(name == _mainMenuButtonsNames[1])
        twoPlayers();

    else if(name == _mainMenuButtonsNames[2])
        incPairs();

    else if(name == _mainMenuButtonsNames[3])
        decPairs();

    if(name == _mainMenuButtonsNames[4])
    {
        if(!this->start())
            logError("[Memory] Failed to start game.");
    }

    if(name == _gameMenuButtonsNames[0])
    {
        this->newGame();
    }
}
