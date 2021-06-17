#ifndef MOUSEHANDLER
#define MOUSEHANDLER

#include <SDL.h>
#include "Node.hpp"

class MouseHandler
{
    public:
    MouseHandler();
    MouseHandler(SDL_Rect action_area);
    ~MouseHandler();

    bool addSubscriber(Node* node);
    bool removeSubscriber(Node* node);

    void setActionArea(SDL_Rect action_area);
    SDL_Rect getActionArea();

    /**
     * @brief Call highlight() on hovered nodes or not.
     * 
     * @param highlight 
     */
    void setHighlight(bool highlight);

    /**
     * @brief Read current mouse cursor position
     * to determine which element is hovered.
     * 
     */
    void motion();

    /**
     * @brief Call the hovered node click callback.
     * 
     * @return Ok or not.
     */
    bool click();

    private:
    void normalCursor();
    void handCursor();

    SDL_Point getCursorPos();

    /**
     * @brief Return whether or not the mouse pointer
     * is in this object action area.
     * Return always true if the action area is empty.
     * 
     * @return true 
     * @return false 
     */
    bool isTargeted();

    private:
    std::vector<Node*> _subscribers;
    Node* _hoveredNode = nullptr;
    SDL_Cursor* _normalCursor;
    SDL_Cursor* _handCursor;
    bool _highlight = false;

    /**
     * @brief Area in which this handler should be active.
     */
    SDL_Rect _action_area;
};

#endif // MOUSEHANDLER