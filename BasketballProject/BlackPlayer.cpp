#include "BlackPlayer.hpp"

BlackPlayer::BlackPlayer(std::string name) : Player::Player(name) {
    frameTime = 2;
    currFrameTime = 0;
    pJumpPower = -10;
    pMovementSpeed = 10;
}

void BlackPlayer::update() {

    processInput();

    if(isRunning && !isDefending && !hasTheBall) {

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
    else if(isRunning && isDefending && !hasTheBall) {
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
    else if(isDefending && isStanding && !hasTheBall) {
        pTexture = pDefenceStance;
        if(!positioned) {
            pPos.y += 20;
            positioned = true;
        }
    }
    else if(isJumping && !hasTheBall) {
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
    else if(hasTheBall && isStanding) {
        if(currFrameTime == frameTime) {
            frame++;
            currFrameTime = 0;
        }
        else {
            currFrameTime++;
        }
        if(isLastFrame(STANDDRIBBLE)) {
            frame = 0;
        }
        pTexture = standDribbleTextures[frame];
    }
    else if(hasTheBall && isRunning) {
//        if(currFrameTime == frameTime) {
//            frame++;
//            currFrameTime = 0;
//        }
//        else {
//            currFrameTime++;
//        }
        if(isLastFrame(DRIBBLING)) {
            frame = 0;
        }
        pTexture = dribblingTextures[frame];
        frame++;
    }
    else if(hasTheBall && isJumping) {
        if(currFrameTime == frameTime) {
            frame++;
            currFrameTime = 0;
        }
        else {
            currFrameTime++;
        }
        if(isLastFrame(SHOOT)) {
            frame = SHOOT-1;
        }
        pTexture = shootingTextures[frame];
    }
    else if(!hasTheBall && isJumping) {
        frame = 5;
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

void BlackPlayer::setDribblingScenes() {
    for(int i=0; i<DRIBBLING; i++) {
        if(!dribblingTextures[i].loadFromFile("player/Dribble/Dribble_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void BlackPlayer::setStandDribbleScenes() {
    for(int i=0; i<STANDDRIBBLE; i++) {
        if(!standDribbleTextures[i].loadFromFile("player/StandingDribble/Dribble_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void BlackPlayer::setShootingScenes() {
    for(int i=0; i<SHOOT; i++) {
        if(!shootingTextures[i].loadFromFile("player/Shoot/Shot_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void BlackPlayer::free() {
    for(int i = 0; i < RUNNING; i++) {
        BlackPlayer::runningTextures[i].free();
    }
}
