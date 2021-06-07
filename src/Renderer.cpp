#include "Renderer.hpp"
#include "Logger.hpp"

Renderer::Renderer()
{}

Renderer::~Renderer()
{}

bool Renderer::init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        logError("[Renderer] Failed to initialize SDL.");
        return false;
    }

    if(TTF_Init() != 0)
    {
        logError("[Renderer] Failed to initialize TTF.");
        return false;
    }

    if(!getScreenSize())
    {
        logError("[Renderer] Failed to get screen's size.");
        return false;
    }

    _window = SDL_CreateWindow(
        "Memory", // Title
        0,
        0,
        _width,
        _height,
        SDL_WINDOW_FULLSCREEN
    );
    if(_window == nullptr)
    {
        logError("[Renderer] Failed to create window.");
        return false;
    }

    _renderer = SDL_CreateRenderer(
        _window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    if(_renderer == nullptr)
    {
        logError("[Renderer] Failed to create renderer.");
        return false;
    }

    logInfo("[Renderer] Init done.");
    return true;
}

void Renderer::stop()
{
    SDL_DestroyRenderer(_renderer);
    _renderer = nullptr;

    SDL_DestroyWindow(_window);
    _window = nullptr;

    SDL_Quit();
}

void Renderer::refresh()
{
    SDL_RenderPresent(_renderer);
}

bool Renderer::clear()
{
    if(SDL_RenderClear(_renderer) == -1)
    {
        logError("[Renderer] Failed to clear renderer.");
        return false;
    }
    return true;
}

SDL_Texture* Renderer::getRenderTarget()
{
    return SDL_GetRenderTarget(_renderer);
}

bool Renderer::setRenderTarget(SDL_Texture* dst)
{
    if(SDL_SetRenderTarget(_renderer, dst) == -1)
    {
        logError("[Renderer] Failed to set rendering target.");
        return false;
    }
    return true;
}

bool Renderer::setViewport(SDL_Rect* rect)
{
    if(rect == nullptr)
    {
        logError("[Renderer] Cannot set viewport, nullptr.");
        return false;
    }

    if(SDL_RenderSetViewport(_renderer, rect) == -1)
    {
        logError("[Renderer] Failed to set viewport.");
        return false;
    }
    logInfo("[Renderer] Set viewport to h = " + std::to_string(rect->h) + ", w = " + std::to_string(rect->w) + ", x = " + std::to_string(rect->x) + ", y = " + std::to_string(rect->y));
    return true;
}

bool Renderer::setDrawColor(SDL_Color& color)
{
    if(SDL_SetRenderDrawColor(
        _renderer,
        color.r,
        color.g,
        color.b,
        color.a
    ) == -1)
    {
        logError("[Renderer] Failed to set draw color.");
        return false;
    }
    logInfo("[Renderer] Set drawing color to h = " + std::to_string(color.r) + ", w = " + std::to_string(color.g) + ", x = " + std::to_string(color.b) + ", y = " + std::to_string(color.a));
    return true;    
}

TTF_Font* Renderer::loadFont(std::string fontPath, int size)
{
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), size);
    if(font == nullptr)
        logError("[Renderer] Unable to load font.");
    else
        logInfo("[Renderer] Loaded font " + fontPath);
    return font;
}

SDL_Texture* Renderer::loadImage(std::string imgPath)
{
    if(imgPath.empty())
    {
        logError("[Renderer] Cannot load image, empty path.");
        return nullptr;
    }

    SDL_Surface* surface = SDL_LoadBMP(imgPath.c_str());
    if(surface == nullptr)
    {
        logError("[Renderer] Failed to create surface from image.");
        return nullptr;
    }

    SDL_Texture* texture = surfaceToTexture(surface);
    if(texture == nullptr)
    {
        logError("[Renderer] Failed to create texture from image.");
        return nullptr;
    }

    logInfo("[Renderer] Loaded image " + imgPath);
    return texture;
}

