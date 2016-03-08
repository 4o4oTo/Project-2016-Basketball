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
public:
    MusicButton();

    void handleEvents(SDL_Event* e);

    MusicSprite getCurrentSprite();

};

#endif
