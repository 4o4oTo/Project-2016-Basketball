#ifndef MENU_BUTTON_HPP
#define MENU_BUTTON_HPP
#include "main.hpp"
#include "Button.hpp"

enum MenuButtons {
    PLAY = 0,
    OPTIONS = 1,
    EXIT = 2,
    TOTAL_MENU_BUTTONS = 3
};

class MenuButton : public Button {
    MenuButtons currentButton;
    bool clicked;
public:
    MenuButton();

    void handleEvents(SDL_Event* e);

    bool isClicked();

    void click();

    void unclick();

    void setCurrentButton(MenuButtons b);

    void setClip(int x, int y, int w, int h);

};

#endif

