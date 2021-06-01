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
        logError("[Renderer] Failed to load TTF.");
        return false;
    }

    if(!_getScreenSize())
    {
        logError("[Renderer] Failed to get screen's size.");
        return false;
    }

    _window = SDL_CreateWindow(
        "Memory", // Title
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _width,
        _height,
        SDL_WINDOW_BORDERLESS
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

bool Renderer::setViewport(const SDL_Rect* rect)
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
    return true;
}

void Renderer::setDefaultFont(TTF_Font* font)
{
    _default_font = font;
}

bool Renderer::setDrawColor(const SDL_Color& color)
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
    return true;    
}

SDL_Texture* Renderer::loadImage(const std::string& imgPath)
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

    SDL_Texture* texture = _surfaceToTexture(surface);
    if(texture == nullptr)
    {
        logError("[Renderer] Failed to create texture from image.");
        return nullptr;
    }
    logInfo("[Renderer] Image " + imgPath + " loaded.");
    return texture;
}

SDL_Texture* Renderer::loadText(
    const std::string& text,
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

    SDL_Texture* texture = _surfaceToTexture(surface);
    if(texture == nullptr)
    {
        logError("[Renderer] Failed to create texture from image.");
        return nullptr;
    }
    logInfo("[Renderer] Text '" + text + "' rendered.");
    return texture;
}

bool Renderer::renderTexture(SDL_Texture* texture, const SDL_Rect* dst, const SDL_Rect* portion)
{
    if(texture == nullptr)
    {
        logError("[Renderer] Cannot render texture, nullptr.");
        return false;
    }

    if(!SDL_RenderCopy(_renderer, texture, portion, dst) == -1)
    {
        logError("[Renderer] Failed to render texture.");
        return false;
    }
    return true;
}

bool Renderer::CropTexture(SDL_Texture* src, SDL_Texture*& dst, const SDL_Rect* rect)
{
    if(src == nullptr)
    {
        logError("[Renderer] Cannot extract texture part, source = nullptr.");
        return false;
    }

    if(rect == nullptr)
    {
        logError("[Renderer] Cannot extract texture part, rectangle = nullptr.");
        return false;
    } 

    if(rect->h == 0 || rect->w == 0)
    {
        logError("[Renderer] Cannot extract texture part, rectangle has either height or width at 0.");
        return false;
    }

    dst = SDL_CreateTexture(_renderer, SDL_GetWindowPixelFormat(_window), SDL_TEXTUREACCESS_TARGET, rect->w, rect->h);

    if(dst == nullptr)
    {
        logError("[Renderer] Failed to create target texture.");
        return false;
    }

    // Set render target to resulting texture.
    if(SDL_SetRenderTarget(_renderer, dst) == -1)
    {
        logError("[Renderer] Failed to set rendering target prior to extract texture part.");
        return false;
    }

    if(this->renderTexture(src, nullptr, rect) == false)
    {
        logError("[Renderer] Failed to render texture part to target texture.");
        return false;
    }

    // Reset rendering target to default (screen).
    if(SDL_SetRenderTarget(_renderer, NULL) == -1)
    {
        logError("[Renderer] Failed to set rendering target back to default.");
        return false;
    }

    return true;
}

bool Renderer::renderRectangle(const SDL_Rect* rect)
{
    if(!SDL_RenderDrawRect(_renderer, rect) == -1)
    {
        logError("[Renderer] Failed to render rectangle.");
        return false;
    }
    return true;
}

bool Renderer::_getScreenSize()
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

SDL_Texture* Renderer::_surfaceToTexture(SDL_Surface* surface)
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