#include "Card.hpp"
#include "Logger.hpp"

SDL_Texture* Card::_backTexture = nullptr;

std::vector<std::string> Card::_suits = {
    "clubs", "spades", "hearts", "diamonds"
};

std::vector<std::string> Card::_ranks = {
    "ace",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "jack",
    "queen",
    "king"
};

Card::Card(  
    Renderer* renderer,
    uint32_t suit,
    uint32_t rank,
    SDL_Texture* frontTexture
) :
    Node(renderer, this->generateName(suit, rank), frontTexture),
    _suit(suit),
    _rank(rank),
    _frontTexture(frontTexture)
{}

Card::~Card() {}

bool Card::setBackTexture(SDL_Texture* texture)
{
    if(texture == nullptr)
    {
        logError("[Card] Cannot set back texture, texture = nullptr.");
        return false;
    }
    _backTexture = texture;
    return true;
}

void Card::setRevealed(bool revealed)
{
    if(revealed)
        _texture = _frontTexture;
    else
        _texture = _backTexture;
    _revealed = revealed;
}

void Card::flip()
{
    this->setRevealed(!this->getRevealed());
}

std::string Card::generateName(uint32_t suit, uint32_t rank)
{
    return "card_" + _suits[suit] + "_" + _ranks[rank];
}
