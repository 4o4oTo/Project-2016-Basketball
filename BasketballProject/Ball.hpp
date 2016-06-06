#ifndef BALL_HPP
#define BALL_HPP

#include "main.hpp"
#include "Texture.hpp"
#include "BasketballPole.hpp"

class Ball {
    const float bGravity;
    float bouncePower;
    float changedWidth;
    float changedHeight;
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
    bool wasChangingPerspective;
    bool hasGoneThroughTheNet;
    bool isStolen;
    float bVelX;
    float bVelY;
    float bMaxPower;
    float bInitialY;
    float rotationAngle;
    BasketballPole* pole;
    Mix_Chunk* rim;
    Mix_Chunk* swish;
    Mix_Chunk* backboard;
    Mix_Chunk* dribble;
public:

    Ball(float bounce);

    ~Ball();

    void changingPerspective(bool b);

    bool loadSoundEffects();

    Texture& getTexture();

    int getX();

    int getY();

    void passThePole(BasketballPole* p);

    void setIsStolen(bool s);

    void setPosition(int x, int y);

    void setHasCollidedWithThePole(bool b);

    void setHasScored(bool h);

    void processInput();

    void setChangedDimensions(float changedWidth = 0, float changedHeight = 0);

    void handleEvents(SDL_Event* e);

    void update();

    void resetDimensions();

    void checkCollision(int x = 0, int y = 0);

    void checkCollisionWithPole();

    void render();

    void setPossession(bool possession);

    void setIsThrown(bool t);

    void setVelocity(float xVel, float yVel);

    void setFirstRotation(bool f);

    bool isThrown();

    bool hasScored();

    bool hasBeenStolen();

    bool hasCollidedWithThePole();

    bool isPossessed();
};

#endif // BALL_HPP
