#include "Texture.hpp"
#include "main.hpp"
#include "MusicButton.hpp"
#include "MenuButton.hpp"
#include "Troy.hpp"

const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 1000;
const int AVG_FPS = 60;
const int TIME_PER_FRAME = 1000 / AVG_FPS;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

//Music
Mix_Music* gMusic = NULL;
MusicButton gMusicButton;

//Menu Background Texture
Texture gBackgroundTexture;
SDL_Rect gBackgroundClip;

//Menu Buttons
MenuButton gMenuButtons[TOTAL_MENU_BUTTONS];

//Font
TTF_Font* gFont = NULL;

//Court texture
Texture gCourt;
SDL_Rect gCourtClip;

//Player
BlackPlayer troy("Troy");

bool init();

bool loadMedia();

void shutdown();

int main(int argc, char* argv[]) {
    if(init()) {
        if(loadMedia()) {
            bool quit = false;
            SDL_Event e;
            Mix_PlayMusic(gMusic, -1);
            Uint8 prevTime = 0;
            Uint8 currTime = 0;
            Uint8 frameTime = 0;
            Uint8 totalFrameTime = 0;
            while(!quit) {

                prevTime = currTime;
                currTime = SDL_GetTicks();
                frameTime = currTime - prevTime;
                totalFrameTime += frameTime;

                while(SDL_PollEvent(&e) != 0) {
                    if(e.type == SDL_QUIT) {
                        quit = true;
                    }
                    for(int i = 0; i < 3; i++) {
                        gMenuButtons[i].handleEvents(&e);
                    }

                    gMusicButton.handleEvents(&e);

                    if(gMenuButtons[PLAY].isClicked()) {
                        troy.handleEvents(&e);
                    }
                }

                SDL_RenderClear(gRenderer);

                if(gMenuButtons[PLAY].isClicked()) {
                    while(totalFrameTime >= TIME_PER_FRAME) {
                        troy.update();
                        totalFrameTime -= TIME_PER_FRAME;
                    }
                    gCourt.render(0, 0, &gCourtClip);
                    troy.render();
                }
                else if(gMenuButtons[OPTIONS].isClicked()) {

                }
                else if(gMenuButtons[EXIT].isClicked()) {
                    quit = true;

                    gBackgroundTexture.render(0, 0, &gBackgroundClip);

                    for(int j = 0; j < 3; j++) {
                        gMenuButtons[j].render();
                    }
                }
                else {

                    gBackgroundTexture.render(0, 0, &gBackgroundClip);

                    for(int j = 0; j < 3; j++) {
                        gMenuButtons[j].render();
                    }
                }

                gMusicButton.render();

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
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

    if(!gCourt.loadFromFile("background/court.png")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gCourtClip.x = 500;
        gCourtClip.y = 20;
        gCourtClip.w = SCREEN_WIDTH;
        gCourtClip.h = SCREEN_HEIGHT;
    }

    if(!gMusicButton.loadTextureFromFile("music/icon.png")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gMusicButton.setClip(UNMUTED, 0, 0, 40, 70);
        gMusicButton.setClip(MUTED, 40, 0, 40, 70);
        gMusicButton.setDimensions(50, 0, 40, 70);
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
        if(!gMenuButtons[PLAY].loadTextureFromText("Play", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[PLAY].setDimensions(430, 30, gMenuButtons[PLAY].getWidth(), gMenuButtons[PLAY].getHeight());
            gMenuButtons[PLAY].setCurrentButton(PLAY);
        }

        //Options
        if(!gMenuButtons[OPTIONS].loadTextureFromText("Options", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[OPTIONS].setDimensions(170, 300, gMenuButtons[OPTIONS].getWidth(), gMenuButtons[OPTIONS].getHeight());
            gMenuButtons[OPTIONS].setCurrentButton(OPTIONS);
        }

        //Exit
        if(!gMenuButtons[EXIT].loadTextureFromText("Exit", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[EXIT].setDimensions(630, 300, gMenuButtons[EXIT].getWidth(), gMenuButtons[EXIT].getHeight());
            gMenuButtons[EXIT].setCurrentButton(EXIT);
        }
    }

    if(!troy.setNormalStance("player/NormalRunning/Run.1.png")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        troy.setInitialPosition((SCREEN_WIDTH - troy.getWidth())/2,(SCREEN_HEIGHT - troy.getHeight())-50);
        troy.setFacingDirection(RIGHT);
        troy.setRunningScenes();
    }

    return success;
}

void shutdown() {
    gMusicButton.free();
    gCourt.free();
    gBackgroundTexture.free();
    for(int i=0; i<3; i++) {
        gMenuButtons[i].free();
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
