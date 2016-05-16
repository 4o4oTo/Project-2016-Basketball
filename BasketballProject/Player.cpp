#include "Player.hpp"

const float Player::pGravity = 0.1;

Player::Player(std::string name, float jumpPower) {
    Player::pName = name;
    currFrameTime = 0;
    Player::pFlip = SDL_FLIP_NONE;
    Player::pVelX = 0.0;
    Player::pVelY = 0.0;
    Player::pJumpPower = jumpPower;
    Player::pPos.x = 0;
    Player::pPos.y = 0;
    Player::changedWidth = 0;
    Player::changedHeight = 0;
    Player::frame = 0;
    Player::positioned = false;
    Player::isRunning = false;
    Player::isDefending = false;
    Player::isStanding = true;
    Player::isJumping = false;
    Player::hasLanded = true;
    Player::textureRealHeight = 0;
    Player::textureRealWidth = 0;
    Player::behindRim = false;
    Player::belowRim = false;
    Player::hasTheBall = false;
    Player::hadTheBallBeforeJump = false;
    Player::hasThrownTheBall = false;
    Player::hasScored = false;
    Player::isChangingPerspective = false;
    Player::dribble = NULL;
    Player::isComingCloser = false;
    Player::isGoingAway = false;
    Player::movingLeft = false;
    Player::movingRight = false;
    Player::isTheWinner = false;
    Player::pScore = 0;
    Player::pShotPosition = 0;
}

Player::~Player() {
    Player::pTexture.free();
    Player::textureRealHeight = 0;
    Player::textureRealWidth = 0;
    Player::pVelX = 0.0;
    Player::pVelY = 0.0;
    Player::pPos.x = 0;
    Player::pPos.y = 0;
    Player::frame = 0;
    Player::pDefenceStance.free();
    Player::pNormalStance.free();
    Player::pCurrentScore.free();
    Mix_FreeChunk(dribble);
    dribble = NULL;
}

bool Player::loadSoundEffects() {
    bool success = true;
    dribble = Mix_LoadWAV("music/Dribble.wav");
    if(dribble == NULL) {
        printf("%s\n", Mix_GetError());
        success = false;
    }
    return success;
}

Texture& Player::getTexture() {
    return pTexture;
}

void Player::passTheBall(Ball* b) {
    ball = b;
}

bool Player::isLastFrame(int frames_count) {
    return frame == frames_count;
}

bool Player::hasWon() {
    return isTheWinner;
}

bool Player::isBehindRim() {
    return behindRim;
}

bool Player::isJumpin() {
    return isJumping;
}

bool Player::isBelowRim() {
    return belowRim;
}

bool Player::hasBall() {
    return hasTheBall;
}

void Player::setBehindRim(bool behind) {
    behindRim = behind;
}

void Player::setBallPossession(bool possession) {
    hasTheBall = possession;
}

void Player::setBelowRim(bool below) {
    belowRim = below;
}

void Player::setInitialPosition(int x, int y) {
    Player::pPos.x = x;
    Player::pPos.y = y;
    Player::changedHeight = pTexture.getHeight();
    Player::changedWidth = pTexture.getWidth();
}

void Player::setFacingDirection(FacingDirection dir) {
    Player::facing = dir;
}

int Player::getX() {
    return Player::pPos.x;
}

int Player::getY() {
    return Player::pPos.y;
}

int Player::getWidth() {
    return Player::pTexture.getWidth();
}

int Player::getHeight() {
    return Player::pTexture.getHeight();
}

std::string Player::getName() {
    return Player::pName;
}

bool Player::loadTextureFromFile(std::string path) {
    return Player::pTexture.loadFromFile(path);
}

void Player::setVictory(bool v) {
    isTheWinner = v;
}

void Player::render() {
    Player::pTexture.render(Player::pPos.x, Player::pPos.y, NULL, Player::pFlip, 0.0, NULL, Player::changedWidth, Player::changedHeight);
}

void Player::renderScoreIndicator() {
    pCurrentScore.render(scoreIndicatorDim.x, scoreIndicatorDim.y);
}

