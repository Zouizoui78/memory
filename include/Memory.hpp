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

    void update();
    bool getQuit();
    void eventHandler(SDL_Event event);

    private:

    void quit();

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
        std::vector<double> buttonYFactors,
        std::vector<bool (Memory::*)(Node*)> callbacks
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
    void updateRecord();

    Player* getActivePlayer();
    Player* getNextPlayer();


    //====================
    // State functions
    //====================

    void state1(Card* clicked);
    void state2(Card* clicked);
    void state3();
    void state4();


    //====================
    // Buttons functions
    //====================

    bool setPlayers(int players);
    bool onePlayer(Node* n);
    bool twoPlayers(Node* n);

    bool changePairs(int offset);
    bool incPairs(Node* n);
    bool incPairs10(Node* n);
    bool decPairs(Node* n);
    bool decPairs10(Node* n);

    /**
     * @brief Start a new game with the setting
     * from the main menu.
     * 
     * @return Ok or not.
     */
    bool start(Node* n);

    /**
     * @brief Reset the game state.
     * Destroy the running game
     * and brings back the main menu.
     */
    bool newGame(Node* n);

    bool buttonQuit(Node* n);


    //==========================
    // Input handling
    //==========================

    void keypress(int keycode);
    void motion();
    bool click();

    bool cardCallback(Node* clicked);
    void buttonCallback(Node* clicked);


    //==========================
    // High scores saving
    //==========================

    bool readSave();
    bool save();


    //==========================
    // Attributes
    //==========================

    bool _quit = false;
    bool _pause = false;

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
    int _pairs = 20;
    int _pairsFound = 0;

    uint8_t _minPairs = 2;
    uint8_t _maxPairs = 52;

    uint32_t _gameStartTime = 0;
    uint32_t _previousTimeChange = 0;
    uint32_t _gameDuration = 0;

    float _boardWidthRel = 0.85;

    std::vector<Player*> _players;

    std::vector<uint32_t> _highScores;
    std::string _savePath = "high_scores";

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

    std::vector<std::string> _mainMenuButtonsNames = { "button_one_player", "button_two_players", "button_inc_pairs", "button_inc_pairs_10","button_dec_pairs", "button_dec_pairs_10", "button_start", "button_quit" };
    std::vector<std::string> _gameMenuButtonsNames = { "button_new_game", "button_quit" };
    std::vector<std::string> _scoreFieldsNames = { "textfield_timer", "textfield_player" };
};

#endif // MEMORY