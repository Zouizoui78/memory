#ifndef MOUSEHANDLER
#define MOUSEHANDLER

#include <SDL.h>
#include "Node.hpp"

class MouseHandler
{
    public:
    MouseHandler();
    ~MouseHandler();

    bool addSubscriber(Node* node);
    bool removeSubscriber(Node* node);

    /**
     * @brief Read current mouse cursor position
     * to determine which element is hovered.
     * 
     */
    void motion();

    /**
     * @brief Calls the hovered object's (if any)
     * click() method.
     * 
     */
    void click();

    void normalCursor() { SDL_SetCursor(_normalCursor); }
    void handCursor() { SDL_SetCursor(_handCursor); }

    private:
    std::vector<Node*> _subscribers;
    Node* _hoveredNode = nullptr;
    SDL_Cursor* _normalCursor;
    SDL_Cursor* _handCursor;
};

#endif // MOUSEHANDLER