void Player::setScoreIndicatorPosition(int x, int y) {
    scoreIndicatorDim.x = x;
    scoreIndicatorDim.y = y;
}

void Player::setTextureRealDimensions(int w, int h) {
    textureRealHeight = h;
    textureRealWidth = w;
}

int Player::getTextureRealHeight() {
    return textureRealHeight;
}

int Player::getTextureRealWidth() {
    return textureRealWidth;
}

void Player::processInput() {
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);
    if((keyState[SDL_SCANCODE_A] && pName == "Troy") || (keyState[SDL_SCANCODE_LEFT] && pName == "Zac")) {
        if(facing == RIGHT && hasLanded) {
            facing = LEFT;
            pFlip = SDL_FLIP_HORIZONTAL;
        }
        if(!isJumping) {
            isRunning = true;
            isStanding = false;
            if(!isDefending) {
                pMovementSpeed = 10;
            }
            if( (!isDefending && isComingCloser && pPos.y < 478) || (isDefending && isComingCloser && pPos.y < 498)) {
                changedHeight += 1;
                changedWidth += 1;
                printf("wtf\n");
                pVelY = 1;
                if(hasTheBall) {
                    ball->setChangedDimensions(0.2, 0.2);
                }
            }
            else if((isGoingAway && pPos.y > 403 && !isDefending) || (isDefending && isGoingAway && pPos.y > 423)) {
                changedHeight -= 0.5;
                changedWidth -= 0.5;
                pVelY = -0.5;
                if(hasTheBall) {
                    ball->setChangedDimensions(-0.2, -0.2);
                }
            }
            movingLeft = true;
        }
        pVelX = -pMovementSpeed;
    }
    else if((keyState[SDL_SCANCODE_D] && pName == "Troy") || (keyState[SDL_SCANCODE_RIGHT] && pName == "Zac")) {
        if(facing == LEFT) {
            facing = RIGHT;
            pFlip = SDL_FLIP_NONE;
        }
        if(!isJumping) {
            movingRight = true;
            isRunning = true;
            isStanding = false;
            if(!isDefending) {
                pMovementSpeed = 10;
            }
            if((!isDefending && isComingCloser && pPos.y < 478) || (isDefending && isComingCloser && pPos.y < 498)) {
                changedHeight += 1;
                changedWidth += 1;
                pVelY = 1;
                if(hasTheBall) {
                    ball->setChangedDimensions(0.2, 0.2);
                }
            }
            else if((isGoingAway && pPos.y > 403 && !isDefending) || (isDefending && pPos.y > 423 && isGoingAway)) {
                changedHeight -= 1;
                changedWidth -= 1;
                pVelY = -1;
                if(hasTheBall) {
                    ball->setChangedDimensions(-0.2, -0.2);
                }
            }
        }
        pVelX = pMovementSpeed;
    }
    else if((keyState[SDL_SCANCODE_LSHIFT] && pName == "Troy") || (keyState[SDL_SCANCODE_KP_0] && pName == "Zac")) {
        if(!isDefending && hasLanded && !hasTheBall) {
            isDefending = true;
            if(pMovementSpeed != 3) {
                pMovementSpeed = 3;
            }
        }

    }
    if((keyState[SDL_SCANCODE_W] && pName == "Troy") || (keyState[SDL_SCANCODE_UP] && pName == "Zac")) {
        if(!isJumping && !movingLeft && !movingRight) {
            isRunning = true;
            isStanding = false;
            if(!isDefending) {
                pMovementSpeed = 10;
            }
            if((pPos.y > 403 && !isDefending) || (pPos.y > 403 + 20 && isDefending)) {

                if(isDefending) {
                    changedHeight -= 1;
                    changedWidth -= 1;
                    pVelY = -1;
                }
                else {
                    changedHeight -= 2;
                    changedWidth -= 2;
                    pVelY = -2;
                }
                if(hasTheBall) {
                    ball->setChangedDimensions(-0.2, -0.2);
                }
            }
        }
    }
    else if((keyState[SDL_SCANCODE_S] && pName == "Troy") || (keyState[SDL_SCANCODE_DOWN] && pName == "Zac")) {
        if(!isJumping && !movingLeft && !movingRight) {
            isRunning = true;
            isStanding = false;
            if(!isDefending) {
                pMovementSpeed = 10;
            }
            if((pPos.y < 478 && !isDefending) || (pPos.y < 478 + 20 && isDefending)) {
               if(isDefending) {
                    changedHeight += 1;
                    changedWidth += 1;
                    pVelY = 1;
                }
                else {
                    changedHeight += 2;
                    changedWidth += 2;
                    pVelY = 2;
                }
                if(hasTheBall) {
                    ball->setChangedDimensions(0.2, 0.2);
                }
            }
        }
    }
    if(isJumping) {
        isRunning = false;
        isDefending = false;
        if(pPos.y > pInitialY) {
            isJumping = false;
            frame = 0;
            currFrameTime = 0;
            pMovementSpeed = 10;
            pVelY = 0.0;
            pVelX = 0.0;
            pPos.y = pInitialY;
            isStanding = true;
            hasLanded = true;
            if(hadTheBallBeforeJump && !ball->hasCollidedWithThePole() && hasTheBall) {
                ball->resetDimensions();
                ball->setPosition(100, SCREEN_HEIGHT - ball->getTexture().getHeight());
                ball->setVelocity(0, 0);
                ball->setPossession(false);
                hadTheBallBeforeJump = false;
                hasTheBall = false;
            }
            else {
                ball->setHasCollidedWithThePole(false);
            }
            if(!hasTheBall && !isDefending) {
                pTexture = pNormalStance;
            }
            else if(!hasTheBall && isDefending) {
                pTexture = pDefenceStance;
            }
        }
        else {
            if(pVelY >= 0) {
                pVelY += 3*pGravity;
            }
            else {
                pVelY += 2*pGravity;
            }
        }
    }
    pPos.x += pVelX;
    pPos.y += pVelY;
}

