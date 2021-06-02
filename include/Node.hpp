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

    Node(const std::string& name);

    Node(const std::string& name, SDL_Texture* texture, SDL_Rect* destination = nullptr);

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
     * @brief Find a child by pointer and remove it.
     * 
     * @param child
     * @return Ok or not.
     */
    bool removeChild(Node* child);

    /**
     * @brief Find a child by name and remove it.
     * 
     * @param name
     * @return Ok or not.
     */
    bool removeChild(const std::string& name);

    /**
     * @brief Search for a child by name.
     * 
     * @param name Name of the child to look for.
     * @return Pointer to the child or nullptr if not found.
     */
    Node* findChild(const std::string& name);


    //===============
    // Rendering
    //===============

    /**
     * @brief Render this node.
     * Render all children as well.
     * 
     * @param renderer Renderer to render into.
     * @return Ok or not.
     */
    bool render(Renderer* renderer);

    private:
    /**
     * @brief Render all children.
     * 
     * @param renderer Renderer to render into.
     * @return Ok or not.
     */
    bool renderChildren(Renderer* renderer);

    public:


    //===============
    // Getters
    //===============

    std::string getName() { return _name; }

    SDL_Rect* getDestination() { return _destination; }
    int getWidth() { return _destination->w; }
    int getHeight() { return _destination->h; }
    int getX() {return _destination->x; }
    int getY() { return _destination->y; }

    SDL_Texture* getTexture() { return _texture; }
    std::vector<Node*> getChildren() { return _children; }
    Node* getParent() { return _parent; }


    //===============
    // Setters
    //===============

    void setName(std::string name) { _name = name; }

    void setDestination(SDL_Rect* dst) { _destination = dst; }
    void setWidth(int width) { _destination->w = width; }
    void setHeight(int height) { _destination->h = height; }
    void setX(int x) { _destination->x = x; }
    void setY(int y) { _destination->y = y; }

    void setTexture(SDL_Texture* texture) { _texture = texture;}

    private:
    void setParent(Node* parent) { _parent = parent; }


    //===============
    // Attributes
    //===============

    private:
    std::string _name;
    SDL_Rect* _destination;
    SDL_Texture* _texture;
    std::vector<Node*> _children;
    Node* _parent = nullptr;
};

#endif // NODE