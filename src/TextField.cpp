#include "TextField.hpp"
#include "Logger.hpp"

TextField::TextField(Renderer* renderer, const std::string& name) :
    Node(renderer, name),
    _defaultFont(nullptr)
{
    _text = "";

    _defaultColor.a = 255;
    _defaultColor.r = 255;
    _defaultColor.g = 255;
    _defaultColor.b = 255;

    _destination = new SDL_Rect;
}

TextField::TextField(Renderer* renderer, const std::string& name, const std::string& text, TTF_Font* font) :
    Node(renderer, name),
    _defaultFont(nullptr)
{
    _defaultColor.a = 255;
    _defaultColor.r = 255;
    _defaultColor.g = 255;
    _defaultColor.b = 255;

    _destination = new SDL_Rect;

    this->setText(text, font);
}

TextField::~TextField() {}

bool TextField::setText(
    const std::string& text,
    TTF_Font* font,
    SDL_Color color
)
{
    if(text.empty())
    {
        logError("[TextField] Cannot set text, string is empty.");
        return false;
    }

    // If default color is passed, use object's text color.
    if(color.a == 255 && color.b == 255 && color.g == 255 && color.r == 255)
    {
        logInfo("[TextField] setText() : using object's default color.");
        color = _defaultColor;
    }

    // Load text's texture with the right font.
    SDL_Texture* texture = nullptr;
    if(font != nullptr)
    {
        logInfo("[TextField] setText() : using provided font.");
        texture = _renderer->loadText(text, color, font);
    }
    else if(_defaultFont != nullptr)
    {
        logInfo("[TextField] setText() : using object's default font.");
        texture = _renderer->loadText(text, color, _defaultFont);
    }
    else if(_renderer->getDefaultFont() != nullptr)
    {
        logInfo("[TextField] setText() : using renderer's default font.");
        texture = _renderer->loadText(text, color, _renderer->getDefaultFont());
    }
    else
    {
        logError("[TextField] No font available. A font can be set in the renderer or in this object or passed to this function.");
        return false;
    }

    if(texture == nullptr)
    {
        logError("[TextField] Failed to load text '" + text + "' for node " + _name);
        return false;
    }

    int width;
    int height;
    if(SDL_QueryTexture(texture, nullptr, nullptr, &width, &height) == -1)
    {
        logError("[TextField] Failed to query texture size for node " + _name);
        return false;
    }

    // Everything ok

    if(_texture != nullptr)
    {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
    
    _text = text;
    _texture = texture;
    this->setWidth(width);
    this->setHeight(height);

    logInfo("[TextField] Set text for node " + _name + " to '" + _text + "'.");
    return true;
}