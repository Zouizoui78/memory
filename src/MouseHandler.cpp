#include "MouseHandler.hpp"
#include "Logger.hpp"

MouseHandler::MouseHandler()
{
    logInfo("salut");
    _normalCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    _handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
}

MouseHandler::~MouseHandler()
{
    _hoveredNode = nullptr;
}

void MouseHandler::mouseEventHandler(SDL_Event event)
{
    if(event.type == SDL_MOUSEMOTION)
        this->mouseMotion(event);
    else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        this->mouseClick();
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

void MouseHandler::mouseMotion(SDL_Event event)
{
    bool hover = false;
    SDL_Point cursor_pos = {
        event.button.x,
        event.button.y
    };
    for(auto node : _subscribers)
    {
        if(SDL_PointInRect(&cursor_pos, node->getDestination()) && node->isClickable())
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

void MouseHandler::mouseClick()
{
    if(_hoveredNode == nullptr)
    {
        logInfo("[MouseHandler] Click registered but cursor is not on a clickable element.");
    }
    else
    {
        _hoveredNode->click();
    }
}