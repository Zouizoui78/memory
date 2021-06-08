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
        uint32_t suit,
        uint32_t rank,
        SDL_Texture* frontTexture
    );

    ~Card();


    //===============
    // Setters
    //===============

    /**
     * @brief Set the Back Texture
     * rendered when card is not revealed.
     * 
     * @param texture Texture to use.
     */
    static bool setBackTexture(SDL_Texture* texture);

    /**
     * @brief Set revealed state and change texture
     * if needed to reflect the card state.
     * 
     * @param revealed 
     */
    void setRevealed(bool revealed);


    //===============
    // Getters
    //===============
    
    static uint32_t getCardHeight() { return _cardHeight; }
    static uint32_t getCardWidth() { return _cardWidth; }
    static std::string getSuitName(int suit) { return _suits[suit]; }
    static std::string getRankName(int rank) { return _ranks[rank]; }

    /**
     * @brief Give the card key.
     * A key is an ID unique among different cards
     * but shared by identical cards.
     * 
     * @return uint32_t 
     */
    uint32_t getKey() { return _rank * 10 + _suit; }
    uint32_t getSuit() { return _suit; }
    uint32_t getRank() { return _rank; }

    bool getRevealed() { return _revealed; }

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
    static std::string generateName(uint32_t suit, uint32_t rank);

    bool _revealed = true;
    uint32_t _suit;
    uint32_t _rank;
    SDL_Texture* _frontTexture;

    static const uint32_t _cardWidth = 69;
    static const uint32_t _cardHeight = 94;
    static SDL_Texture* _backTexture;

    static std::vector<std::string> _suits;
    static std::vector<std::string> _ranks;
};

#endif // CARD