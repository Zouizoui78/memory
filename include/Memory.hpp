#ifndef MEMORY
#define MEMORY

#include "TextField.hpp"
#include "Card.hpp"
#include "MouseHandler.hpp"
#include "Player.hpp"

#include <map>

class Memory : public Node
{
    public:
    Memory(Renderer* renderer, SDL_Texture* spriteSheet, SDL_Texture* background);
    ~Memory();


    //==========================
    // Util functions
    //==========================

    public:

    void update();
    bool getQuit();

    void quit();

    private:

    Node* createMainMenu();
    Node* createGameMenu();

    /**
     * @brief Create a menu node.
     * 
     * @param menuName Node name.
     * @param buttonNames Node names for the buttons.
     * @param buttonTexts Text to display on the buttons.
     * @param buttonYFactors Factor to apply on y position.
     * @param callbacks Callbacks to bind to the buttons.
     * @return Menu node, nullptr if failure.
     */
    Node* createMenu(
        std::string menuName,
        std::vector<std::string> buttonNames,
        std::vector<std::string> buttonTexts,
        std::vector<double> buttonYFactors
    );

    /**
     * @brief Load the cards textures
     * from the spritesheet.
     * 
     * @param spriteSheet 
     * @return Ok or not.
     */
    bool loadTextures(SDL_Texture* spriteSheet);

    /**
     * @brief Return a random card from the deck.
     * 
     * @return Card 
     */
    Card* randomCard();

    /**
     * @brief Find a random placement on screen
     * and avoid overlapping.
     * 
     * @param w Width value of the generated rectangle
     * @param h Height value of the generated rectangle
     * @param maxX Maximum x value of the generated rectangle.
     * @param maxY Maximum y value of the generated rectangle.
     * @return SDL_Rect 
     */
    SDL_Rect randomDestination(int w, int h, int maxX, int maxY);

    void createPairs();
    void prepareCard(Card* card, std::string suffixe);
    void removeCard(Card* card);

    std::string ticksToString(uint32_t ticks);
    void updateTimer();

    Player* getActivePlayer();
    Player* getNextPlayer();


    //====================
    // Buttons functions
    //====================

    void setPlayers(int players);
    void onePlayer();
    void twoPlayers();

    void changePairs(int offset);
    void incPairs();
    void decPairs();

    /**
     * @brief Start a new game with the setting
     * from the main menu.
     * 
     * @return Ok or not.
     */
    bool start();

    /**
     * @brief Reset the game state.
     * Destroy the running game
     * and brings back the main menu.
     */
    void newGame();


    //==========================
    // Input handling
    //==========================

    public:
    void key(int keycode);
    void motion();
    void click();

    void callback(Card* clicked);
    void callback(TextField* clicked);


    //==========================
    // Attributes
    //==========================

    private:

    bool _quit = false;

    /**
     * @brief Describe the game state.
     * 0 = menu
     * 1 = no card revealed
     * 2 = one card revealed
     * 3 = two cards revealed
     * 4 = pair found
     */
    uint32_t _state = 0;

    uint32_t _playersNb = 1;
    uint32_t _pairs = 20;
    uint32_t _pairsFound = 0;

    uint32_t _gameStartTime = 0;
    uint32_t _previousTimeChange = 0;

    std::vector<Player*> _players;

    typedef std::map<uint8_t, std::map<uint8_t, SDL_Texture*>> TextureSet;
    TextureSet _textureSet;

    typedef std::map<uint8_t, std::map<uint8_t, Card*>> Deck;
    Deck _deck;

    std::vector<Card*> _pickedCard;
    std::pair<Card*, Card*> _revealedCards;

    SDL_Texture* _background;

    Node* _board = nullptr;
    Node* _gameMenu = nullptr;
    Node* _mainMenu = nullptr;

    MouseHandler _buttonMouseHandler;
    MouseHandler _cardMouseHandler;

    std::vector<std::string> _mainMenuButtonsNames = { "button_one_player", "button_two_players", "button_inc_pairs", "button_dec_pairs", "button_start", "button_quit" };
    std::vector<std::string> _gameMenuButtonsNames = { "button_new_game", "button_quit" };
    std::vector<std::string> _scoreFieldsNames = { "textfield_timer", "textfield_player" };
};

#endif // MEMORY