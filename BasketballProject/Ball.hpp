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
    float bVelX;
    float bVelY;
public:

    Ball(float power);

    ~Ball();

    Texture& getTexture();

    int getX();

    int getY();

    void setPosition(int x, int y);

    void update();

    void checkCollision(int x = 0, int y = 0);

    void render();

    void setPossession(bool possession);

    bool isPossessed();
};

#endif // BALL_HPP
