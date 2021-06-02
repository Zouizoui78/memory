#include "Logger.hpp"
#include "Node.hpp"

#include <algorithm>

Node::Node(const std::string& name) :
    _name(name),
    _destination(nullptr),
    _texture(nullptr)
{}

Node::Node(const std::string& name, SDL_Texture* texture, SDL_Rect* destination) : 
    _name(name),
    _destination(destination),
    _texture(texture)
{}

Node::~Node()
{
    if(_destination != nullptr)
    {
        delete _destination;
    }

    if(_texture != nullptr)
    {
        SDL_DestroyTexture(_texture);
    }

    for(Node* child : _children)
    {
        delete child;
    }
    _children.clear();

    logInfo("[Node] Node " + _name + " freed.");
}


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
        delete *search;
        _children.erase(search);
        logInfo("[Node] Removed child " + name + " from " + _name);
        return true;
    }
    else
    {
        logError("[Node] Cannot remove child " + name + " from " + _name + ", not found.");
        return false;
    }
}

bool Node::removeChild(const std::string& name)
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

Node* Node::findChild(const std::string& name)
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

bool Node::render(Renderer* renderer)
{
    if(_texture == nullptr)
    {
        logError("[Node] Cannot render texture for node " + _name + ", _texture = nullptr");
        return false;
    }

    if(renderer == nullptr)
    {
        logError("[Node] Cannot render texture for node " + _name + ", renderer = nullptr");
        return false;
    }

    SDL_Rect dst = *_destination;
    if(_parent != nullptr)
    {
        dst.x += _parent->getX();
        dst.y += _parent->getY();
    }
    
    if(!renderer->renderTexture(_texture, &dst))
    {
        logError("[Node] Failed to render " + _name);
        return false;
    }
    if(!this->renderChildren(renderer))
    {
        logError("[Node] " + _name + " : failed to render one or more children.");
        return false;
    }

    return true;
}

bool Node::renderChildren(Renderer* renderer)
{
    bool ok = true;
    for(Node* child : _children)
    {
        if(!child->render(renderer))
            ok = false;
    }
    return ok;
}