SDL_Texture* Renderer::loadText(
    std::string text,
    SDL_Color color,
    TTF_Font* font
)
{
    SDL_Surface* surface = nullptr;
    if(font == nullptr)
    {
        if(_default_font == nullptr)
        {
            logError("[Renderer] Cannot load text, no default font and no font provided.");
            return nullptr;
        }
        surface = TTF_RenderText_Solid(_default_font, text.c_str(), color);
    }
    else
    {
        surface = TTF_RenderText_Solid(font, text.c_str(), color);
    }

    if(surface == nullptr)
    {
        logError("[Renderer] Failed to create surface from text.");
        return nullptr;
    }

    SDL_Texture* texture = surfaceToTexture(surface);
    if(texture == nullptr)
    {
        logError("[Renderer] Failed to create texture from image.");
        return nullptr;
    }
    
    logInfo("[Renderer] Loaded text '" + text + "'");
    return texture;
}

bool Renderer::renderTexture(SDL_Texture* texture, SDL_Rect* dst, SDL_Rect* portion)
{
    if(texture == nullptr)
    {
        logError("[Renderer] Cannot render texture, nullptr.");
        return false;
    }

    if(SDL_RenderCopy(_renderer, texture, portion, dst) == -1)
    {
        logError("[Renderer] Failed to render texture.");
        return false;
    }
    return true;
}

bool Renderer::cropTexture(SDL_Texture* src, SDL_Texture*& dst, SDL_Rect* rect)
{
    if(src == nullptr)
    {
        logError("[Renderer] Cannot crop texture, source = nullptr.");
        return false;
    }

    if(rect == nullptr)
    {
        logError("[Renderer] Cannot crop texture, rectangle = nullptr.");
        return false;
    } 

    if(rect->h == 0 || rect->w == 0)
    {
        logError("[Renderer] Cannot crop texture, rectangle has either height or width at 0.");
        return false;
    }

    dst = this->createBlankRenderTarget(rect->w, rect->h);

    if(dst == nullptr)
    {
        logError("[Renderer] Failed to create target texture.");
        return false;
    }

    if(!this->renderToTexture(src, dst, rect))
    {
        logError("[Renderer] Failed to crop texture.");
        return false;
    }

    return true;
}

bool Renderer::renderRectangle(SDL_Rect* rect)
{
    if(SDL_RenderDrawRect(_renderer, rect) == -1)
    {
        logError("[Renderer] Failed to render rectangle.");
        return false;
    }
    return true;
}

bool Renderer::getScreenSize()
{
    SDL_DisplayMode mode = SDL_DisplayMode();
    if(SDL_GetDesktopDisplayMode(0, &mode) == -1)
    {
        logError("[Renderer] Failed to get display information.");
        return false;
    }
    _width = mode.w;
    _height = mode.h;
    return true;
}

SDL_Texture* Renderer::surfaceToTexture(SDL_Surface* surface)
{
    if(surface == nullptr)
    {
        logError("[Renderer] Cannot create texture from surface, nullptr.");
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    if(texture == nullptr)
    {
        logError("[Renderer] Failed to create texture from surface.");
        return nullptr;
    }

    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* Renderer::textureAsRenderingTarget(SDL_Texture* src, SDL_Rect* rect, bool freeSrc)
{
    SDL_Texture* dst = this->createBlankRenderTarget(rect->w, rect->h);

    if(dst == nullptr)
    {
        logError("[Renderer] Failed to create target texture.");
        return nullptr;
    }

    if(!this->renderToTexture(src, dst, rect))
    {
        logError("[Renderer] Failed to copy texture into a new rendering target texture.");
        return nullptr;
    }

    return dst;
}

bool Renderer::renderToTexture(SDL_Texture* src, SDL_Texture* dst, SDL_Rect* dstRect)
{
    // Backup rendering target.
    SDL_Texture* target = this->getRenderTarget();

    // Set render target to destination texture.
    if(!this->setRenderTarget(dst))
    {
        logError("[Renderer] Failed to set rendering target prior to render to texture.");
        return false;
    }

    if(this->renderTexture(src, nullptr, dstRect) == false)
    {
        logError("[Renderer] Failed to render texture to target texture.");
        return false;
    }

    // Restore rendering target.
    if(!this->setRenderTarget(target))
    {
        logError("[Renderer] Failed to set rendering target back to default.");
        return false;
    }

    return true;
}

SDL_Texture* Renderer::createBlankRenderTarget(int width, int height)
{
    return SDL_CreateTexture(_renderer, SDL_GetWindowPixelFormat(_window), SDL_TEXTUREACCESS_TARGET, width, height);
}