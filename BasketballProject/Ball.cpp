#include "Ball.hpp"

Ball::Ball(float power) : bGravity(0.3){
    bouncePower = power;
    bVelX = 0;
    bVelY = 0;
    possessed = false;
    bBall.x = 0;
    bBall.y = 0;
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

void Ball::render() {
    bTexture.render(bBall.x, bBall.y);
}

void Ball::update() {

}

void Ball::checkCollision(int x, int y) {

}
