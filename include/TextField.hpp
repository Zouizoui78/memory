#ifndef TEXTFIELD
#define TEXTFIELD

#include "Node.hpp"

class TextField : public Node
{
    public:

    TextField(Renderer* renderer, std::string name, std::string text, TTF_Font* font = nullptr);

    ~TextField();

    std::string getText() { return _text; }

    /**
     * @brief Set the text contained in this object
     * and create a new texture to render it.
     * 
     * Font priority order is :
     * 1) Argument to this function
     * 2) Object's default font
     * 3) Renderer's default font.
     * 
     * @param text Text to display.
     * @param font Font to use, nullptr to use objects's font or renderer's.
     * @param color Color to use, defaults to object's default color.
     * @return Ok or not.
     */
    bool setText(
        std::string text,
        TTF_Font* font = nullptr,
        SDL_Color color = { 255, 255, 255, 255 }
    );

    SDL_Color getDefaultTextColor() { return _defaultColor; }
    void setDefaultTextColor(SDL_Color color) { _defaultColor = color; }

    TTF_Font* getDefaultFont() { return _defaultFont; }
    void setDefaultFont(TTF_Font* font) { _defaultFont = font; }

    protected:
    std::string _text;
    SDL_Color _defaultColor;
    TTF_Font* _defaultFont;
};

#endif // TEXTFIELD