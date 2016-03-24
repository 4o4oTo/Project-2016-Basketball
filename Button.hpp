#ifndef BUTTON_HPP
#define BUTTON_HPP
#include "main.hpp"
#include "Texture.hpp"

class Button {
protected:
    SDL_Rect _BPosition;
    SDL_Color _BColor;
    Texture _BTexture;
    SDL_Rect _BClip;
public:
    Button();

    bool loadTextureFromFile(std::string path);

    bool loadTextureFromText(std::string text, SDL_Color color);

    void setDimensions(int x, int y, int w, int h);

    void free();

    virtual void handleEvents(SDL_Event* e) = 0;

    SDL_Color getColor();

    int getWidth();

    int getHeight();

    void render();
};
#endif
