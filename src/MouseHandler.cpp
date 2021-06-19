#include "MouseHandler.hpp"
#include "Logger.hpp"

#include <algorithm>

MouseHandler::MouseHandler(SDL_Rect action_area) : _action_area(action_area)
{
    _normalCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    _handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
}

MouseHandler::~MouseHandler()
{
    _hoveredNode = nullptr;
    _subscribers.clear();
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

void MouseHandler::setActionArea(SDL_Rect action_area)
{
    _action_area = action_area;
}

SDL_Rect MouseHandler::getActionArea()
{
    return _action_area;
}

void MouseHandler::setHighlight(bool highlight)
{
    _highlight = highlight;
}

void MouseHandler::motion()
{
    bool hover = false;
    if(this->isTargeted())
    {
        SDL_Point cursor_pos = this->getCursorPos();
        for(auto node : _subscribers)
        {
            SDL_Rect dest = node->getGlobalDestination();
            if(SDL_PointInRect(&cursor_pos, &dest) && node->isClickable())
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
        {
            if(_highlight)
            {
                SDL_Color color = { 100, 100, 100, 255};
                _hoveredNode->highlight(color);
            }
            this->handCursor();
        }
        else
            this->normalCursor();
    }

    // Set pointer to nullptr here to update it
    // even if cursor is not in action area.
    if(!hover)
        _hoveredNode = nullptr;
}

bool MouseHandler::click()
{
    if(this->isTargeted())
    {
        if(_hoveredNode == nullptr)
        {
            logInfo("[MouseHandler] Click registered but cursor is not on a clickable element.");
            return false;
        }
        else
        {
            logInfo("[MouseHandler] " + _hoveredNode->getName() + " clicked.");
            return _hoveredNode->click();
        }
    }
    return true;
}

void MouseHandler::normalCursor()
{
    SDL_SetCursor(_normalCursor);
}

void MouseHandler::handCursor()
{
    SDL_SetCursor(_handCursor);
}

SDL_Point MouseHandler::getCursorPos()
{
    SDL_Point cursor_pos;
    SDL_GetMouseState(&cursor_pos.x, &cursor_pos.y);
    return cursor_pos;
}

bool MouseHandler::isTargeted()
{
    SDL_Point cursor_pos = this->getCursorPos();
    return SDL_RectEmpty(&_action_area) || SDL_PointInRect(&cursor_pos, &_action_area);
}