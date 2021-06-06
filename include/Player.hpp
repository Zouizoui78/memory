#ifndef PLAYER
#define PLAYER

class Player
{
    public:
    Player();
    ~Player();

    unsigned int getScore() { return _score; }
    void setScore(unsigned int score) { _score = score; }

    bool isActive() { return _active; }
    void setActive(bool active) { _active = active; }

    private:
    unsigned int _score = 0;
    bool _active = false;
};

#endif // PLAYER