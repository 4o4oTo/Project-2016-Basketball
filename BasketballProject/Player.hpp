#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "main.hpp"
#include "Texture.hpp"

class Player {
    std::string pName;
    Texture pTexture;
public:
    Player(std::string name);

    ~Player();


};
#endif
