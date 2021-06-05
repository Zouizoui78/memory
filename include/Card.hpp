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
        unsigned int suit,
        unsigned int rank,
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
     * @return unsigned int 
     */
    unsigned int getKey() { return _rank * 10 + _suit; }
    unsigned int getSuit() { return _suit; }
    unsigned int getRank() { return _rank; }

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
    static std::string generateName(unsigned int suit, unsigned int rank);

    bool _revealed = true;
    unsigned int _suit;
    unsigned int _rank;
    SDL_Texture* _frontTexture;

    static const unsigned int _cardWidth = 69;
    static const unsigned int _cardHeight = 94;
    static SDL_Texture* _backTexture;

    static std::vector<std::string> _suits;
    static std::vector<std::string> _ranks;
};

#endif // CARD