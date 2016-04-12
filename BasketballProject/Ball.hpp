#ifndef BALL_HPP
#define BALL_HPP

#include "main.hpp"
#include "Texture.hpp"

class Ball {
    const float bGravity;
    float bouncePower;
    SDL_Point bBall;
    Texture bTexture;
    bool possessed;
    bool thrown;
    float bVelX;
    float bVelY;
    float bMaxPower;
    float bInitialY;
public:

    Ball(float bounce);

    ~Ball();

    Texture& getTexture();

    int getX();

    int getY();

    void setPosition(int x, int y);

    void processInput();

    void handleEvents(SDL_Event* e);

    void update();

    void checkCollision(int x = 0, int y = 0);

    void render();

    void setPossession(bool possession);

    void setIsThrown(bool t);

    bool isThrown();

    bool isPossessed();
};

#endif // BALL_HPP
