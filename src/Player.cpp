#include "Player.hpp"
#include "Logger.hpp"

Player::Player(Renderer* renderer, std::string name, std::string playerName) :
    TextField(renderer, name, "-"),
    _playerName(playerName)
{
    this->setScore(0);
}

Player::~Player() {}

void Player::setScore(uint32_t score)
{
    _score = score;
    this->setText(_playerName + " : " + std::to_string(this->getScore()));
}

void Player::incScore()
{
    this->setScore(this->getScore() + 1);
}

void Player::setActive(bool active)
{
    _active = active;
    logInfo("[Player] Set activity of " + this->getName() + " to " + std::to_string(active));
}