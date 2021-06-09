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
     * @brief Calls the passed callback.
     * 
     * @tparam T Type of the object holding the callback.
     * @tparam TClicked Type of the clicked object.
     * @param object Object holding the callback.
     * @param callback Callback to call.
     */
    template<class T, class TClicked>
    void click(T* object, void (T::*callback)(TClicked* clicked))
    {
        object->callback((TClicked*)_hoveredNode);
    }

    void normalCursor();
    void handCursor();

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