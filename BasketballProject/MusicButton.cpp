#include "MusicButton.hpp"

MusicButton::MusicButton() : Button::Button() {
    MusicButton::currentSprite = UNMUTED;
}

void MusicButton::setClip(MusicSprite sprite, int x, int y, int w, int h) {
    MusicButton::sprites[sprite].x = x;
    MusicButton::sprites[sprite].y = y;
    MusicButton::sprites[sprite].w = w;
    MusicButton::sprites[sprite].h = h;
}

void MusicButton::handleEvents(SDL_Event* e) {
    if(e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        bool inside = true;
        if(x < MusicButton::_BPosition.x) {
            inside = false;
        }
        else if(x > MusicButton::_BPosition.x + MusicButton::_BPosition.w) {
            inside = false;
        }
        else if(y < MusicButton::_BPosition.y) {
            inside = false;
        }
        else if(y > MusicButton::_BPosition.y + MusicButton::_BPosition.h) {
            inside = false;
        }
        if(!inside) {
            MusicButton::_BColor = {255, 127, 39};
        }
        else {
            switch(e->type) {
                case SDL_MOUSEBUTTONDOWN:
                    if(MusicButton::currentSprite == UNMUTED) {
                        MusicButton::currentSprite = MUTED;
                        Mix_PauseMusic();
                    }
                    else {
                        MusicButton::currentSprite = UNMUTED;
                        Mix_ResumeMusic();
                    }
                    MusicButton::_BColor = {255, 150, 39};
                    break;

                case SDL_MOUSEBUTTONUP:
                    MusicButton::_BColor = {255, 150, 39};
                    break;

                case SDL_MOUSEMOTION:
                    MusicButton::_BColor = {255, 150, 39};
                    break;
            }
        }
    MusicButton::_BTexture.setColor(MusicButton::_BColor);
    MusicButton::_BClip = MusicButton::sprites[MusicButton::currentSprite];
    }
}
