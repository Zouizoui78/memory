#ifndef RENDERER
#define RENDERER

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

/**
 * This class handles everything directly related to rendering on screen.
 */
class Renderer
{
    public:
        Renderer();
        ~Renderer();

        /**
         * Renderer initialization.
         * 
         * @return Ok or not.
         */
        bool init();

        /**
         * Renderer stop.
         */
        void stop();

        /**
         * Render current back buffer.
         */
        void render();

        /**
         * Clear the screen and fill it with the current drawing color.
         * Ignore viewports.
         * 
         * @return Ok or not.
         */
        bool clear();

        /**
         * Get renderer's width.
         * 
         * @return Renderer's width.
         */
        int getWidth() { return _width; }

        /**
         * Get renderer's height.
         * 
         * @return Renderer's height.
         */
        int getHeight() { return _height; }

        /**
         * Set current rendering viewport to the rectangle.
         * 
         * @param rect Rectangle to use as viewport. If nullptr the entire screen is taken as a viewport.
         * 
         * @return Ok or not.
         */
        bool setViewport(SDL_Rect* rect = nullptr);

        /**
         * Set default font for text rendering.
         * 
         * @param font Font to set as default.
         */
        void setDefaultFont(TTF_Font* font);

        /**
         * Set the current rendering color used for rectangles, lines and clear.
         * 
         * @param color Color to be used.
         * 
         * @return Ok or not.
         */
        bool setDrawColor(SDL_Color color);

        /**
         * Load an image into a texture.
         * 
         * @param imgPath Path to the image to load.
         * 
         * @return texture or nullptr on error.
         */
        SDL_Texture* loadImage(std::string& imgPath);

        /**
         * Create a texture from a text.
         * 
         * @param text Text to render.
         * @param color Text color, default is white.
         * @param font Font to use. Default font is used if nullptr.
         * 
         * @return texture or nullptr on error.
         */
        SDL_Texture* loadText(
            std::string& text,
            SDL_Color color = { 255, 255, 255, 255 },
            TTF_Font* font = nullptr
        );

        /**
         * Render a texture.
         * 
         * @param texture The texture to render.
         * @param dst The destination rectangle in which the texture is rendered. Whole texture is rendered if nullptr.
         * @param portion The portion of the texture to copy to the destination. Entire destination is used if nullptr.
         * 
         * @return Ok or not.
         */
        bool renderTexture(SDL_Texture* texture, SDL_Rect* dst = nullptr, SDL_Rect* portion = nullptr);

        /**
         * Render the rectangle.
         * 
         * @param rect Rectangle to render.
         * 
         * @return Ok or not.
         */
        bool renderRectangle(SDL_Rect* rect);
    
    private:
        /**
         * Fill _width and _height with values taken from SDL.
         * 
         * @return Ok or not.
         */
        bool _getScreenSize();

        /**
         * Create a texture from a surface. The surface is freed.
         * 
         * @param surface
         * 
         * @return texture or nullptr on error.
         */
        SDL_Texture* _surfaceToTexture(SDL_Surface* surface);

        /**
         * Window's and renderer width.
         */
        int _width = 0;

        /**
         * Window's and renderer height.
         */
        int _height = 0;

        /**
         * Window in which the rendering is done.
         */
        SDL_Window* _window = nullptr;

        /**
         * Renderer.
         */
        SDL_Renderer* _renderer = nullptr;

        /**
         * Font used to render text, if not nullptr.
         */
        TTF_Font* _default_font = nullptr;
};

#endif // RENDERER