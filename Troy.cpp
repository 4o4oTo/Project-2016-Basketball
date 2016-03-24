#include "Troy.hpp"

BlackPlayer::BlackPlayer(std::string name) : Player::Player(name) {
    frameTime = 2;
    currFrameTime = 0;
}

bool BlackPlayer::isLastFrame() {
    return BlackPlayer::frame == 18;
}

void BlackPlayer::update() {

    processInput();

    if(isRunning) {

        if(currFrameTime == frameTime) {
            frame++;
            currFrameTime = 0;
        }
        else {
             currFrameTime++;
        }
        if(isLastFrame()) {
            frame = 0;
        }
        pTexture = runningTextures[frame];
    }
}

void BlackPlayer::setRunningScenes() {
    int counter;
    for(counter = 0; counter < 18; counter++) {
        BlackPlayer::runningTextures[counter].loadFromFile(std::string("player/NormalRunning/Run.") + std::to_string(counter+2).c_str() + ".png");
    }
}

void BlackPlayer::free() {
    for(int i = 0; i < 18; i++) {
        BlackPlayer::runningTextures[i].free();
    }
}
