#ifndef BALL_HPP
#define BALL_HPP

#include "main.hpp"
#include "Texture.hpp"
#include "BasketballPole.hpp"

class Ball {
    const float bGravity;
    float bouncePower;
    SDL_Point bBall;
    Texture bTexture;
    bool possessed;
    bool thrown;
    bool collidedWithPole;
    bool hitBoard;
    bool sameLvlAsRim;
    bool aboveRim;
    bool belowRim;
    bool behindRim;
    bool beforeRim;
    bool firstRotation;
    float bVelX;
    float bVelY;
    float bMaxPower;
    float bInitialY;
    float rotationAngle;
    BasketballPole* pole;
public:

    Ball(float bounce);

    ~Ball();

    Texture& getTexture();

    int getX();

    int getY();

    void passThePole(BasketballPole* p);

    void setPosition(int x, int y);

    void setHasCollidedWithThePole(bool b);

    void processInput();

    void handleEvents(SDL_Event* e);

    void update();

    void checkCollision(int x = 0, int y = 0);

    void checkCollisionWithPole();

    void render();

    void setPossession(bool possession);

    void setIsThrown(bool t);

    void setVelocity(float xVel, float yVel);

    bool isThrown();

    bool hasCollidedWithThePole();

    bool isPossessed();
};

#endif // BALL_HPP
