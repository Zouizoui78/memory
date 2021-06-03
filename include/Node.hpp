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

    Node(Renderer* renderer, const std::string& name);

    Node(Renderer* renderer, const std::string& name, SDL_Texture* texture, SDL_Rect* destination = nullptr);

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

    /**
     * @brief Set the Renderer object.
     * No effect if renderer == nullptr.
     * 
     * @param renderer 
     */
    void setRenderer(Renderer* renderer);

    void setName(std::string name) { _name = name; }
    void setDestination(SDL_Rect* dst) { _destination = dst; }
    void setWidth(int width);
    void setHeight(int height);
    void setX(int x);
    void setY(int y);
    void setTexture(SDL_Texture* texture) { _texture = texture;}

    private:
    void setParent(Node* parent) { _parent = parent; }


    //===============
    // Others
    //===============
    public:
    void initialize_destination();


    //===============
    // Attributes
    //===============

    protected:
    Renderer* _renderer;
    std::string _name;
    SDL_Rect* _destination;
    SDL_Texture* _texture;
    std::vector<Node*> _children;
    Node* _parent = nullptr;
};

#endif // NODE