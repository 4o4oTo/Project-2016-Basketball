#include "BasketballPole.hpp"

BasketballPole::BasketballPole() {
    polePos.x = 0;
    polePos.y = 0;
    board.x = 0;
    board.y = 0;
    board.w = 0;
    board.h = 0;
    belowBoard.x = 0;
    belowBoard.y = 0;
    belowBoard.w = 0;
    belowBoard.h = 0;
    rim.x = 0;
    rim.y = 0;
    rim.w = 0;
    rim.h = 0;

}

Texture& BasketballPole::getTexture() {
    return poleTexture;
}

void BasketballPole::setPosition(int x, int y) {
    polePos.x = x;
    polePos.y = y;
}

int BasketballPole::getX() {
    return polePos.x;
}

int BasketballPole::getY() {
    return polePos.y;
}

void BasketballPole::render() {
    poleTexture.render(polePos.x, polePos.y);
}

void BasketballPole::setBoardDimensions(int x, int y, int w, int h) {
    board.x = x;
    board.y = y;
    board.w = w;
    board.h = h;
}

void BasketballPole::setDimensionsBelowBoard(int x, int y, int w, int h) {
    belowBoard.x = x;
    belowBoard.y = y;
    belowBoard.w = w;
    belowBoard.h = h;
}

void BasketballPole::setRimDimensions(int x, int y, int w, int h) {
    rim.x = x;
    rim.y = y;
    rim.w = w;
    rim.h = h;
}

SDL_Rect& BasketballPole::getRim() {
    return rim;
}

SDL_Rect& BasketballPole::getBoard() {
    return board;
}

SDL_Rect& BasketballPole::getBelowBoard() {
    return belowBoard;
}

void BasketballPole::free() {
    poleTexture.free();
    polePos.x = 0;
    polePos.y = 0;
    board.x = 0;
    board.y = 0;
    board.w = 0;
    board.h = 0;
    belowBoard.x = 0;
    belowBoard.y = 0;
    belowBoard.w = 0;
    belowBoard.h = 0;
    rim.x = 0;
    rim.y = 0;
    rim.w = 0;
    rim.h = 0;
}
