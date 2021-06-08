#ifndef PLAYER
#define PLAYER

#include "TextField.hpp"

class Player : public TextField
{
    public:
    Player(Renderer* renderer, std::string name, std::string playerName);
    ~Player();

    uint32_t getScore() { return _score; }
    void setScore(uint32_t score);
    void incScore();

    bool isActive() { return _active; }
    void setActive(bool active);

    private:
    uint32_t _score;
    bool _active = false;
    std::string _playerName;
};

#endif // PLAYER