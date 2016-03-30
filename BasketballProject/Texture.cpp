#include "Texture.hpp"

Texture::Texture() {
    Texture::_Texture = NULL;
    Texture::_THeight = 0;
    Texture::_TWidth = 0;
    Texture::_Text = "";
}

Texture::~Texture() {
    Texture::free();
}

bool Texture::loadFromFile(std::string path) {
    SDL_Texture* finalTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL) {
        printf("%s\n", IMG_GetError());
    }
    else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));

        finalTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(finalTexture == NULL) {
            printf("%s\n", SDL_GetError());
        }
        else {
            Texture::_THeight = loadedSurface->h;
            Texture::_TWidth = loadedSurface->w;
        }
        SDL_FreeSurface(loadedSurface);
    }
    Texture::_Texture = finalTexture;
    return (Texture::_Texture != NULL);
}

bool Texture::loadFromRenderedText(std::string textureText, SDL_Color textColor) {
    //Dispose of previously loaded textures
    Texture::free();

    Texture::_Text = textureText;
    //Render passed string
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if(textSurface == NULL) {
        printf("Failed to load rendered text! TTF_Error: %s\n", TTF_GetError());
    }
    else {
        Texture::_Texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if(Texture::_Texture == NULL) {
            printf("Failed to create texture from surface! SDL_Error: %s\n", SDL_GetError());
        }
        else {
            //Get texture dimensions
            Texture::_TWidth = textSurface->w;
            Texture::_THeight = textSurface->h;
        }
        //Free old surface
        SDL_FreeSurface(textSurface);
    }
    return Texture::_Texture != NULL;
}

void Texture::setColor(SDL_Color color) {
    SDL_SetTextureColorMod(Texture::_Texture, color.r, color.g, color.b);
}

int Texture::getHeight() {
    return Texture::_THeight;
}

int Texture::getWidth() {
    return Texture::_TWidth;
}

std::string Texture::getText() {
    return Texture::_Text;
}

void Texture::render(int x, int y, SDL_Rect* clip, SDL_RendererFlip flip) {
    SDL_Rect renderQuad = {x, y, Texture::_TWidth, Texture::_THeight};
    if(clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(gRenderer, Texture::_Texture, clip, &renderQuad, 0.0, NULL, flip);
}

void Texture::free() {
    if(Texture::_Texture != NULL) {
        SDL_DestroyTexture(Texture::_Texture);
        Texture::_Texture = NULL;
        Texture::_THeight = 0;
        Texture::_TWidth = 0;
    }
}
