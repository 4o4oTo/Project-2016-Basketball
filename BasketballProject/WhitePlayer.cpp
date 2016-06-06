#include "WhitePlayer.hpp"

WhitePlayer::WhitePlayer(std::string name) : Player::Player(name, -10.0) {
    frameTime = 2;
    //currFrameTime = 0;
    pJumpPower = -10.0;
    pMovementSpeed = 10;
//    stancePositioned = false;
}

void WhitePlayer::setInitialScore() {
    pCurrentScore = scoreTextures[0];
    pScore = 0;
}

SDL_Rect& WhitePlayer::getScoreIndicator() {
    return scoreIndicatorDim;
}

void WhitePlayer::handleEvents(SDL_Event* e) {
    if(e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_RIGHT:
                if(facing == LEFT && hasLanded) {
                    frame = 0;
                    currFrameTime = 0;
                    movingRight = true;
                }
                isStanding = false;
                isRunning = true;
//                if(stancePositioned) {
//                    stancePositioned = false;
//                    pPos.y += 10;
//                }
                break;

            case SDLK_LEFT:
                if(facing == RIGHT && hasLanded) {
                    frame = 0;
                    currFrameTime = 0;
                    movingLeft = true;
                }
                isStanding = false;
                isRunning = true;
//                if(stancePositioned) {
//                    stancePositioned = false;
//                    pPos.y += 10;
//                }
                break;

            case SDLK_KP_0:
                if(!isDefending && !isJumping && !hasTheBall) {
                    frame = 0;
                    currFrameTime = 0;
                    isDefending = true;
                    if(pMovementSpeed != 3) {
                        pMovementSpeed = 3;
                    }
//                    if(stancePositioned) {
//                        stancePositioned = false;
//                        pPos.y += 10;
//                    }
                }
                break;
            case SDLK_RSHIFT:
                if(!isJumping) {
//                    if(stancePositioned) {
//                        stancePositioned = false;
//                        pPos.y += 10;
//                    }
                    if(hasTheBall) {
                        pShotPosition = pPos.x + getTextureRealWidth()*2;
                        ball->setVelocity(0, 0);
                        hadTheBallBeforeJump = true;
                    }
                    else {
                        hadTheBallBeforeJump = false;
                    }
                    frame = 0;
                    currFrameTime = 0;
                    pInitialY = pPos.y;
                    isJumping = true;
                    if(!isChangingPerspective) {
                        pVelY = pJumpPower;
                    }
                    else {
                        pVelY = 8*pJumpPower/9;
                    }
                    pMovementSpeed = 3;
                    hasLanded = false;
                    isDefending = false;
                    isRunning = false;
                    isStanding = false;
                }
                break;

            case SDLK_UP:
                if(!isJumping) {
//                    if(stancePositioned) {
//                        stancePositioned = false;
//                        pPos.y += 10;
//                    }
                    isChangingPerspective = true;
                    ball->changingPerspective(true);
                    isGoingAway = true;
                }
                break;

            case SDLK_DOWN:
//                if(stancePositioned) {
//                    stancePositioned = false;
//                    pPos.y += 10;
//                }
                if(!isJumping) {
                    if(pPos.y < 478) {
                        isChangingPerspective = true;
                        if(hasTheBall) {
                            ball->changingPerspective(true);
                        }
                        isComingCloser = true;
                    }
                }
                break;

            case SDLK_RCTRL:
                if(isDefending) {
                    isStealing = true;
                }
                break;
        }
    }
    else if(e->type == SDL_KEYUP && (e->key.keysym.sym == SDLK_LEFT || e->key.keysym.sym == SDLK_RIGHT) && hasLanded) {
        pVelX = 0.0;
        frame = 0;
        currFrameTime = 0;
        if(!hasTheBall) {
            pTexture = pNormalStance;
        }
        isRunning = false;
        isStanding = true;
        if(positioned) {
            pPos.y -= 20;
            positioned = false;
        }
        if(e->key.keysym.sym == SDLK_LEFT) {
            movingLeft = false;
        }
        else {
            movingRight = false;
        }
    }
    else if(e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_KP_0 && hasLanded && !hasTheBall) {
        isDefending = false;
        frame = 0;
        currFrameTime = 0;
        pMovementSpeed = 10;
        if(positioned) {
            pPos.y -= 20;
            positioned = false;
        }
        if(!isRunning) {
            pTexture = pNormalStance;
        }
    }
    else if(e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_RETURN && isJumping && hasTheBall && facing == RIGHT) {
        hasThrownTheBall = true;
        hasTheBall = false;
        ball->setIsThrown(true);
        lastPlayerToShoot = pName;
    }
    else if(e->type == SDL_KEYUP && (e->key.keysym.sym == SDLK_UP || e->key.keysym.sym == SDLK_DOWN) && hasLanded) {
        pVelY = 0.0;
        frame = 0;
//        currFrameTime = 0;
        if((pPos.y == 478 && !isDefending)) {
            isChangingPerspective = false;
            if(hasTheBall) {
                ball->changingPerspective(false);
            }
        }
        else if(pPos.y == 498 && isDefending) {
            isChangingPerspective = false;
        }
        if(!hasTheBall) {
            pTexture = pNormalStance;
        }
        if(e->key.keysym.sym == SDLK_UP) {
            isGoingAway = false;
        }
        else {
            isComingCloser = false;
        }
        isRunning = false;
        isStanding = true;
    }
}

