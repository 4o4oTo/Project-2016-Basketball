#include "Ball.hpp"

Ball::Ball(float bounce) : bGravity(0.3){
    bouncePower = bounce;
    bVelX = 0;
    bVelY = 0;
    possessed = false;
    thrown = false;
    sameLvlAsRim = false;
    aboveRim = false;
    belowRim = true;
    hitBoard = false;
    behindRim = false;
    beforeRim = true;
    firstRotation = true;
    bBall.x = 0;
    bBall.y = 0;
    bMaxPower = 10;
    bInitialY = 0;
    rotationAngle = 0.0;
}

Ball::~Ball() {
    bTexture.free();
    bVelX = 0;
    bVelY = 0;
    bouncePower = 0;
}

void Ball::passThePole(BasketballPole* p) {
    pole = p;
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

void Ball::setVelocity(float xVel, float yVel) {
    bVelX = xVel;
    bVelY = yVel;
}

void Ball::handleEvents(SDL_Event* e) {
    if(e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_r:
                if(possessed && !thrown && bVelX == 0 && bVelY == 0) {
                    bVelX = bouncePower;
                    bVelY = -bouncePower;
                    firstRotation = true;
                }
                break;
        }
    }
    else if(e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_r && thrown) {
        bInitialY = SCREEN_HEIGHT - bTexture.getHeight();
        possessed = false;
    }
}

void Ball::render() {
    if(!possessed) {
        if(thrown) {
            bTexture.render(bBall.x, bBall.y, NULL, SDL_FLIP_NONE, rotationAngle);
        }
        else {
            bTexture.render(bBall.x, bBall.y);
        }
    }
}

void Ball::processInput() {
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);

    if(keyState[SDL_SCANCODE_R]) {
        if(possessed && !thrown) {
            bVelX += bGravity*2.8;
            bVelY -= bGravity/1.1;
            if(bVelX >= bMaxPower) {
                bVelX = bMaxPower;
            }
            if(bVelY <= -bMaxPower*2) {
                bVelY = -bMaxPower*2;
            }
        }
    }
}

void Ball::update() {
    processInput();

    if(possessed && hitBoard) {
        hitBoard = false;
    }

    if(thrown && !possessed) {
        if(bBall.y + bTexture.getHeight() > bInitialY + bTexture.getHeight()) {
            checkCollision(0, bInitialY + bTexture.getHeight());
        }
        if(rotationAngle > 0) {
            rotationAngle += 8;
        }
        else {
            rotationAngle -= 8;
        }
        bVelY += bGravity*1.5;
        if(bVelX > 0) {
            bVelX -= bGravity/15;
        }
        else if(bVelX < 0) {
            bVelX += bGravity/15;
        }
        if(bVelX == 0.0 && bVelY == 0.0) {
            bBall.y = bInitialY;
            thrown = false;
            rotationAngle = 0.0;
        }
        else {
            bBall.x += bVelX;
            bBall.y += bVelY;
        }
        //checkCollision();
    }
}

void Ball::checkCollisionWithPole() {
    if(bBall.x + bTexture.getWidth() < pole->getRim().x) {
        beforeRim = true;
        behindRim = false;
    }
    else if(bBall.x >= pole->getRim().x + pole->getRim().w) {
        behindRim = true;
        beforeRim = false;
    }
    else {
        behindRim = false;
        beforeRim = false;
    }
    if(bBall.y + bTexture.getHeight() < pole->getRim().y) {
        aboveRim = true;
        sameLvlAsRim = false;
        belowRim = false;
    }
    else if(bBall.y <= pole->getRim().y && bBall.y + bTexture.getHeight() >= pole->getRim().y + pole->getRim().h) {
        sameLvlAsRim = true;
        aboveRim = false;
        belowRim = false;
    }
    else if(bBall.y > pole->getRim().y + bTexture.getHeight()) {
        belowRim = true;
        aboveRim = false;
        sameLvlAsRim = false;
    }
    if(aboveRim) {
        checkCollision(pole->getBoard().x);
    }
    else if(sameLvlAsRim && bBall.x + 2*bTexture.getWidth()/3 <= pole->getRim().x && !hitBoard) {
        checkCollision(pole->getRim().x);
    }
    else if(sameLvlAsRim && !beforeRim && !behindRim) {
        if(bBall.x < pole->getRim().x && bBall.x + bTexture.getWidth() > pole->getRim().x) {
            checkCollision(-(pole->getRim().x), pole->getRim().y);
        }
        else if(bBall.x + bTexture.getWidth() >= pole->getRim().x + pole->getRim().w - 5) {
            checkCollision(pole->getRim().x + pole->getRim().w - 5, pole->getRim().y);
        }
    }
    else if(belowRim && behindRim) {
        checkCollision(pole->getBelowBoard().x, -pole->getBelowBoard().y);
    }
}

void Ball::checkCollision(int x, int y) {
    if(bBall.x <= 0) {
        bBall.x = 0;
        bVelX = -bVelX;
        bVelY = bVelY;
        rotationAngle = -rotationAngle;
    }
    else if(x != 0 && y == 0) {
        if(x > 0) {
            if(bBall.x + bTexture.getWidth() >= x) {
                if(x == pole->getBoard().x) {
                    hitBoard = true;
                }
                else {
                    hitBoard = false;
                }
                bBall.x = x - bTexture.getWidth();
                bVelX = -bVelX/1.5;
                bVelY = bVelY/1.5;
                rotationAngle = -rotationAngle;
            }
        }
        else {
            x = -x;
            if(bBall.x <= x) {
                bBall.x = x;
                bVelX = -bVelX/1.5;
                bVelY = bVelY/1.5;
                rotationAngle = -rotationAngle;
            }
        }

    }
    else if(x == 0 && y != 0) {
        if(y > 0) {
            if(bBall.y + bTexture.getHeight() > y) {
                bBall.y  = y - bTexture.getHeight();
                bVelY = -bVelY/1.5;
                if(firstRotation) {
                    rotationAngle = -rotationAngle;
                    firstRotation = false;
                }
            }
        }
        else {
            y = -y;
            if(bBall.y < y) {
                bBall.y = y;
                bVelY = -bVelY/1.5;
                if(firstRotation) {
                    rotationAngle = -rotationAngle;
                    firstRotation = false;
                }
            }
        }
    }
    else if(x != 0 && y != 0) {
        if(y > 0) {
            if(bBall.y + bTexture.getHeight() > y) {
                bBall.y  = y - bTexture.getHeight();
                bVelY = -bVelY/1.5;
                if(firstRotation) {
                    rotationAngle = -rotationAngle;
                    firstRotation = false;
                }
            }
        }
        else {
            y = -y;
            if(bBall.y < y) {
                bBall.y = y;
                bVelY = -bVelY/1.5;
                if(firstRotation) {
                    rotationAngle = -rotationAngle;
                    firstRotation = false;
                }
            }
        }
        if(x > 0) {
            if(bBall.x + bTexture.getWidth() >= x) {
                if(x == pole->getBoard().x) {
                    hitBoard = true;
                }
                else {
                    hitBoard = false;
                }
                bBall.x = x - bTexture.getWidth();
                bVelX = -bVelX/1.5;
                bVelY = bVelY/1.5;
                rotationAngle = -rotationAngle;
            }
        }
        else {
            x = -x;
            if(bBall.x <= x) {
                bBall.x = x;
                bVelX = -bVelX/1.5;
                bVelY = bVelY/1.5;
                rotationAngle = -rotationAngle;
            }
        }
    }
}
