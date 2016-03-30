#ifndef BLACK_PLAYER_HPP
#define BLACK_PLAYER_HPP

#include "main.hpp"
#include "Player.hpp"

#define RUNNING 18
#define DEFENDING 13
#define JUMPING 6

class BlackPlayer : public Player {
    int frameTime;
    int currFrameTime;
    Texture runningTextures[RUNNING];
    Texture defenceTextures[DEFENDING];
    Texture jumpingTextures[JUMPING];
public:
    BlackPlayer(std::string name);

    void free();

    void setRunningScenes();

    void setDefenceScenes();

    void setJumpingScenes();

    void update();
};

#endif
