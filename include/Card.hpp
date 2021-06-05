#ifndef CARD
#define CARD

#include "Node.hpp"

class Card : public Node
{
    public:

    enum Suit{
        CLUBS,
        SPADES,
        HEARTS,
        DIAMONDS
    };

    enum Rank{
        ACE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING,
        SPECIAL
    };

    Card(  
        Renderer* renderer,
        uint8_t suit,
        uint8_t rank,
        SDL_Texture* frontTexture,
        SDL_Rect* destination
    );

    ~Card();

    /**
     * @brief Set the Back Texture
     * rendered when card is not revealed.
     * 
     * @param texture Texture to use.
     */
    static bool setBackTexture(SDL_Texture* texture);

    /**
     * @brief Give the card key.
     * A key is an ID unique among different cards
     * but shared by identical cards.
     * 
     * @return uint8_t 
     */
    uint8_t getKey() { return _rank * 10 + _suit; }
    uint8_t getSuit() { return _suit; }
    uint8_t getRank() { return _rank; }

    bool getRevealed() { return _revealed; }

    /**
     * @brief Set revealed state and change texture
     * if needed to reflect the card state.
     * 
     * @param revealed 
     */
    void setRevealed(bool revealed);

    /**
     * @brief Calls setRevealed() to flip the card.
     */
    void flip();

    private:

    /**
     * @brief Generate a name for the node
     * based on its rank and suit.
     * 
     * @return generated name
     */
    static std::string generateName(uint8_t suit, uint8_t rank);

    bool _revealed = true;
    uint8_t _suit;
    uint8_t _rank;
    SDL_Texture* _frontTexture;

    static const uint8_t _cardWidth = 69;
    static const uint8_t _cardHeight = 94;
    static SDL_Texture* _backTexture;

    static std::vector<std::string> _suits;
    static std::vector<std::string> _ranks;
};

#endif // CARD