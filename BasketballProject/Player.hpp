#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "main.hpp"
#include "Texture.hpp"

enum FacingDirection {
    RIGHT = 0,
    LEFT = 1
};

class Player {
protected:
    static const float pGravity;
    int pInitialY;
    float pJumpPower;
    bool isDefending;
    bool isRunning;
    bool hasLanded;
    bool isStanding;
    bool isJumping;
    int pMaxJumpHeight;
    int frame;
    bool positioned;
    std::string pName;
    Texture pTexture;
    SDL_Point pPos;
    SDL_RendererFlip pFlip;
    FacingDirection facing;
    Texture pNormalStance;
    Texture pDefenceStance;
    float pVelX;
    float pVelY;
    int textureRealHeight, textureRealWidth;
    float pMovementSpeed;
public:
    Player(std::string name);

    ~Player();

    void handleEvents(SDL_Event* e);

    virtual void setRunningScenes() = 0;

    virtual void update() = 0;

    void setInitialPosition(int x, int y);

    void processInput();

    bool isLastFrame(int frames_count);

    void checkCollision(int collider = 0);

    void setFacingDirection(FacingDirection dir);

    bool loadTextureFromFile(std::string path);

    bool setNormalStance(std::string path);

    bool setDefenceStance(std::string path);

    void setTextureRealDimensions(int w, int h);

    int getTextureRealHeight();

    int getTextureRealWidth();

    int getX();

    int getY();

    int getWidth();

    int getHeight();

    Texture& getTexture();

    std::string getName();

    void render();
};
#endif
