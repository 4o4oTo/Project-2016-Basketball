#include "Player.hpp"

Player::Player(std::string name) {
    Player::pName = name;
    Player::pFlip = SDL_FLIP_NONE;
    Player::pVelX = 0.0;
    Player::pVelY = 0.0;
    Player::pPos.x = 0;
    Player::pPos.y = 0;
    Player::frame = 0;
    Player::isRunning = false;
    Player::isDefending = false;
    Player::isStanding = true;
}

Player::~Player() {
    Player::pTexture.free();
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
                if(facing == LEFT) {
                    frame = 0;
                    pFlip = SDL_FLIP_NONE;
                    facing = RIGHT;
                }
                break;

            case SDLK_a:
                if(facing == RIGHT) {
                    frame = 0;
                    pFlip = SDL_FLIP_HORIZONTAL;
                    facing = LEFT;
                }
                break;
        }
    }
    else if(e->type == SDL_KEYUP && (e->key.keysym.sym == SDLK_a || e->key.keysym.sym == SDLK_d)) {
        pVelX = 0.0;
        pVelY = 0.0;
        frame = 0;
        pTexture = pNormalStance;
        isRunning = false;
        isStanding = true;
    }
}

void Player::render() {
    Player::pTexture.render(Player::pPos.x, Player::pPos.y, NULL, Player::pFlip);
}

void Player::processInput() {
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);

    if(keyState[SDL_SCANCODE_A]) {
        pVelX = -10;
        isRunning = true;
    }
    else if(keyState[SDL_SCANCODE_D]) {
        pVelX = 10;
        isRunning = true;
    }
    else if(keyState[SDL_SCANCODE_LSHIFT]) {
        isDefending = true;
    }
    pPos.x += pVelX;
    checkCollision();
}

void Player::checkCollision() {
    if(pPos.x < -100) {
        pPos.x = -100;
    }
    else if(pPos.x + pTexture.getWidth() >= SCREEN_WIDTH) {
        pPos.x = SCREEN_WIDTH - pTexture.getWidth();
    }
}

bool Player::setNormalStance(std::string path) {
    bool success = Player::pNormalStance.loadFromFile(path);
    if(success) {
        Player::pTexture = Player::pNormalStance;
    }
    return success;
}