void WhitePlayer::update() {

    Player::processInput();

    if(lastPlayerToShoot.c_str() == pName && ball->hasScored() && !hasScored) {
        if((pShotPosition <= 553 && !isChangingPerspective)
                || (isChangingPerspective && pInitialY == 403)
                || (isChangingPerspective && pShotPosition <= 553)){
            if(pScore + 2 >= 21) {
                pScore = 21;
                if(!isTheWinner) {
                    isTheWinner = true;
                }
            }
            else {
                pScore += 2;
            }
        }
        else {
            if(pScore + 1 >= 21) {
                pScore = 21;
                if(!isTheWinner) {
                    isTheWinner = true;
                }
            }
            else {
                pScore++;
            }
        }
        pCurrentScore = scoreTextures[pScore];
        hasScored = true;
    }

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
    else if(isDefending && isStanding && !hasTheBall && !isStealing) {
        pTexture = pDefenceStance;
        if(!positioned) {
            pPos.y += 20;
            positioned = true;
        }
    }
    else if(isDefending && isStanding && !hasTheBall && isStealing) {
        if(currFrameTime == frameTime) {
            frame++;
            currFrameTime = 0;
        }
        else {
             currFrameTime++;
        }
        if(isLastFrame(STEALING)) {
            frame = 0;
            isStealing = false;
        }
        pTexture = stealingTextures[frame];
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
            if(frame == 8) {
                Mix_PlayChannel(-1, dribble, 0);
            }
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
        if(frame == 10) {
            Mix_PlayChannel(-1, dribble, 0);
        }
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
    else if(isStanding && !hasTheBall) {
//        if(!stancePositioned) {
//            stancePositioned = true;
//            pPos.y -= 10;
//        }
    }
}

void WhitePlayer::setRunningScenes() {
    for(int counter = 0; counter < RUNNING; counter++) {
        if(!WhitePlayer::runningTextures[counter].loadFromFile(std::string("player/Zac/NormalRunning/Run_") + std::to_string(counter+1).c_str() + ".png")) {
            printf("Error loading running images! %s\n", SDL_GetError());
        }
    }
}

void WhitePlayer::setDefenceScenes() {
    for(int i=0; i<DEFENDING; i++) {
        if(defenceTextures[i].loadFromFile("player/Zac/DefenceWalk/Walk_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void WhitePlayer::setJumpingScenes() {
    for(int i=0; i<JUMPING ; i++) {
        if(!jumpingTextures[i].loadFromFile("player/Zac/Jump/Jump_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void WhitePlayer::setDribblingScenes() {
    for(int i=0; i<DRIBBLING; i++) {
        if(!dribblingTextures[i].loadFromFile("player/Zac/Dribble/Dribble_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void WhitePlayer::setStandDribbleScenes() {
    for(int i=0; i<STANDDRIBBLE; i++) {
        if(!standDribbleTextures[i].loadFromFile("player/Zac/StandingDribble/Dribble_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void WhitePlayer::setShootingScenes() {
    for(int i=0; i<SHOOT; i++) {
        if(!shootingTextures[i].loadFromFile("player/Zac/Shoot/Shot_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void WhitePlayer::setStealingScenes() {
    for(int i=0; i<STEALING; i++) {
        if(stealingTextures[i].loadFromFile("player/Zac/Stealing/Steal_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void WhitePlayer::setScoreIndicator() {
    for(int i=0; i<SCORE; i++) {
        if(!scoreTextures[i].loadFromFile("background/score/Zac/" + std::to_string(i) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
    scoreIndicatorDim.w = scoreTextures[0].getWidth();
    scoreIndicatorDim.h = scoreTextures[0].getHeight();
}

void WhitePlayer::free() {
    for(int i = 0; i < RUNNING; i++) {
        WhitePlayer::runningTextures[i].free();
    }
    for(int i = 0; i < DRIBBLING; i++) {
        WhitePlayer::dribblingTextures[i].free();
    }
    for(int i = 0; i < DEFENDING; i++) {
        WhitePlayer::defenceTextures[i].free();
    }
    for(int i = 0; i < STANDDRIBBLE; i++) {
        WhitePlayer::standDribbleTextures[i].free();
    }
    for(int i = 0; i < SHOOT; i++) {
        WhitePlayer::shootingTextures[i].free();
    }
    for(int i = 0; i < JUMPING; i++) {
        WhitePlayer::jumpingTextures[i].free();
    }
    for(int i = 0; i < SCORE; i++) {
        WhitePlayer::scoreTextures[i].free();
    }
}
