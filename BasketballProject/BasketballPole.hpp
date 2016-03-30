#ifndef BASKETBALL_POLE_HPP
#define BASKETBALL_POLE_HPP
#include "main.hpp"
#include "Texture.hpp"

class BasketballPole {
    SDL_Rect polePos;
    Texture poleTexture;
    int textureRealHeight, textureRealWidth;
public:
    BasketballPole();

    void setPosition(int x, int y);

    void setTextureRealDimensions(int w, int h);

    int getTextureRealHeight();

    int getTextureRealWidth();

    int getX();

    int getY();

    Texture& getTexture();

    void render();

    void free();
};

#endif // BASKETBALL_POLE_HPP
