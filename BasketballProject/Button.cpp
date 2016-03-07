#include "Button.hpp"

Button::Button() {
    Button::_BPosition.x = 0;
    Button::_BPosition.y = 0;
    Button::_BPosition.w = 0;
    Button::_BPosition.h = 0;
    Button::_BColor = {0, 0, 0};
}

void Button::setDimensions(int x, int y, int w, int h) {
    Button::_BPosition.x = x;
    Button::_BPosition.y = y;
    Button::_BPosition.w = w;
    Button::_BPosition.h = h;
}

SDL_Color Button::getColor() {
    return Button::_BColor;
}

void Button::render(Texture* t, SDL_Rect* clip) {
    if(t->getText() != "") {
        if(t->loadFromRenderedText(t->getText(), Button::_BColor)) {
            t->render(Button::_BPosition.x, Button::_BPosition.y);
        }
    }
    else {
        t->render(Button::_BPosition.x, Button::_BPosition.y, clip);
    }
}
