#include "BasketballPole.hpp"

BasketballPole::BasketballPole() {
    polePos.x = 0;
    polePos.y = 0;
    textureRealHeight = 0;
    textureRealWidth = 0;
}

Texture& BasketballPole::getTexture() {
    return poleTexture;
}

void BasketballPole::setPosition(int x, int y) {
    polePos.x = x;
    polePos.y = y;
}

int BasketballPole::getX() {
    return polePos.x;
}

int BasketballPole::getY() {
    return polePos.y;
}

void BasketballPole::render() {
    poleTexture.render(polePos.x, polePos.y);
}

void BasketballPole::setTextureRealDimensions(int w, int h) {
    textureRealHeight = h;
    textureRealWidth = w;
}

int BasketballPole::getTextureRealHeight() {
    return textureRealHeight;
}

int BasketballPole::getTextureRealWidth() {
    return textureRealWidth;
}

void BasketballPole::free() {
    poleTexture.free();
    polePos.x = 0;
    polePos.y = 0;
    textureRealHeight = 0;
    textureRealWidth = 0;
}
