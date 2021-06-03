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
     * @brief Renderer initialization.
     * 
     * @return Ok or not.
     */
    bool init();

    /**
     * @brief Renderer stop.
     */
    void stop();

    /**
     * @brief Render current back buffer.
     */
    void refresh();

    /**
     * @brief Clear the screen and fill it with the current drawing color.
     * Ignore viewports.
     * 
     * @return Ok or not.
     */
    bool clear();

    int getWidth() { return _width; }

    int getHeight() { return _height; }

    TTF_Font* getDefaultFont() { return _default_font; }

    SDL_Texture* getRenderTarget();
    bool setRenderTarget(SDL_Texture* dst);

    /**
     * @brief Set current rendering viewport to the rectangle.
     * 
     * @param rect Rectangle to use as viewport. If nullptr the entire screen is taken as a viewport.
     * @return Ok or not.
     */
    bool setViewport(SDL_Rect* rect = nullptr);

    /**
     * @brief Set default font for text rendering.
     * 
     * @param font Font to set as default.
     */
    void setDefaultFont(TTF_Font* font) { _default_font = font; }

    /**
     * @brief Set the current rendering color used for rectangles, lines and clear.
     * 
     * @param color Color to be used.
     * @return Ok or not.
     */
    bool setDrawColor(SDL_Color& color);

    /**
     * @brief Load a font.
     * 
     * @param fontPath Path to the font to load.
     * @param size Point size to load the font fe.
     * @return TTF_Font* nullptr if error.
     */
    TTF_Font* loadFont(std::string fontPath, int size);

    /**
     * @brief Load an image into a texture.
     * 
     * @param imgPath Path to the image to load.
     * @return texture or nullptr on error.
     */
    SDL_Texture* loadImage(std::string imgPath);

    /**
     * @brief Create a texture from a text.
     * 
     * @param text Text to render.
     * @param color Text color, default is white.
     * @param font Font to use. Default font is used if nullptr.
     * @return texture or nullptr on error.
     */
    SDL_Texture* loadText(
        std::string text,
        SDL_Color color = { 255, 255, 255, 255 },
        TTF_Font* font = nullptr
    );

    /**
     * @brief Render a texture.
     * 
     * @param texture The texture to render.
     * @param dst Part of the rendering target in which the texture is rendered. Whole rendering target if nullptr.
     * @param portion The portion of the texture to copy to the destination. Entire destination is used if nullptr.
     * @return Ok or not.
     */
    bool renderTexture(SDL_Texture* texture, SDL_Rect* dst = nullptr, SDL_Rect* portion = nullptr);

    /**
     * @brief Extract a part of a texture.
     * 
     * @param source Texture to extract a part of.
     * @param dst Texture to render to. Will be erased.
     * @param rect Part to extract.
     * @return Ok or not.
     */
    bool cropTexture(SDL_Texture* src, SDL_Texture*& dst, SDL_Rect* rect);

    /**
     * @brief Render the rectangle.
     * 
     * @param rect Rectangle to render.
     * @return Ok or not.
     */
    bool renderRectangle(SDL_Rect* rect);

    /**
     * @brief Fill _width and _height with values taken from SDL.
     * 
     * @return Ok or not.
     */
    bool getScreenSize();

    /**
     * @brief Create a texture from a surface. The surface is freed.
     * 
     * @param surface
     * @return texture or nullptr on error.
     */
    SDL_Texture* surfaceToTexture(SDL_Surface* surface);

    /**
     * @brief Turns a texture into a rendering target.
     * 
     * @param texture Source texture
     * @param freeSrc Whether to free the source texture or not, default = true.
     * @return new texture, nullptr if error.
     */
    SDL_Texture* textureAsRenderingTarget(SDL_Texture* src, SDL_Rect* rect, bool freeSrc = true);

    /**
     * @brief Render a texture into another texture.
     * 
     * @param src Texture to render.
     * @param dst Texture to render into.
     * @param dstRect Destination rectangle.
     * @return true Success
     * @return false Failure
     */
    bool renderToTexture(SDL_Texture* src, SDL_Texture* dst, SDL_Rect* dstRect);

    /**
     * @brief Create a Blank Render Target object.
     * 
     * @param width 
     * @param height 
     * @return SDL_Texture* nullptr if failure.
     */
    SDL_Texture* createBlankRenderTarget(int width, int height);
    

    private:

    /**
     * @brief Window's and renderer width.
     */
    int _width = 0;

    /**
     * @brief Window's and renderer height.
     */
    int _height = 0;

    /**
     * @brief Window in which the rendering is done.
     */
    SDL_Window* _window = nullptr;

    /**
     * @brief Renderer.
     */
    SDL_Renderer* _renderer = nullptr;

    /**
     * @brief Font used to render text, if not nullptr.
     */
    TTF_Font* _default_font = nullptr;
};

#endif // RENDERER