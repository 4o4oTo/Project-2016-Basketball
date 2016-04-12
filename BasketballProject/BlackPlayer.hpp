#ifndef BLACK_PLAYER_HPP
#define BLACK_PLAYER_HPP

#include "main.hpp"
#include "Player.hpp"

#define RUNNING 18
#define DEFENDING 13
#define JUMPING 6
#define DRIBBLING 21
#define STANDDRIBBLE 22
#define SHOOT 5

class BlackPlayer : public Player {
    int frameTime;
    int currFrameTime;
    Texture runningTextures[RUNNING];
    Texture defenceTextures[DEFENDING];
    Texture jumpingTextures[JUMPING];
    Texture dribblingTextures[DRIBBLING];
    Texture standDribbleTextures[STANDDRIBBLE];
    Texture shootingTextures[SHOOT];
public:
    BlackPlayer(std::string name);

    void free();

    void setRunningScenes();

    void setDefenceScenes();

    void setJumpingScenes();

    void setDribblingScenes();

    void setShootingScenes();

    void setStandDribbleScenes();

    void update();
};

#endif
