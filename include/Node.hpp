#ifndef NODE
#define NODE

#include <string>
#include <vector>

#include <SDL.h>

#include "Renderer.hpp"
#include "Clickable.hpp"

/**
 * Graphic node abstraction.
 * Can hold children.
 */
class Node : public Clickable<Node>
{
    public:

    Node(
        Renderer* renderer,
        std::string name,
        SDL_Texture* texture = nullptr,
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
     * @param recursive Search in the whole tree from this node or not.
     * @return Pointer to the child or nullptr if not found.
     */
    Node* findChild(std::string name, bool recursive = false);


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

    Renderer* getRenderer();
    std::string getName();
    SDL_Rect getDestination();
    int getWidth();
    int getHeight();
    int getX();
    int getY();
    SDL_Texture* getTexture();
    std::vector<Node*> getChildren();
    Node* getParent();
    bool isInTree();

    /**
     * @brief Return true if this' destination
     * has width and height at 0.
     * 
     * @return yes/no
     */
    bool hasEmptyDestination();

    /**
     * @brief Return this' destination relatively to renderer origin.
     * 
     * @return SDL_Rect 
     */
    SDL_Rect getGlobalDestination();

    /**
     * @brief Return whether or not the node's destination
     * is at x = 0, y = 0.
     * 
     * @return true 
     * @return false 
     */
    bool isAtOrigin();

    /**
     * @brief Get whether this object is visible or not.
     * An invisible object and its children are not rendered nor clickable.
     * 
     * @return true 
     * @return false 
     */
    bool isVisible();

    /**
     * @brief Get whether this object will react
     * to input mouse events or not.
     * An invisible object is not clickable.
     * 
     * @return true 
     * @return false 
     */
    virtual bool isClickable() override;


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

    void setName(std::string name);
    void setDestination(SDL_Rect dst);
    void setWidth(int width);
    void setHeight(int height);
    void setSize(int width, int height);
    void setX(int x);
    void setY(int y);
    void setOrigin(int x, int y);
    void setTexture(SDL_Texture* texture);
    void setVisible(bool visible);

    void setParent(Node* parent);


    //===============
    // Others
    //===============
    protected:
    void initializeDestination();

    public:
    void centerX();
    void centerY();

    /**
     * @brief Highlight this node by drawing a rectangle around it.
     * 
     * @param color Defaults to red.
     * @return Ok or not.
     */
    bool highlight(SDL_Color color = { 255, 0, 0, 255 });


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
};

#endif // NODE