void Player::checkCollision(int x, int y) {
//    printf("%d HAS THE BALL\n", hasTheBall);
//    printf("%d HAD THE BALL\n", hadTheBallBeforeJump);
//    printf("%d has COLLIDED\n", ball->hasCollidedWithThePole());

    if(isChangingPerspective && pPos.y <= 403 && !isJumping && !isDefending) {
        pPos.y = 403;
    }
    else if(isChangingPerspective && pPos.y >= 478 && !isJumping && !isDefending) {
        pPos.y = 478;
    }
    else if(isChangingPerspective && pPos.y <= 423 && !isJumping && isDefending) {
        pPos.y = 423;
    }
    else if(isChangingPerspective && pPos.y >= 498 && !isJumping && isDefending) {
        pPos.y = 498;
    }

    if(pPos.x < -(getTexture().getWidth() - getTextureRealWidth())/2) {
        pPos.x = -(getTexture().getWidth() - getTextureRealWidth())/2;
    }
    else if(x != 0 && y == 0) {
        if(pPos.x + getTextureRealWidth()*2 > x && x > 0) {
            pPos.x = x - getTextureRealWidth()*2;
            pVelX = 0;
        }
    }
    else if(y != 0 && x == 0) {
        if(pPos.y <= y) {
            pPos.y = y + 1;
            pVelY = 0;
            pVelX = 0;
        }
    }
    else if(y != 0 && x != 0) {
        if(pPos.x + getTextureRealWidth()*2 > x && x > 0) {
            pPos.x = x - getTextureRealWidth()*2;
            pVelX = 0;
        }
        else if(x < 0) {
            x = -x;
            if(pPos.x + getTextureRealWidth() <= x) {
                pPos.x = x - getTextureRealWidth();
                pVelX = 0;
            }
        }
        if(pPos.y <= y) {
            pPos.y = y + 1;
            pVelY = 0;
            pVelX = 0;
        }
    }
}

