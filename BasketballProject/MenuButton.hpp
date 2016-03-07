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
public:
    MenuButton();

    int handleEvents(SDL_Event* e);

    void setCurrentButton(MenuButtons b);

};

#endif

