#include "Logger.hpp"
#include "Node.hpp"

#include <algorithm>

Node::Node(Renderer* renderer, std::string name) :
    _renderer(renderer),
    _name(name),
    _texture(nullptr)
{
    this->initializeDestination();
    logInfo("[Node] Instanciated node " + _name);
}

Node::Node(Renderer* renderer, std::string name, SDL_Texture* texture, SDL_Rect destination) : 
    _renderer(renderer),
    _name(name),
    _destination(destination),
    _texture(texture)
{
    logInfo("[Node] Instanciated node " + _name);
}

Node::~Node()
{
    for(Node* child : _children)
    {
        delete child;
    }
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
    _children.push_back(child);
    logInfo("[Node] New child for " + _name + " : " + child->getName());
    return true;
}

bool Node::removeChild(Node* child)
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

bool Node::removeChild(std::string name)
{
    if(name.empty())
    {
        logError("[Node] Cannot remove child, name is empty.");
        return false;
    }
    
    Node* child = this->findChild(name);
    if(child != nullptr)
    {
        return this->removeChild(child);
    }
    else
    {
        logError("[Node] Cannot remove child " + name + " from " + _name + ", not found.");
        return false;
    }
}

Node* Node::findChild(std::string name)
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

    if(!_visible)
        return true;

    if(_texture != nullptr)
    {
        if(_parent != nullptr)
        {
            _destination.x += _parent->getX();
            _destination.y += _parent->getY();
        }

        SDL_Rect* rect;
        if(_destination.h == 0 && _destination .w == 0 && _destination .x == 0 && _destination.y == 0)
            rect = nullptr;
        else
            rect = &_destination;

        if(!_renderer->renderTexture(_texture, rect))
        {
            logError("[Node] Failed to render " + _name);
            return false;
        }

        if(_parent != nullptr)
        {
            _destination.x -= _parent->getX();
            _destination.y -= _parent->getY();
        }
    }

    if(!this->renderChildren())
    {
        logError("[Node] " + _name + " : failed to render one or more children.");
        return false;
    }

    return true;
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
// Setters
//===============

void Node::setRenderer(Renderer* renderer)
{
    if(renderer != nullptr)
    {
        _renderer = renderer;
    }
}

void Node::setWidth(int width)
{
    _destination.w = width;
}

void Node::setHeight(int height)
{
    _destination.h = height;
}

void Node::setX(int x)
{
    _destination.x = x;
}

void Node::setY(int y)
{
    _destination.y = y;
}


//===============
// Inputs
//===============

void Node::click()
{
    if(!_clickable)
    {
        logError("[Node] " + _name + " registered a click but is not set as clickable.");
    }
    else if(_click == nullptr)
    {
        logWarning("[Node] " + _name + " clicked but no callback is registered.");
    }
    else
    {
        _click(this);
    }
}


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
