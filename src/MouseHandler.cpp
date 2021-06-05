#include "MouseHandler.hpp"
#include "Logger.hpp"

#include <algorithm>

MouseHandler::MouseHandler()
{
    _normalCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    _handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
}

MouseHandler::~MouseHandler()
{
    _hoveredNode = nullptr;
}

bool MouseHandler::addSubscriber(Node* node)
{
    if(node == nullptr)
    {
        logError("[MouseHandler] Cannot add subscriber, node = nullptr.");
        return false;
    }

    _subscribers.push_back(node);
    logInfo("[MouseHandler] Subscriber added : " + node->getName());
    return true;
}

bool MouseHandler::removeSubscriber(Node* node)
{
    if(node == nullptr)
    {
        logError("[MouseHandler] Cannot remove subscriber, node = nullptr.");
        return false;
    }

    std::string name = node->getName();
    auto search = std::find(_subscribers.begin(), _subscribers.end(), node);
    if(search != _subscribers.end())
    {
        _subscribers.erase(search);
        logInfo("[MouseHandler] Subscriber removed : " + name);
        return true;
    }
    else
    {
        logError("[MouseHandler] Cannot remove subscriber " + node->getName() + ", not found.");
        return false;
    }
}

void MouseHandler::motion()
{
    bool hover = false;
    SDL_Point cursor_pos;
    SDL_GetMouseState(&(cursor_pos.x), &(cursor_pos.y));
    for(auto node : _subscribers)
    {
        if(node->isVisible() && node->isClickable() && SDL_PointInRect(&cursor_pos, node->getDestination()))
        {
            hover = true;
            if(_hoveredNode != node)
            {
                logInfo("[MouseHandler] Hovering clickable node " + node->getName());
                _hoveredNode = node;
            }
            break;
        }
    }
    
    if(hover)
        this->handCursor();
    else
    {
        this->normalCursor();
        _hoveredNode = nullptr;
    }
}

void MouseHandler::click()
{
    if(_hoveredNode == nullptr)
    {
        logInfo("[MouseHandler] Click registered but cursor is not on a clickable element.");
    }
    else
    {
        logInfo("[MouseHandler] " + _hoveredNode->getName() + " clicked.");
        _hoveredNode->click();
    }
}
