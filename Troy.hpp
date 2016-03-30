#ifndef TROY_HPP
#define TROY_HPP

#include "main.hpp"
#include "Player.hpp"

class BlackPlayer : public Player {
    int frameTime;
    int currFrameTime;
    Texture runningTextures[18];
public:
    BlackPlayer(std::string name);

    void free();

    void setRunningScenes();

    void update();

    bool isLastFrame();
};

#endif
