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
    bool isDefending;
    bool isRunning;
    bool isStanding;
    int frame;
    std::string pName;
    Texture pTexture;
    SDL_Point pPos;
    SDL_RendererFlip pFlip;
    FacingDirection facing;
    Texture pNormalStance;
    float pVelX;
    float pVelY;
public:
    Player(std::string name);

    ~Player();

    void handleEvents(SDL_Event* e);

    virtual void setRunningScenes() = 0;

    virtual void update() = 0;

    void setInitialPosition(int x, int y);

    void processInput();

    void checkCollision();

    void setFacingDirection(FacingDirection dir);

    bool loadTextureFromFile(std::string path);

    bool setNormalStance(std::string path);

    int getX();

    int getY();

    int getWidth();

    int getHeight();

    std::string getName();

    void render();
};
#endif
