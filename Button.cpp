#include "Button.hpp"

Button::Button() {
    Button::_BPosition.x = 0;
    Button::_BPosition.y = 0;
    Button::_BPosition.w = 0;
    Button::_BPosition.h = 0;
    Button::_BColor = {0, 0, 0};
    Button::_BClip = {0, 0, 0, 0};
}

void Button::setDimensions(int x, int y, int w, int h) {
    Button::_BPosition.x = x;
    Button::_BPosition.y = y;
    Button::_BPosition.w = w;
    Button::_BPosition.h = h;
}

int Button::getHeight() {
    return Button::_BTexture.getHeight();
}

int Button::getWidth() {
    return Button::_BTexture.getWidth();
}

bool Button::loadTextureFromFile(std::string path) {
    return Button::_BTexture.loadFromFile(path);
}

bool Button::loadTextureFromText(std::string text, SDL_Color color) {
    return Button::_BTexture.loadFromRenderedText(text, color);
}

void Button::render() {
    if(Button::_BTexture.getText() != "") {
        if(Button::_BTexture.loadFromRenderedText(Button::_BTexture.getText(), Button::_BColor)) {
            Button::_BTexture.render(Button::_BPosition.x, Button::_BPosition.y);
        }
    }
    else {
        if(Button::_BClip.w != 0) {
            Button::_BTexture.render(Button::_BPosition.x, Button::_BPosition.y, &(Button::_BClip));
        } else {
            Button::_BTexture.render(Button::_BPosition.x, Button::_BPosition.y);
        }

    }
}

void Button::free() {
    Button::_BTexture.free();
}
