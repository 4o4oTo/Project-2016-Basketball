#ifndef WHITE_PLAYER_HPP
#define WHITE_PLAYER_HPP

#include "Player.hpp"

#define RUNNING 19
#define DEFENDING 10
#define JUMPING 7
#define DRIBBLING 19
#define STANDDRIBBLE 22
#define SHOOT 4
#define SCORE 22

class WhitePlayer : public Player {
    int frameTime;
    //int currFrameTime;
    Texture runningTextures[RUNNING];
    Texture defenceTextures[DEFENDING];
    Texture jumpingTextures[JUMPING];
    Texture dribblingTextures[DRIBBLING];
    Texture standDribbleTextures[STANDDRIBBLE];
    Texture shootingTextures[SHOOT];
    Texture scoreTextures[SCORE];
    bool stancePositioned;
public:
    WhitePlayer(std::string name);

    void free();

    void setRunningScenes();

    void setDefenceScenes();

    void setJumpingScenes();

    void setDribblingScenes();

    void setShootingScenes();

    void setStandDribbleScenes();

    void setScoreIndicator();

    void setInitialScore();

    void update();

    void handleEvents(SDL_Event* e);

    SDL_Rect& getScoreIndicator();
};

#endif // WHITE_PLAYER_HPP
