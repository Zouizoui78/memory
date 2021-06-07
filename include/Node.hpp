#ifndef NODE
#define NODE

#include <string>
#include <vector>

#include <SDL.h>

#include "Renderer.hpp"

/**
 * Graphic node abstraction.
 * Can hold children.
 */
class Node
{
    public:

    Node(Renderer* renderer, std::string name);

    Node(
        Renderer* renderer,
        std::string name,
        SDL_Texture* texture,
        SDL_Rect destination =
        {
            0, 0, 0, 0
        }
    );

    ~Node();


    //===============
    // Children
    //===============

    /**
     * @brief Add a child to the children.
     * 
     * @param child
     * @return Ok or not.
     */
    bool addChild(Node* child);

    /**
     * @brief Remove a child from the vector.
     * Does not call the child's destructor !
     * 
     * @param child
     * @param deleteNode whether the node is deleted (freed) or not.
     * @return Ok or not.
     */
    bool removeChild(Node* child, bool deleteNode = false);

    /**
     * @brief Remove child by name.
     * 
     * @param name
     * @param deleteNode whether the node is deleted (freed) or not.
     * @return Ok or not.
     */
    bool removeChild(std::string name, bool deleteNode = false);

    /**
     * @brief Search for a child by name.
     * 
     * @param name Name of the child to look for.
     * @return Pointer to the child or nullptr if not found.
     */
    Node* findChild(std::string name);


    //===============
    // Rendering
    //===============

    /**
     * @brief Render this node and its children.
     * 
     * @return Ok or not.
     */
    bool render();

    private:
    /**
     * @brief Render all children.
     * 
     * @return Ok or not.
     */
    bool renderChildren();

    public:


    //===============
    // Getters
    //===============

    Renderer* getRenderer() { return _renderer; }
    std::string getName() { return _name; }
    SDL_Rect getDestination() { return _destination; }

    /**
     * @brief Return this' destination relatively to renderer origin.
     * 
     * @return SDL_Rect 
     */
    SDL_Rect getGlobalDestination();

    int getWidth() { return _destination.w; }
    int getHeight() { return _destination.h; }
    int getX() {return _destination.x; }
    int getY() { return _destination.y; }
    SDL_Texture* getTexture() { return _texture; }
    std::vector<Node*> getChildren() { return _children; }
    Node* getParent() { return _parent; }
    bool isInTree();

    /**
     * @brief Get whether this object is visible or not.
     * An invisible object and its children are not rendered.
     * 
     * @return true 
     * @return false 
     */
    bool isVisible();

    /**
     * @brief Get whether this object will react
     * to input mouse events or not.
     * 
     * @return true 
     * @return false 
     */
    bool isClickable();


    //===============
    // Setters
    //===============

    /**
     * @brief Set the Renderer object.
     * No effect if renderer == nullptr.
     * 
     * @param renderer 
     */
    void setRenderer(Renderer* renderer);

    void setName(std::string name) { _name = name; }
    void setDestination(SDL_Rect dst) { _destination = dst; }
    void setWidth(int width);
    void setHeight(int height);
    void setSize(int width, int height) { _destination.w = width; _destination.h = height; }
    void setX(int x);
    void setY(int y);
    void setOrigin(int x, int y) { _destination.x = x; _destination.y = y; }
    void setTexture(SDL_Texture* texture) { _texture = texture; }
    void setVisible(bool visible) { _visible = visible; }
    void setClickable(bool clickable) { _clickable = clickable; }

    private:
    void setParent(Node* parent) { _parent = parent; }


    //===============
    // Others
    //===============
    public:
    void initializeDestination();

    void centerX();
    void centerY();


    //===============
    // Attributes
    //===============

    protected:
    Renderer* _renderer;
    std::string _name;
    SDL_Rect _destination;
    SDL_Texture* _texture;
    std::vector<Node*> _children;
    Node* _parent = nullptr;
    bool _inTree = false;
    bool _visible = true;
    bool _clickable = false;
};

#endif // NODE