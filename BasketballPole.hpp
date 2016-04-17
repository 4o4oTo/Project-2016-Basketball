#ifndef BASKETBALL_POLE_HPP
#define BASKETBALL_POLE_HPP
#include "main.hpp"
#include "Texture.hpp"

class BasketballPole {
    SDL_Rect polePos;
    Texture poleTexture;
    SDL_Rect board;
    SDL_Rect belowBoard;
    SDL_Rect rim;
public:
    BasketballPole();

    void setPosition(int x, int y);

    void setDimensionsBelowBoard(int x, int y, int w, int h);

    void setBoardDimensions(int x, int y, int w, int h);

    void setRimDimensions(int x, int y, int w, int h);

    SDL_Rect& getBoard();

    SDL_Rect& getRim();

    SDL_Rect& getBelowBoard();

    int getX();

    int getY();

    Texture& getTexture();

    void render();

    void free();
};

#endif // BASKETBALL_POLE_HPP
