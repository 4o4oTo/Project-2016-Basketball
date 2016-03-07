#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include "main.hpp"

class Texture {
    SDL_Texture* _Texture;
    int _THeight, _TWidth;
    std::string _Text;
public:
    Texture();

    ~Texture();

    void render(int x, int y, SDL_Rect* clip = NULL);

    bool loadFromFile(std::string path);

    bool loadFromRenderedText(std::string str, SDL_Color color);

    std::string getText();

    void setColor(SDL_Color color);

    void free();

    int getHeight();

    int getWidth();

};
#endif // TEXTURE_HPP
