#include "Player.hpp"

const float Player::pGravity = 0.1;

Player::Player(std::string name) {
    Player::pName = name;
    Player::pFlip = SDL_FLIP_NONE;
    Player::pVelX = 0.0;
    Player::pVelY = 0.0;
    Player::pPos.x = 0;
    Player::pPos.y = 0;
    Player::frame = 0;
    Player::positioned = false;
    Player::isRunning = false;
    Player::isDefending = false;
    Player::isStanding = true;
    Player::isJumping = false;
    Player::hasLanded = true;
    Player::pMaxJumpHeight = 0;
    Player::textureRealHeight = 0;
    Player::textureRealWidth = 0;
    Player::behindRim = false;
    Player::belowRim = false;
    Player::hasTheBall = false;
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
}

Texture& Player::getTexture() {
    return pTexture;
}

bool Player::isLastFrame(int frames_count) {
    return frame == frames_count;
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

void Player::handleEvents(SDL_Event* e) {
    if(e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_d:
                if(facing == LEFT && hasLanded) {
                    frame = 0;
                }
                isStanding = false;
                break;

            case SDLK_a:
                if(facing == RIGHT && hasLanded) {
                    frame = 0;
                }
                isStanding = false;
                break;

            case SDLK_LSHIFT:
                if(!isDefending && !isJumping && !hasTheBall) {
                    frame = 0;
                    isDefending = true;
                    if(pMovementSpeed != 3) {
                        pMovementSpeed = 3;
                    }
                }
                break;
            case SDLK_w:
                if(!isJumping) {
                    frame = 0;
                    pInitialY = pPos.y;
                    isJumping = true;
                    pVelY = pJumpPower;
                    pMovementSpeed = 3;
                    hasLanded = false;
                }
                break;
        }
    }
    else if(e->type == SDL_KEYUP && (e->key.keysym.sym == SDLK_a || e->key.keysym.sym == SDLK_d) && hasLanded) {
        pVelX = 0.0;
        frame = 0;
        if(!hasTheBall) {
            pTexture = pNormalStance;
        }
        isRunning = false;
        isStanding = true;
        if(positioned) {
            pPos.y -= 20;
            positioned = false;
        }
    }
    else if(e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_LSHIFT && hasLanded && !hasTheBall) {
        isDefending = false;
        frame = 0;
        pMovementSpeed = 10;
        if(positioned) {
            pPos.y -= 20;
            positioned = false;
        }
        if(!isRunning) {
            pTexture = pNormalStance;
        }
    }
}

void Player::render() {
    Player::pTexture.render(Player::pPos.x, Player::pPos.y, NULL, Player::pFlip);
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

    if(keyState[SDL_SCANCODE_A]) {
        if(facing == RIGHT && hasLanded) {
            facing = LEFT;
            pFlip = SDL_FLIP_HORIZONTAL;
        }
        pVelX = -pMovementSpeed;
        isRunning = true;
        isStanding = false;
    }
    else if(keyState[SDL_SCANCODE_D]) {
        if(facing == LEFT) {
            facing = RIGHT;
            pFlip = SDL_FLIP_NONE;
        }
        pVelX = pMovementSpeed;
        isRunning = true;
        isStanding = false;
    }
    else if(keyState[SDL_SCANCODE_LSHIFT]) {
        if(!isDefending && !isJumping && !hasTheBall) {
            isDefending = true;
            if(pMovementSpeed != 3) {
                pMovementSpeed = 3;
            }
        }

    }
    if(isJumping) {
        isRunning = false;
        isDefending = false;
        if(pPos.y > pInitialY) {
            isJumping = false;
            pMovementSpeed = 10;
            pVelY = 0.0;
            pVelX = 0.0;
            pPos.y = pInitialY;
            isStanding = true;
            hasLanded = true;
            pTexture = pNormalStance;
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
    if(pPos.x < -(getTexture().getWidth() - getTextureRealWidth())/2) {
        pPos.x = -(getTexture().getWidth() - getTextureRealWidth())/2;
    }
    else if(x != 0 && y == 0) {
        if(pPos.x + getTextureRealWidth() >= x) {
            pPos.x = x - (getTexture().getWidth() - getTextureRealWidth())/2;
            pVelX = 0;
        }
    }
    else if(y != 0 && x == 0) {
        if(pPos.y <= y) {
            pPos.y = y;
            pVelY = -(pVelY)/2;
        }
    }
    else if(y != 0 && x != 0) {
        if(pPos.x + getTextureRealWidth() >= x) {
            pPos.x = x - (getTexture().getWidth() - getTextureRealWidth())/2;
            pVelX = 0;
        }
        if(pPos.y <= y) {
            pPos.y = y;
            pVelY = -(pVelY)/2;
        }
    }
}

void Player::checkBasketballPoleCollision(BasketballPole* pole) {
    if(getX() > pole->getRim().x + pole->getRim().w) {
        behindRim = true;
    }
    else {
        behindRim = false;
    }
    if(getX() + getTextureRealWidth()/2 >= pole->getRim().x && getX() <= pole->getRim().x + pole->getRim().w) {
       belowRim = true;
    }
    else {
        belowRim = false;
    }
    if(getY() <= pole->getRim().y && !behindRim && !belowRim) {
        checkCollision(pole->getRim().x);
    }
    else if(behindRim) {
        checkCollision(pole->getBelowBoard().x, pole->getBelowBoard().y);
    }
    else if(belowRim) {
        if(getY() <= pole->getRim().y + pole->getRim().h && getX() + getTextureRealWidth()/2 <= pole->getBoard().x) {
            checkCollision(pole->getBoard().x, pole->getRim().y + pole->getRim().h);
        }
        else {
            checkCollision(0, pole->getRim().y + pole->getRim().h);
        }
    }
    else {
        checkCollision();
    }
}

void Player::checkBallCollision(Ball* ball) {
    if(pPos.x + getTextureRealWidth() >= ball->getX() && pPos.x + getTextureRealWidth() <= ball->getX() + ball->getTexture().getWidth() && pPos.y <= ball->getY() && !hasTheBall) {
        hasTheBall = true;
        ball->setPossession(true);
        ball->hide();
        ball->setPosition(pPos.x, pPos.y);
    }
    else if (hasTheBall) {
        ball->setPosition(pPos.x, pPos.y);
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

bool Player::setDefenceStance(std::string path) {
    return pDefenceStance.loadFromFile(path);
}
