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
                break;

            case SDLK_a:
                if(facing == RIGHT && hasLanded) {
                    frame = 0;
                }
                break;

            case SDLK_LSHIFT:
                if(!isDefending && !isJumping) {
                    frame = 0;
                    isDefending = true;
                }
                pMovementSpeed = 3;
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
        pTexture = pNormalStance;
        isRunning = false;
        isStanding = true;
        if(positioned) {
            pPos.y -= 20;
            positioned = false;
        }
    }
    else if(e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_LSHIFT && hasLanded) {
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
    }
    else if(keyState[SDL_SCANCODE_D]) {
        if(facing == LEFT) {
            facing = RIGHT;
            pFlip = SDL_FLIP_NONE;
        }
        pVelX = pMovementSpeed;
        isRunning = true;
    }
    else if(keyState[SDL_SCANCODE_LSHIFT]) {
        if(!isDefending) {
            isDefending = true;
        }
        if(pMovementSpeed != 3) {
            pMovementSpeed = 3;
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
                pVelY += pGravity;
            }
        }
    }
    pPos.x += pVelX;
    pPos.y += pVelY;
}

void Player::checkCollision(int collider) {
    if(pPos.x < -(pTexture.getWidth()-getTextureRealWidth())) {
        pPos.x = -(pTexture.getWidth()-getTextureRealWidth());
    }
    else if(collider != 0) {
        if(pPos.x + getTextureRealWidth() >= collider) {
            pPos.x = collider - getTextureRealWidth();
        }
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
