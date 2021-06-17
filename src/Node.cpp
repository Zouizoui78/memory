#include "Logger.hpp"
#include "Node.hpp"

#include <algorithm>

Node::Node(Renderer* renderer, std::string name, SDL_Texture* texture, SDL_Rect destination) : 
    _renderer(renderer),
    _name(name),
    _destination(destination),
    _texture(texture)
{
    if(SDL_RectEmpty(&destination) && _texture != nullptr)
        SDL_QueryTexture(_texture, nullptr, nullptr, &_destination.w, &_destination.h);
    logInfo("[Node] Instanciated node " + _name);
}

Node::~Node()
{
    for(Node* child : _children)
        delete child;
    _children.clear();
    logInfo("[Node] Removed node " + _name);
}


//===============
// Children
//===============

bool Node::addChild(Node* child)
{
    if(child == nullptr)
    {
        logError("[Node] Cannot add child to " + _name + ", child = nullptr.");
        return false;
    }

    child->setParent(this);
    child->_inTree = true;
    _children.push_back(child);
    logInfo("[Node] New child for " + _name + " : " + child->getName());
    return true;
}

bool Node::removeChild(Node* child, bool deleteNode)
{
    if(child == nullptr)
    {
        logError("[Node] Cannot remove child from " + _name + ", child = nullptr.");
        return false;
    }

    std::string name = child->getName();
    auto search = std::find(_children.begin(), _children.end(), child);
    if(search != _children.end())
    {
        Node* child = *search;
        if(deleteNode)
            delete child;
        else
        {
            child->setParent(nullptr);
            child->_inTree = false;
        }
        _children.erase(search);
        logInfo("[Node] Removed child from " + _name + " : " + name);
        return true;
    }
    else
    {
        logError("[Node] Cannot remove child " + name + " from " + _name + ", not found.");
        return false;
    }
}

bool Node::removeChild(std::string name, bool deleteNode)
{
    if(name.empty())
    {
        logError("[Node] Cannot remove child, name is empty.");
        return false;
    }
    
    Node* child = this->findChild(name);
    if(child != nullptr)
    {
        return this->removeChild(child, deleteNode);
    }
    else
    {
        logError("[Node] Cannot remove child " + name + " from " + _name + ", not found.");
        return false;
    }
}

Node* Node::findChild(std::string name, bool recursive)
{
    if(name.empty())
    {
        logError("[Node] Cannot find child, name is empty.");
        return nullptr;
    }

    for(Node* child : _children)
    {
        if(child->getName() == name)
            return child;
    }

    if(recursive)
    {
        for(Node* node : _children)
        {
            Node* result = node->findChild(name, recursive);
            if(result != nullptr)
                return result;
        }
    }

    return nullptr;
}


//===============
// Rendering
//===============

bool Node::render()
{
    if(_renderer == nullptr)
    {
        logError("[Node] Cannot render texture for node " + _name + ", renderer = nullptr");
        return false;
    }

    if(!this->isVisible())
        return true;

    bool ok = true;
    if(_texture != nullptr)
    {
        if(SDL_RectEmpty(&_destination))
            ok = _renderer->renderTexture(_texture, nullptr);
        else if(_parent->isAtOrigin())
            ok = _renderer->renderTexture(_texture, &_destination);
        else
        {
            SDL_Rect dest = this->getGlobalDestination();
            ok = _renderer->renderTexture(_texture, &dest);
        }

        if(!ok)
            logError("[Node] Failed to render " + _name);
    }

    if(!this->renderChildren())
    {
        logError("[Node] " + _name + " : failed to render one or more children.");
        ok = false;
    }

    return ok;
}

bool Node::renderChildren()
{
    bool ok = true;
    for(Node* child : _children)
    {
        if(!child->render())
            ok = false;
    }
    return ok;
}


//===============
// Getters
//===============

Renderer* Node::getRenderer() { return _renderer; }
std::string Node::getName() { return _name; }
SDL_Rect Node::getDestination() { return _destination; }
int Node::getX() { return _destination.x; }
int Node::getY() { return _destination.y; }
SDL_Texture* Node::getTexture() { return _texture; }
std::vector<Node*> Node::getChildren() { return _children; }
Node* Node::getParent() { return _parent; }
bool Node::isInTree() { return _inTree; }
bool Node::isClickable() { return Clickable::isClickable() && this->isVisible(); }
bool Node::isAtOrigin() { return _destination.x == 0 && _destination.y == 0; }

int Node::getWidth() 
{
    if(SDL_RectEmpty(&_destination))
        return _renderer->getWidth();
    return _destination.w;
}

int Node::getHeight()
{
    if(SDL_RectEmpty(&_destination))
        return _renderer->getHeight();
    return _destination.h;
}

SDL_Rect Node::getGlobalDestination()
{
    if(_parent == nullptr || _parent->isAtOrigin())
        return _destination;

    SDL_Rect ret = _destination;
    SDL_Rect parent = _parent->getGlobalDestination();
    ret.x += parent.x;
    ret.y += parent.y;
    return ret;
}

bool Node::isVisible()
{

    if(_parent != nullptr)
        return _visible && this->isInTree() && _parent->isVisible();
    else
        return _visible;
}


//===============
// Setters
//===============

void Node::setRenderer(Renderer* renderer)
{
    if(renderer != nullptr)
        _renderer = renderer;
}

void Node::setName(std::string name) { _name = name; }
void Node::setDestination(SDL_Rect dst) { _destination = dst; }
void Node::setWidth(int width) { _destination.w = width; }
void Node::setHeight(int height) { _destination.h = height; }
void Node::setX(int x) { _destination.x = x; }
void Node::setY(int y) { _destination.y = y; }
void Node::setTexture(SDL_Texture* texture) { _texture = texture; }

void Node::setSize(int width, int height)
{
    this->setWidth(width);
    this->setHeight(height);
}

void Node::setOrigin(int x, int y)
{
    this->setX(x);
    this->setY(y);
}

void Node::setVisible(bool visible)
{
    _visible = visible;
    logInfo("[Node] Set visibility of node " + _name + " to " + std::to_string(visible));
}

void Node::setParent(Node* parent) { _parent = parent; }


//===============
// Others
//===============

void Node::initializeDestination()
{
    _destination.h = 0;
    _destination.w = 0;
    _destination.x = 0;
    _destination.y = 0;
}

void Node::centerX()
{
    if(_parent != nullptr)
        _destination.x = _parent->getWidth() / 2 - _destination.w / 2;
}

void Node::centerY()
{
    if(_parent != nullptr)
        _destination.y = _parent->getHeight() / 2 - _destination.h / 2;
}

bool Node::highlight(SDL_Color color)
{
    SDL_Rect rect = this->getGlobalDestination();
    return _renderer->drawRectangle(&rect, color);
}
