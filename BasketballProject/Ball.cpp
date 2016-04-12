#include "Ball.hpp"

Ball::Ball(float bounce) : bGravity(0.3){
    bouncePower = bounce;
    bVelX = 0;
    bVelY = 0;
    possessed = false;
    thrown = false;
    bBall.x = 0;
    bBall.y = 0;
    bMaxPower = 10;
    bInitialY = 0;
}

Ball::~Ball() {
    bTexture.free();
    bVelX = 0;
    bVelY = 0;
    bouncePower = 0;
}

void Ball::setPossession(bool possession) {
    possessed = possession;
}

bool Ball::isPossessed() {
    return possessed;
}

void Ball::setIsThrown(bool t) {
    thrown = t;
}

bool Ball::isThrown() {
    return thrown;
}

Texture& Ball::getTexture() {
    return bTexture;
}

int Ball::getY() {
    return bBall.y;
}

int Ball::getX() {
    return bBall.x;
}

void Ball::setPosition(int x, int y) {
    bBall.x = x;
    bBall.y = y;
}

void Ball::handleEvents(SDL_Event* e) {
    if(e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_r:
                if(possessed && !thrown && bVelX == 0 && bVelY == 0) {
                    bVelX = bouncePower;
                    bVelY = -bouncePower;
                }
                break;
        }
    }
    else if(e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_r && possessed && !thrown) {
        bInitialY = SCREEN_HEIGHT - bTexture.getHeight();
        thrown = true;
        possessed = false;
    }
}

void Ball::render() {
    if(!possessed) {
        bTexture.render(bBall.x, bBall.y);
    }
}

void Ball::processInput() {
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);

    if(keyState[SDL_SCANCODE_R]) {
        if(possessed && !thrown) {
            bVelX += bGravity*2.8;
            bVelY -= bGravity/1.1;
            printf("%f\n", bVelX);
            if(bVelX >= bMaxPower) {
                bVelX = bMaxPower;
            }
        }
    }
}

void Ball::update() {
    processInput();

    if(thrown && !possessed) {
        if(bBall.y > bInitialY) {
            thrown = false;
            bVelX = 0;
            bVelY = 0;
            bBall.y = bInitialY;
        }
        else {
            if(bVelY >= 0) {
                bVelY += bGravity*1.5;
            }
            else {
                bVelY += bGravity*1.5;
            }
            if(bVelX >= 0) {
                bVelX -= bGravity/20;
            }
        }
        bBall.x += bVelX;
        bBall.y += bVelY;
    }
}

void Ball::checkCollision(int x, int y) {

}
