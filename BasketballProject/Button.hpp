#ifndef BUTTON_HPP
#define BUTTON_HPP
#include "main.hpp"
#include "Texture.hpp"

class Button {
protected:
    SDL_Rect _BPosition;
    SDL_Color _BColor;
public:
    Button();

    void setDimensions(int x, int y, int w, int h);

    virtual void handleEvents(SDL_Event* e) = 0;

    SDL_Color getColor();

    void render(Texture* t, SDL_Rect* clip = NULL);
};
#endif
