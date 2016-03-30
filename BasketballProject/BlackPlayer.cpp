#include "BlackPlayer.hpp"

BlackPlayer::BlackPlayer(std::string name) : Player::Player(name) {
    frameTime = 2;
    currFrameTime = 0;
    pJumpPower = -6.5;
    pMovementSpeed = 10;
}

void BlackPlayer::update() {

    processInput();

    if(isRunning && !isDefending) {

        if(currFrameTime == frameTime) {
            frame++;
            currFrameTime = 0;
        }
        else {
            currFrameTime++;
        }
        if(isLastFrame(RUNNING)) {
            frame = 0;
        }
        pTexture = runningTextures[frame];
    }
    else if(isRunning && isDefending) {
        printf("%d\n",frame);
        if(currFrameTime == frameTime) {
            frame++;
            currFrameTime = 0;
        }
        else {
             currFrameTime++;
        }
        if(isLastFrame(DEFENDING)) {
            frame = 0;
        }
        pTexture = defenceTextures[frame];
        if(!positioned) {
            pPos.y += 20;
            positioned = true;
        }
    }
    else if(isDefending && isStanding) {
        pTexture = pDefenceStance;
        if(!positioned) {
            pPos.y += 20;
            positioned = true;
        }
    }
    else if(isJumping) {
        if(currFrameTime == frameTime) {
            frame++;
            currFrameTime = 0;
        }
        else {
             currFrameTime++;
        }
        if(isLastFrame(JUMPING)) {
            frame=JUMPING-1;
        }
        pTexture = jumpingTextures[frame];
    }

}

void BlackPlayer::setRunningScenes() {
    for(int counter = 0; counter < RUNNING; counter++) {
        if(!BlackPlayer::runningTextures[counter].loadFromFile(std::string("player/NormalRunning/Run.") + std::to_string(counter+2).c_str() + ".png")) {
            printf("Error loading running images! %s\n", SDL_GetError());
        }
    }
}

void BlackPlayer::setDefenceScenes() {
    for(int i=0; i<DEFENDING; i++) {
        if(defenceTextures[i].loadFromFile("player/DefenceWalk/Walk_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void BlackPlayer::setJumpingScenes() {
    for(int i=0; i<JUMPING ; i++) {
        if(!jumpingTextures[i].loadFromFile("player/Jump/Jump_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void BlackPlayer::free() {
    for(int i = 0; i < RUNNING; i++) {
        BlackPlayer::runningTextures[i].free();
    }
}
