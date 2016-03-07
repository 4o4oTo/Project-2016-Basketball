#include "Texture.hpp"
#include "main.hpp"
#include "Button.hpp"
#include "MusicButton.hpp"
#include "MenuButton.hpp"

const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 1000;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

//Music
Mix_Music* gMusic = NULL;
Texture gMusicTexture;
SDL_Rect gMusicClip[TOTAL_MUSIC_SPRITES];
MusicButton gMusicButton;

//Background texture
Texture gBackgroundTexture;
SDL_Rect gBackgroundClip;

//Menu
Texture menuChoices[3];

//Font
TTF_Font* gFont = NULL;

//Button
MenuButton gMenuButtons[TOTAL_MENU_BUTTONS];

bool init();

bool loadMedia();

void shutdown();

int main(int argc, char* argv[]) {
    if(init()) {
        if(loadMedia()) {
            bool quit = false;
            SDL_Event e;
            MusicSprite sprite = UNMUTED;
            bool exit = false;
            Mix_PlayMusic(gMusic, -1);
            while(!quit) {
                while(SDL_PollEvent(&e) != 0) {
                    if(e.type == SDL_QUIT) {
                        quit = true;
                    }
                    for(int i = 0; i < 3; i++) {
                       exit = gMenuButtons[i].handleEvents(&e);
                       if(exit) {
                        quit = true;
                        break;
                       }
                    }

                    sprite = (MusicSprite) gMusicButton.handleEvents(&e);
                }

                gMusicTexture.setColor(gMusicButton.getColor());

                SDL_RenderClear(gRenderer);

                gBackgroundTexture.render(0, 0, &gBackgroundClip);

                for(int j = 0; j < 3; j++) {
                    gMenuButtons[j].render(&menuChoices[j]);
                }

                gMusicButton.render(&gMusicTexture, &gMusicClip[sprite]);

                SDL_RenderPresent(gRenderer);
            }
        }
    }
    shutdown();
    return 0;
}

bool init() {
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow("1v1 Basketball game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(gWindow == NULL) {
            printf("%s\n",SDL_GetError());
            success = false;
        }
        else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL) {
                printf("%s\n",SDL_GetError());
                success = false;
            }

            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

            //Different image format initialization
            int imgFlags = IMG_INIT_JPG;
            if(!(IMG_Init(imgFlags) & imgFlags)) {
                printf("%s\n",IMG_GetError());
                success = false;
            }

            //Fonts init
            if(TTF_Init() == -1) {
                printf("%s\n",TTF_GetError());
                success = false;
            }

            //Music init
            if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                printf("%s\n",Mix_GetError());
                success = false;
            }
        }
    }

    return success;
}

bool loadMedia() {
    bool success = true;

    if(!gBackgroundTexture.loadFromFile("background/background.jpg")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gBackgroundClip.h = SCREEN_HEIGHT;
        gBackgroundClip.w = SCREEN_WIDTH;
        gBackgroundClip.x = 450;
        gBackgroundClip.y = 400;
    }

    if(!gMusicTexture.loadFromFile("music/icon.png")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gMusicClip[UNMUTED].x = 0;
        gMusicClip[UNMUTED].y = 0;
        gMusicClip[UNMUTED].w = 40;
        gMusicClip[UNMUTED].h = 70;

        gMusicClip[MUTED].x = 40;
        gMusicClip[MUTED].y = 0;
        gMusicClip[MUTED].w = 40;
        gMusicClip[MUTED].h = 70;

        gMusicButton.setDimensions(50, 0, gMusicClip[MUTED].w, gMusicClip[MUTED].h);
    }

    gMusic = Mix_LoadMUS("music/hustle.wav");
    if(gMusic == NULL) {
        printf("%s\n", Mix_GetError());
        success = false;
    }

    gFont = TTF_OpenFont("font/Pacifico.ttf", 60);
    if(gFont == NULL) {
        printf("%s\n", TTF_GetError());
        success = false;
    }
    else {
        SDL_Color textColor = {0, 0, 0};
        //Play
        if(!menuChoices[0].loadFromRenderedText("Play", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[0].setDimensions(430,30,menuChoices[0].getWidth(),menuChoices[0].getHeight());
            gMenuButtons[0].setCurrentButton(PLAY);
        }

        //Options
        if(!menuChoices[1].loadFromRenderedText("Options", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[1].setDimensions(170,300,menuChoices[1].getWidth(),menuChoices[1].getHeight());
            gMenuButtons[1].setCurrentButton(OPTIONS);
        }

        //Exit
        if(!menuChoices[2].loadFromRenderedText("Exit", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[2].setDimensions(630,300,menuChoices[2].getWidth(),menuChoices[2].getHeight());
            gMenuButtons[2].setCurrentButton(EXIT);
        }
    }
    return success;
}

void shutdown() {
    gBackgroundTexture.free();
    for(int i = 0; i<3; i++) {
        menuChoices[i].free();
    }

    TTF_CloseFont(gFont);
    gFont = NULL;

    Mix_FreeMusic(gMusic);
    gMusic = NULL;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
