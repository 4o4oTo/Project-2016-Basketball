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
    collidedWithPole = false;
    behindRim = false;
    beforeRim = true;
    firstRotation = true;
    bBall.x = 0;
    bBall.y = 0;
    changedHeight = 0.0;
    changedHeight = 0.0;
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

void Ball::setHasCollidedWithThePole(bool b) {
    collidedWithPole = b;
}

void Ball::setPossession(bool possession) {
    possessed = possession;
}

bool Ball::isPossessed() {
    return possessed;
}

bool Ball::hasCollidedWithThePole() {
    return collidedWithPole;
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
    if(abs(changedHeight-0.0) < 0.00001 && abs(changedWidth-0.0) < 0.00001) {
        changedHeight = (float) bTexture.getHeight();
        changedWidth = (float) bTexture.getWidth();
    }
}

void Ball::resetDimensions() {
    changedHeight = bTexture.getHeight();
    changedWidth = bTexture.getWidth();
}

void Ball::setVelocity(float xVel, float yVel) {
    bVelX = xVel;
    bVelY = yVel;
}

void Ball::setChangedDimensions(float w, float h) {
    changedHeight += h;
    changedWidth += w;
    printf("%f %f\n", changedWidth, changedHeight);
}

void Ball::handleEvents(SDL_Event* e) {
    if(e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_r:
                if(possessed && !thrown && bVelX == 0.0 && bVelY == 0.0) {
                    bVelX = bouncePower;
                    bVelY = -bouncePower;
                    firstRotation = true;
                    rotationAngle = 0.0;
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
        //if(thrown) {
            bTexture.render(bBall.x, bBall.y, NULL, SDL_FLIP_NONE, rotationAngle, NULL, changedWidth, changedHeight);
//        }
//        else {
//            bTexture.render(bBall.x, bBall.y);
//        }
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
        if(!hasCollidedWithThePole() && abs(changedWidth-53.0) < 0.00001 && abs(changedHeight-50.0) < 0.00001) {
            changedHeight += 0.2;
            changedWidth += 0.2;
        }
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
        if(bVelX - bGravity/15 > 1) {
            bVelX -= bGravity/15;
        }
        else if(bVelX + bGravity/15 < 1) {
            bVelX += bGravity/15;
        }
        if((abs(bVelX-0.0) < 0.00001) && (abs(bVelY-0.0) < 0.00001) && bBall.y >= bInitialY) {
            bBall.y = bInitialY;
            thrown = false;
            hitBoard = false;
        }
        else {
            bBall.x += bVelX;
            bBall.y += bVelY;
        }
        //checkCollision();
    }
}

void Ball::checkCollisionWithPole() {
    if(bBall.x + bTexture.getWidth() <= pole->getRim().x) {
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
    else if(bBall.y > pole->getRim().y + pole->getRim().h) {
        belowRim = true;
        aboveRim = false;
        sameLvlAsRim = false;
    }
    if(aboveRim) {
        checkCollision(pole->getBoard().x);
    }
    else if(sameLvlAsRim && bBall.x + 2*bTexture.getWidth()/3 <= pole->getRim().x && !hitBoard && thrown) {
        checkCollision(pole->getRim().x);
    }
    else if(sameLvlAsRim && !beforeRim && !behindRim) {
        if(bBall.x <= pole->getRim().x && bBall.y + bTexture.getHeight()/4 <= pole->getRim().y && hitBoard) {
            checkCollision(-(pole->getRim().x), pole->getRim().y);
        }
        else if(bBall.x <= pole->getRim().x + pole->getRim().w - 15 && bBall.x + bTexture.getWidth() >= pole->getRim().x + pole->getRim().w && bBall.y <= pole->getRim().y) {
            checkCollision(pole->getRim().x + pole->getRim().w - 15, pole->getRim().y);
        }
        else if(bBall.y + bTexture.getHeight()/2 >= pole->getRim().y + pole->getRim().h && !hitBoard) {
            checkCollision(pole->getBoard().x);
        }
        else if(bBall.x <= pole->getRim().x && !hitBoard) {
            checkCollision(0, pole->getRim().y);
        }
    }
    else if(belowRim && behindRim) {
        checkCollision(pole->getBelowBoard().x, -pole->getBelowBoard().y);
    }
    else{
        checkCollision();
    }
}

void Ball::checkCollision(int x, int y) {
    if(bBall.x <= 0) {
        bBall.x = 0;
        bVelX = -bVelX;
        rotationAngle = -rotationAngle;
    }
    else if(x != 0 && y == 0) {
        if(x > 0) {
            if(bBall.x + bTexture.getWidth() >= x) {
                if(x == pole->getBoard().x) {
                    hitBoard = true;
                }
                if(x == pole->getBoard().x || x == pole->getRim().x) {
                    collidedWithPole = true;
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
                if(x == pole->getRim().x + pole->getRim().w - 5) {
                    collidedWithPole = true;
                }
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
                if(y == pole->getRim().y) {
                    collidedWithPole = true;
                }
                bVelY = -bVelY/1.5;
                bBall.y  = y - bTexture.getHeight();
                if(firstRotation) {
                    rotationAngle = -rotationAngle;
                    firstRotation = false;
                }
            }
        }
        else {
            y = -y;
            if(bBall.y <= y) {
                if(y == pole->getRim().y) {
                    collidedWithPole = true;
                }
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
            if(bBall.y + bTexture.getHeight() >= y) {
                if(y == pole->getRim().y) {
                    collidedWithPole = true;
                }
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
            if(bBall.y <= y) {
                if(y == pole->getRim().y || y == pole->getBelowBoard().y) {
                    collidedWithPole = true;
                }
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
                if(x == pole->getBoard().x || x == pole->getRim().x) {
                    collidedWithPole = true;
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
                if(x == pole->getRim().x + pole->getRim().w - 5) {
                    collidedWithPole = true;
                }
                bBall.x = x;
                bVelX = -bVelX/1.5;
                bVelY = bVelY/1.5;
                rotationAngle = -rotationAngle;
            }
        }
    }
    if(abs(bVelY) < 2.5 && bBall.y >= bInitialY) {
        bVelY = 0.0;
    }
    if(abs(bVelX) < 2 && bBall.y >= bInitialY && abs(bVelY-0.0) < 0.000001) {
        bVelX = 0.0;
    }
}