void Player::checkBasketballPoleCollision(BasketballPole* pole) {
    if(!isChangingPerspective) {
        if(getX() + getTextureRealWidth() - 10 > pole->getRim().x + pole->getRim().w - 20) {
            behindRim = true;
        }
        else {
            behindRim = false;
        }
        if(getX() + getTextureRealWidth()*2 - 10 >= pole->getRim().x && getX() + getTextureRealWidth() - 10 <= pole->getRim().x + pole->getRim().w - 20) {
           belowRim = true;
        }
        else {
            belowRim = false;
        }
        if(getY() <= pole->getRim().y && !behindRim && !belowRim) {
            checkCollision(pole->getRim().x);
        }
        else if(behindRim) {
            if(pPos.x + getTextureRealWidth() - 30 <= pole->getRim().x + pole->getRim().w - 20 && pPos.y <= pole->getRim().y + pole->getRim().h + 30) {
                checkCollision(-pole->getBoard().x, pole->getBelowBoard().y);
            }
            else {
                checkCollision(pole->getBelowBoard().x - 70, pole->getBelowBoard().y);
            }

        }
        else if(belowRim) {
            if(getY() <= pole->getRim().y + pole->getRim().h + 30 && getX() + getTextureRealWidth()*2 - 10 <= pole->getBoard().x) {
                checkCollision(pole->getBoard().x, pole->getRim().y + pole->getRim().h);
            }
            else if(getY() <= pole->getBoard().y + pole->getBoard().h + 30 && getX() + getTextureRealWidth()*2 >= pole->getRim().x + pole->getRim().w - 20 && getX() + getTextureRealWidth() - 10 <= pole->getRim().x + pole->getRim().w - 20 && getX() + getTextureRealWidth()*2 - 10 >= pole->getBoard().x) {
                checkCollision(0, pole->getBoard().y + pole->getBoard().h + 30);
            }
            else {
                checkCollision(0, pole->getRim().y + pole->getRim().h);
            }
        }
        else {
            checkCollision();
        }
    }
    else {
        checkCollision(1180, 0);
    }
}

void Player::checkBallCollision() {
    if(pPos.x + getTextureRealWidth()*2 >= ball->getX()
       && pPos.x + getTextureRealWidth() <= ball->getX() + ball->getTexture().getWidth()
       && pPos.y <= ball->getY() && pPos.y + getTextureRealHeight() >= ball->getY() + ball->getTexture().getHeight()
       && !hasTheBall && !isChangingPerspective && !ball->isPossessed()) {
        if(isJumping && ball->getY() >= pPos.y && ball->getY() + ball->getTexture().getHeight() <= pPos.y + getTextureRealHeight()) {
            hasTheBall = true;
            ball->setPossession(true);
            hasThrownTheBall = false;
            ball->setIsThrown(false);
            ball->setVelocity(0, 0);
            ball->resetDimensions();
            //ball->setPosition(pPos.x + getTextureRealWidth()*2 - 30, pPos.y - 5);
        }
        else if(!isJumping) {
            hasTheBall = true;
            hasThrownTheBall = false;
            ball->setPossession(true);
            ball->setIsThrown(false);
            ball->setVelocity(0, 0);
            if(isDefending) {
                isDefending = false;
            }
            ball->resetDimensions();
            //ball->setPosition(pPos.x + getTextureRealWidth()*2 - 30, pPos.y - 5);

        }
        ball->setHasScored(false);
        hasScored = false;
    }
    else if (hasTheBall && !ball->isThrown()) {
        ball->setPosition(pPos.x + getTextureRealWidth()*2 - 30, pPos.y - 5);
    }
}

bool Player::setNormalStance(std::string path) {
    bool success = Player::pNormalStance.loadFromFile(path);
    if(success) {
        Player::pTexture = Player::pNormalStance;
    }
    else {
        printf("%s\n", SDL_GetError());
    }
    return success;
}

void Player::setNormalStance() {
    Player::pTexture = Player::pNormalStance;
}

bool Player::setDefenceStance(std::string path) {
    return pDefenceStance.loadFromFile(path);
}
