#ifndef MUSIC_BUTTON_HPP
#define MUSIC_BUTTON_HPP
#include "main.hpp"
#include "Button.hpp"

enum MusicSprite {
    UNMUTED = 0,
    MUTED = 1,
    TOTAL_MUSIC_SPRITES = 2
};

class MusicButton : public Button {
    MusicSprite currentSprite;
    SDL_Rect sprites[TOTAL_MUSIC_SPRITES];
public:
    MusicButton();

    void handleEvents(SDL_Event* e);

    void setClip(MusicSprite sprite, int x, int y, int w, int h);

};

#endif
