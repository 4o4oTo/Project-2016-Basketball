#include "Ball.hpp"

Ball::Ball(float bounce) : bGravity(0.3){
    bouncePower = bounce;
    bVelX = 0.0;
    bVelY = 0.0;
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
    wasChangingPerspective = false;
    hasGoneThroughTheNet = false;
    bBall.x = 0;
    bBall.y = 0;
    changedHeight = 0.0;
    changedHeight = 0.0;
    bMaxPower = 10;
    bInitialY = 0;
    rotationAngle = 0.0;
    dribble = NULL;
    rim = NULL;
    swish = NULL;
    backboard = NULL;
}

Ball::~Ball() {
    bTexture.free();
    bVelX = 0;
    bVelY = 0;
    bouncePower = 0;
    Mix_FreeChunk(backboard);
    Mix_FreeChunk(rim);
    Mix_FreeChunk(swish);
    Mix_FreeChunk(dribble);
    backboard = NULL;
    rim = NULL;
    swish = NULL;
    dribble = NULL;
}

bool Ball::loadSoundEffects() {
    bool success = true;
    dribble = Mix_LoadWAV("music/Dribble.wav");
    if(dribble == NULL) {
        printf("%s\n", Mix_GetError());
        success = false;
    }
    rim = Mix_LoadWAV("music/Rim.wav");
    if(rim == NULL) {
        printf("%s\n", Mix_GetError());
        success = false;
    }
    swish = Mix_LoadWAV("music/Swish.wav");
    if(swish == NULL) {
        printf("%s\n", Mix_GetError());
        success = false;
    }
    backboard = Mix_LoadWAV("music/Backboard.wav");
    if(backboard == NULL) {
        printf("%s\n", Mix_GetError());
        success = false;
    }
    return success;
}

void Ball::passThePole(BasketballPole* p) {
    pole = p;
}

void Ball::setHasScored(bool h) {
    hasGoneThroughTheNet = h;
}

