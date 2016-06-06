#include "BlackPlayer.hpp"

BlackPlayer::BlackPlayer(std::string name) : Player::Player(name, -10.0) {
    frameTime = 2;
    //currFrameTime = 0;
    pJumpPower = -10.0;
    pMovementSpeed = 10;
}

void BlackPlayer::setInitialScore() {
    pCurrentScore = scoreTextures[0];
    pScore = 0;
}

SDL_Rect& BlackPlayer::getScoreIndicator() {
    return scoreIndicatorDim;
}

void BlackPlayer::handleEvents(SDL_Event* e) {
    if(e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_d:
                if(facing == LEFT && hasLanded) {
                    frame = 0;
                    currFrameTime = 0;
                    movingRight = true;
                }
                isRunning = true;
                isStanding = false;
                break;

            case SDLK_a:
                if(facing == RIGHT && hasLanded) {
                    frame = 0;
                    currFrameTime = 0;
                    movingLeft = true;
                }
                isRunning = true;
                isStanding = false;
                break;

            case SDLK_LSHIFT:
                if(!isDefending && !isJumping && !hasTheBall) {
                    frame = 0;
                    currFrameTime = 0;
                    isDefending = true;
                    if(pMovementSpeed != 3) {
                        pMovementSpeed = 3;
                    }
                }
                break;
            case SDLK_SPACE:
                if(!isJumping) {
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

            case SDLK_w:
                if(!isJumping) {
                    isChangingPerspective = true;
                    ball->changingPerspective(true);
                    isGoingAway = true;
                }
                break;

            case SDLK_s:
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

            case SDLK_e:
                if(isDefending) {
                    isStealing = true;
                }
                break;
        }
    }
    else if(e->type == SDL_KEYUP && (e->key.keysym.sym == SDLK_a || e->key.keysym.sym == SDLK_d) && hasLanded) {
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
        if(e->key.keysym.sym == SDLK_a) {
            movingLeft = false;
        }
        else {
            movingRight = false;
        }
    }
    else if(e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_LSHIFT && hasLanded && !hasTheBall) {
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
    else if(e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_r && isJumping && hasTheBall && facing == RIGHT) {
        hasThrownTheBall = true;
        hasTheBall = false;
        ball->setIsThrown(true);
        lastPlayerToShoot = pName;
    }
    else if(e->type == SDL_KEYUP && (e->key.keysym.sym == SDLK_w || e->key.keysym.sym == SDLK_s) && hasLanded) {
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
        if(e->key.keysym.sym == SDLK_w) {
            isGoingAway = false;
        }
        else {
            isComingCloser = false;
        }
        isRunning = false;
        isStanding = true;
    }
}

void BlackPlayer::update() {

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

}

bool BlackPlayer::isBehindPlayer(Player *p) {
    if((pPos.y < p->getY() && !isJumping && !p->isJumpin())
       || (isJumping && pInitialY < p->getY() && !p->isJumpin())
       || (isJumping && pInitialY < p->getInitialY() && p->isJumpin())
       || (!isJumping && pPos.y < p->getInitialY() && p->isJumpin())) {
        return true;
    }
    else {
        return false;
    }
}

void BlackPlayer::setRunningScenes() {
    for(int counter = 0; counter < RUNNING; counter++) {
        if(!BlackPlayer::runningTextures[counter].loadFromFile(std::string("player/Troy/NormalRunning/Run.") + std::to_string(counter+2).c_str() + ".png")) {
            printf("Error loading running images! %s\n", SDL_GetError());
        }
    }
}

void BlackPlayer::setDefenceScenes() {
    for(int i=0; i<DEFENDING; i++) {
        if(defenceTextures[i].loadFromFile("player/Troy/DefenceWalk/Walk_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void BlackPlayer::setJumpingScenes() {
    for(int i=0; i<JUMPING ; i++) {
        if(!jumpingTextures[i].loadFromFile("player/Troy/Jump/Jump_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void BlackPlayer::setDribblingScenes() {
    for(int i=0; i<DRIBBLING; i++) {
        if(!dribblingTextures[i].loadFromFile("player/Troy/Dribble/Dribble_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void BlackPlayer::setStandDribbleScenes() {
    for(int i=0; i<STANDDRIBBLE; i++) {
        if(!standDribbleTextures[i].loadFromFile("player/Troy/StandingDribble/Dribble_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void BlackPlayer::setShootingScenes() {
    for(int i=0; i<SHOOT; i++) {
        if(!shootingTextures[i].loadFromFile("player/Troy/Shoot/Shot_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void BlackPlayer::setStealingScenes() {
    for(int i=0; i<STEALING; i++) {
        if(stealingTextures[i].loadFromFile("player/Troy/Stealing/Steal_" + std::to_string(i+1) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
}

void BlackPlayer::setScoreIndicator() {
    for(int i=0; i<SCORE; i++) {
        if(!scoreTextures[i].loadFromFile("background/score/Troy/" + std::to_string(i) + ".png")) {
            printf("%s\n", SDL_GetError());
        }
    }
    scoreIndicatorDim.w = scoreTextures[0].getWidth();
    scoreIndicatorDim.h = scoreTextures[0].getHeight();
}

void BlackPlayer::free() {
    for(int i = 0; i < RUNNING; i++) {
        BlackPlayer::runningTextures[i].free();
    }
    for(int i = 0; i < DRIBBLING; i++) {
        BlackPlayer::dribblingTextures[i].free();
    }
    for(int i = 0; i < STANDDRIBBLE; i++) {
        BlackPlayer::standDribbleTextures[i].free();
    }
    for(int i = 0; i < JUMPING; i++) {
        BlackPlayer::jumpingTextures[i].free();
    }
    for(int i = 0; i < DEFENDING; i++) {
        BlackPlayer::defenceTextures[i].free();
    }
    for(int i = 0; i < SHOOT; i++) {
        BlackPlayer::shootingTextures[i].free();
    }
    for(int i = 0; i < SCORE; i++) {
        BlackPlayer::scoreTextures[i].free();
    }
}
