#ifndef MENU_BUTTON_HPP
#define MENU_BUTTON_HPP
#include "main.hpp"
#include "Button.hpp"

enum MenuButtons {
    PLAY = 0,
    OPTIONS = 1,
    EXIT = 2,
    RESUME = 3,
    MAIN_MENU = 4,
    TOTAL_MENU_BUTTONS = 5
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

};

#endif

