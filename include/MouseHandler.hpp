#ifndef MOUSEHANDLER
#define MOUSEHANDLER

#include <SDL.h>
#include "Node.hpp"

class MouseHandler
{
    public:
    MouseHandler();
    ~MouseHandler();

    void mouseEventHandler(SDL_Event event);
    bool addSubscriber(Node* node);
    bool removeSubscribe(Node* node);

    private:
    void mouseMotion(SDL_Event event);
    void mouseClick();

    void normalCursor() { SDL_SetCursor(_normalCursor); }
    void handCursor() { SDL_SetCursor(_handCursor); }

    std::vector<Node*> _subscribers;
    Node* _hoveredNode = nullptr;
    SDL_Cursor* _normalCursor;
    SDL_Cursor* _handCursor;
};

#endif // MOUSEHANDLER