#include "MenuButton.hpp"

MenuButton::MenuButton() : Button::Button() {}

void MenuButton::setCurrentButton(MenuButtons b) {
    MenuButton::currentButton = b;
}

int MenuButton::handleEvents(SDL_Event* e) {
    if(e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        bool inside = true;
        if(x < MenuButton::_BPosition.x) {
            inside = false;
        }
        else if(x > MenuButton::_BPosition.x + MenuButton::_BPosition.w) {
            inside = false;
        }
        else if(y < MenuButton::_BPosition.y) {
            inside = false;
        }
        else if(y > MenuButton::_BPosition.y + MenuButton::_BPosition.h) {
            inside = false;
        }
        if(!inside) {
            MenuButton::_BColor = {0, 0, 0};
        }
        else {
            switch(e->type) {
                case SDL_MOUSEBUTTONDOWN:
                    MenuButton::_BColor = {255, 255, 255};
                    if(MenuButton::currentButton == EXIT) {
                        return 1;
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    MenuButton::_BColor = {255, 0, 0};
                    break;

                case SDL_MOUSEMOTION:
                    MenuButton::_BColor = {255, 0, 0};
                    break;
            }
        }
    }
    return 0;
}
