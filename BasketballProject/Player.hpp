#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "main.hpp"
#include "Texture.hpp"
#include "BasketballPole.hpp"
#include "Ball.hpp"

enum FacingDirection {
    RIGHT = 0,
    LEFT = 1
};

class Player {
protected:
    static const float pGravity;
    static std::string lastPlayerToShoot;
    int pInitialY;
    int pShotPosition;
    float pJumpPower;
    float pMovementSpeed;
    bool isDefending;
    bool isStealing;
    bool isRunning;
    bool isChangingPerspective;
    bool hasLanded;
    bool isStanding;
    bool isJumping;
    bool hasStolenTheBall;
    bool hasTheBall;
    bool hadTheBallBeforeJump;
    bool hasThrownTheBall;
    bool hasScored;
    bool isComingCloser;
    bool isGoingAway;
    bool movingLeft;
    bool movingRight;
    bool isTheWinner;
    int frame;
    int currFrameTime;
    bool positioned;
    std::string pName;
    Texture pTexture;
    SDL_Point pPos;
    SDL_RendererFlip pFlip;
    FacingDirection facing;
    Texture pNormalStance;
    Texture pDefenceStance;
    Texture pCurrentScore;
    float pVelX;
    float pVelY;
    int textureRealHeight, textureRealWidth;
    int changedHeight, changedWidth;
    bool behindRim;
    bool belowRim;
    Ball* ball;
    Mix_Chunk* dribble;
    int pScore;
    SDL_Rect scoreIndicatorDim;
public:
    Player(std::string name, float jumpPower);

    ~Player();

    virtual void handleEvents(SDL_Event* e) = 0;

    virtual void setRunningScenes() = 0;

    virtual void setDefenceScenes() = 0;

    virtual void setJumpingScenes() = 0;

    virtual void setDribblingScenes() = 0;

    virtual void setShootingScenes() = 0;

    virtual void setStandDribbleScenes() = 0;

    virtual void setScoreIndicator() = 0;

    virtual void setInitialScore() = 0;

    virtual void update() = 0;

    void checkPlayerCollision(Player *p);

    void setInitialPosition(int x, int y);

    void setHasStolenTheBall(bool s);

    bool loadSoundEffects();

    FacingDirection isFacing();

    void passTheBall(Ball* b);

    void setBehindRim(bool behind);

    void setBelowRim(bool below);

    void setBallPossession(bool possession);

    void setScoreIndicatorPosition(int x, int y);

    void processInput();

    bool isDef();

    bool isBelowRim();

    bool isBehindRim();

    bool hasBall();

    bool isJumpin();

    bool isStealingBall();

    bool hasWon();

    void setVictory(bool v);

    void setIsStealingBall(bool i);

    bool isLastFrame(int frames_count);

    void checkCollision(int x = 0, int y = 0);

    void setFacingDirection(FacingDirection dir);

    bool loadTextureFromFile(std::string path);

    void setNormalStance();

    bool setNormalStance(std::string path);

    bool setDefenceStance(std::string path);

    void setTextureRealDimensions(int w, int h);

    int getTextureRealHeight();

    int getTextureRealWidth();

    void checkBasketballPoleCollision(BasketballPole* pole);

    void checkBallCollision();

    int getX();

    int getY();

    int getWidth();

    int getHeight();

    int getInitialY();

    Texture& getTexture();

    std::string getName();

    void render();

    void renderScoreIndicator();
};
#endif