void Ball::changingPerspective(bool b) {
    wasChangingPerspective = b;
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

bool Ball::hasScored() {
    return hasGoneThroughTheNet;
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
    if(changedHeight + h <= bTexture.getHeight()) {
        changedHeight += h;
    }
    if(changedWidth + w <= bTexture.getWidth()) {
        changedWidth += w;
    }
}

void Ball::handleEvents(SDL_Event* e) {
    if(e->type == SDL_KEYDOWN) {
        if(e->key.keysym.sym == SDLK_r || e->key.keysym.sym == SDLK_RETURN) {
            if(possessed && !thrown && bVelX == 0.0 && bVelY == 0.0) {
                bVelX = bouncePower;
                bVelY = -bouncePower;
                firstRotation = true;
                rotationAngle = 0.0;
            }
        }
    }
    else if(e->type == SDL_KEYUP && (e->key.keysym.sym == SDLK_r || e->key.keysym.sym == SDLK_RETURN) && thrown) {
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
    if(keyState[SDL_SCANCODE_R] || keyState[SDL_SCANCODE_RETURN]) {
        if(possessed && !thrown) {
            if(bBall.x < 700) {
                bVelX += bGravity*2.5;
                bVelY -= bGravity/1.1;
                if(bVelY <= -bMaxPower*2) {
                    bVelY = -bMaxPower*2;
                }
            }
            else if(bBall.x >= 700 && bBall.x <= 953) {
                if(wasChangingPerspective) {
                    bVelX += bGravity*0.0001;
                    bVelY -= bGravity/0.7;
                    if(bVelY <= -bMaxPower) {
                        bVelY = -bMaxPower;
                    }
                }
                else {
                    bVelX += bGravity*0.2;
                    bVelY -= bGravity/0.7;
                    if(bVelY <= -bMaxPower) {
                        bVelY = -bMaxPower;
                    }
                }
            }
            else {
                bVelX = 0;
                bVelY -= bGravity/0.5;
                if(bVelY <= -bMaxPower) {
                    bVelY = -bMaxPower;
                }
            }
            if(bVelX >= bMaxPower) {
                bVelX = bMaxPower;
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
        if(!hasCollidedWithThePole()) {
            if(abs(changedWidth-53.0) > 0.00001) {
                changedWidth += 0.5;
            }
            if(abs(changedHeight-50.0) > 0.00001) {
                changedHeight += 0.5;
            }
        }
        if(bBall.y + bTexture.getHeight() > bInitialY + bTexture.getHeight()) {
            checkCollision(0, bInitialY + bTexture.getHeight());
            if(hasGoneThroughTheNet) {
                hasGoneThroughTheNet = false;
                setPosition(100, SCREEN_HEIGHT - bTexture.getHeight());
                setVelocity(0.0, 0.0);
            }
            Mix_PlayChannel(-1, dribble, 0);
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
    if(abs(changedWidth-53.0) <= 1.0 && abs(changedHeight-50.0) <= 1.0) {
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
            if(!wasChangingPerspective) {
                checkCollision(pole->getBoard().x);
            }
        }
        else if(sameLvlAsRim && bBall.x + 2*bTexture.getWidth()/3 <= pole->getRim().x && !hitBoard && thrown) {
            checkCollision(pole->getRim().x);
        }
        else if(sameLvlAsRim && !beforeRim && !behindRim) {
            if(bBall.x <= pole->getRim().x && bBall.y + bTexture.getHeight()/4 <= pole->getRim().y && hitBoard) {
                checkCollision(-(pole->getRim().x), pole->getRim().y);
            }
            else if(bBall.x <= pole->getRim().x + pole->getRim().w - 20 && bBall.x + bTexture.getWidth() >= pole->getRim().x + pole->getRim().w - 18 && bBall.y <= pole->getRim().y) {
                if(!hitBoard) {
                    checkCollision(pole->getRim().x + pole->getRim().w - 20, pole->getRim().y);
                }
                else {
                    checkCollision(pole->getRim().x + pole->getRim().w - 20, 0);
                }
            }
            else if(bBall.y + bTexture.getHeight()/3 >= pole->getRim().y + pole->getRim().h && !hitBoard) {
                checkCollision(pole->getBoard().x);
                Mix_PlayChannel(-1, swish, 0);
                if(!hasGoneThroughTheNet) {
                    hasGoneThroughTheNet = true;
                }
            }
            else if(bBall.x <= pole->getRim().x && !hitBoard) {
                if(wasChangingPerspective) {
                    checkCollision(-(pole->getRim().x), 0);
                }
                else {
                    checkCollision(0, pole->getRim().y);
                }
            }
            else if(bBall.y + bTexture.getHeight()/3 >= pole->getRim().y + pole->getRim().h && hitBoard) {
                Mix_PlayChannel(-1, swish, 0);
                if(!hasGoneThroughTheNet) {
                    hasGoneThroughTheNet = true;
                }
            }
        }
        else if(belowRim && behindRim) {
            checkCollision(pole->getBelowBoard().x, -pole->getBelowBoard().y);
        }
        else{
            checkCollision();
        }
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
                    Mix_PlayChannel(-1, backboard, 0);
                }
                if(x == pole->getBoard().x || x == pole->getRim().x) {
                    collidedWithPole = true;
                    if(x == pole->getRim().x) {
                        Mix_PlayChannel(-1, rim, 0);
                    }
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
                    Mix_PlayChannel(-1, rim, 0);
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
                    Mix_PlayChannel(-1, rim, 0);
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
                    Mix_PlayChannel(-1, rim, 0);
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
                    Mix_PlayChannel(-1, rim, 0);
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
                    if(x == pole->getRim().x) {
                        Mix_PlayChannel(-1, rim, 0);
                    }
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
                    Mix_PlayChannel(-1, backboard, 0);
                }
                if(x == pole->getBoard().x || x == pole->getRim().x) {
                    collidedWithPole = true;
                    if(x == pole->getRim().x) {
                        Mix_PlayChannel(-1, rim, 0);
                    }
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
                    Mix_PlayChannel(-1, rim, 0);
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
