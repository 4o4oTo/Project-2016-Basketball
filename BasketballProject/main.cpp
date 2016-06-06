#include "Texture.hpp"
#include "main.hpp"
#include "MusicButton.hpp"
#include "MenuButton.hpp"
#include "BlackPlayer.hpp"
#include "WhitePlayer.hpp"
#include "BasketballPole.hpp"
#include "Ball.hpp"

const int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN)-100;
const int AVG_FPS = 60;
const int TIME_PER_FRAME = 1000 / AVG_FPS;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

//Victory background texture
Texture gVictoryBackgroundTexture;
SDL_Rect gVictoryBackgroundClip;
Texture gVictoryText;
MenuButton gNewGameButton;
bool gameOver = false;
Mix_Chunk *gCheer;

//Reset the game
bool resetGame = false;

//Pause
bool gamePaused = false;

//Music
Mix_Music* gMusic = NULL;
MusicButton gMusicButton;

//Menu Background Texture
Texture gBackgroundTexture;
SDL_Rect gBackgroundClip;

//Menu Buttons
MenuButton gMenuButtons[TOTAL_MENU_BUTTONS];
MenuButton gOptionButton;
Texture gOptionText[17];

//Font
TTF_Font* gFont = NULL;

//Court texture
Texture gCourt;
SDL_Rect gCourtClip;

//Player
BlackPlayer troy("Troy");
WhitePlayer zac("Zac");

//Entities
BasketballPole gBasketballPole;
Ball gBall(5);

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
            srand(time(NULL));
            while(!quit) {
                prevTime = currTime;
                currTime = SDL_GetTicks();
                frameTime = currTime - prevTime;
                totalFrameTime += frameTime;

                while(SDL_PollEvent(&e) != 0) {
                    if(e.type == SDL_QUIT) {
                        quit = true;
                    }
                    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p) {
                        if(!gamePaused) {
                            gamePaused = true;
                        }
                        else {
                            gamePaused = false;
                        }
                    }
                    if(!gamePaused && !gameOver && !gMenuButtons[OPTIONS].isClicked()) {
                        for(int i = PLAY; i < EXIT+1; i++) {
                            gMenuButtons[i].handleEvents(&e);
                        }
                    }
                    else if(gamePaused && !gameOver && !gMenuButtons[OPTIONS].isClicked()){
                        for(int i = RESUME; i < TOTAL_MENU_BUTTONS; i++) {
                            gMenuButtons[i].handleEvents(&e);
                        }
                    }
                    else if(gameOver) {
                        gNewGameButton.handleEvents(&e);
                        gMenuButtons[MAIN_MENU].handleEvents(&e);
                    }
                    else if(gMenuButtons[OPTIONS].isClicked()) {
                        gOptionButton.handleEvents(&e);
                    }

                    gMusicButton.handleEvents(&e);

                    if(gMenuButtons[PLAY].isClicked()) {
                        troy.handleEvents(&e);
                        zac.handleEvents(&e);
                        gBall.handleEvents(&e);
                    }
                }

                SDL_RenderClear(gRenderer);

                if(gMenuButtons[PLAY].isClicked()) {
                    if(!gamePaused && !gameOver) {
                        if(resetGame) {
                            troy.setNormalStance();
                            troy.setInitialPosition((SCREEN_WIDTH - troy.getTexture().getWidth())/2,(SCREEN_HEIGHT - troy.getTextureRealHeight()));
                            troy.setFacingDirection(RIGHT);
                            zac.setNormalStance();
                            zac.setInitialPosition((SCREEN_WIDTH - (zac.getTexture().getWidth())/3),(SCREEN_HEIGHT - zac.getTextureRealHeight()));
                            zac.setFacingDirection(RIGHT);
                            gBall.setPosition(100, SCREEN_HEIGHT - gBall.getTexture().getHeight());
                            gBall.setVelocity(0.0, 0.0);
                            troy.setInitialScore();
                            zac.setInitialScore();
                            resetGame = false;
                        }
                        while(totalFrameTime >= TIME_PER_FRAME) {
                            troy.update();
                            zac.update();
                            gBall.update();
                            troy.checkBallCollision();
                            zac.checkBallCollision();
                            troy.checkBasketballPoleCollision(&gBasketballPole);
                            zac.checkBasketballPoleCollision(&gBasketballPole);
                            troy.checkPlayerCollision(&zac);
                            zac.checkPlayerCollision(&troy);
                            totalFrameTime -= TIME_PER_FRAME;
                        }
                        if(!gBall.isPossessed()) {
                            gBall.checkCollisionWithPole();
                        }
                        gCourt.render(0, 0, &gCourtClip);\
                        if(troy.isBehindPlayer(&zac)) {
                            troy.render();
                            zac.render();
                        }
                        else {
                            zac.render();
                            troy.render();
                        }
                        gBall.render();
                        gBasketballPole.render();
                        troy.renderScoreIndicator();
                        zac.renderScoreIndicator();
                        if(troy.hasWon() || zac.hasWon()) {
                            gameOver = true;
                            SDL_Color textColor = {255, 0, 0, 0};
                            if(troy.hasWon()) {
                               gVictoryText.loadFromRenderedText(troy.getName() + " has won!", textColor);
                                troy.setVictory(false);
                            }
                            else {
                                gVictoryText.loadFromRenderedText(zac.getName() + " has won!", textColor);
                                zac.setVictory(false);
                            }

                            Mix_PlayChannel(-1, gCheer, 0);
                        }
                    }
                    else if(gamePaused && !gameOver){
                        gBackgroundTexture.render(0, 0, &gBackgroundClip);
                        gMenuButtons[RESUME].render();
                        gMenuButtons[MAIN_MENU].render();
                        if(gMenuButtons[RESUME].isClicked()) {
                            gamePaused = false;
                            gMenuButtons[RESUME].unclick();
                        }
                        else if(gMenuButtons[MAIN_MENU].isClicked()) {
                            gMenuButtons[PLAY].unclick();
                            gamePaused = false;
                            gMenuButtons[MAIN_MENU].unclick();
                        }
                    }
                    else if(gameOver) {
                        gVictoryBackgroundTexture.render(0, 0, &gVictoryBackgroundClip);
                        gVictoryText.render(100, 200);
                        gNewGameButton.render();
                        gMenuButtons[MAIN_MENU].render();
                        if(gNewGameButton.isClicked()) {
                            gameOver = false;
                            resetGame = true;
                            gNewGameButton.unclick();
                        }
                        else if(gMenuButtons[MAIN_MENU].isClicked()) {
                            gMenuButtons[PLAY].unclick();
                            gameOver = false;
                            gMenuButtons[MAIN_MENU].unclick();
                        }
                    }
                }
                else if(gMenuButtons[OPTIONS].isClicked()) {
                    gBackgroundTexture.render(0, 0, &gBackgroundClip);
                    gOptionButton.render();
                    if(gOptionButton.isClicked()){
                        gMenuButtons[OPTIONS].unclick();
                        gOptionButton.unclick();
                    }
                    gOptionText[0].render((SCREEN_WIDTH-gOptionText[0].getWidth())/2, 30);
                    gOptionText[1].render(50, 100);
                    gOptionText[2].render(20, 170);
                    gOptionText[3].render(20, 240);
                    gOptionText[4].render(20, 310);
                    gOptionText[5].render(20, 380);
                    gOptionText[6].render(20, 450);
                    gOptionText[7].render(20, 520);
                    gOptionText[8].render(20, 590);
                    gOptionText[9].render(SCREEN_WIDTH - gOptionText[9].getWidth() - 40, 100);
                    gOptionText[10].render(SCREEN_WIDTH - gOptionText[10].getWidth(), 170);
                    gOptionText[11].render(SCREEN_WIDTH - gOptionText[11].getWidth(), 240);
                    gOptionText[12].render(SCREEN_WIDTH - gOptionText[12].getWidth(), 310);
                    gOptionText[13].render(SCREEN_WIDTH - gOptionText[13].getWidth(), 380);
                    gOptionText[14].render(SCREEN_WIDTH - gOptionText[14].getWidth(), 450);
                    gOptionText[15].render(SCREEN_WIDTH - gOptionText[15].getWidth(), 520);
                    gOptionText[16].render(SCREEN_WIDTH - gOptionText[16].getWidth(), 590);
                }
                else if(gMenuButtons[EXIT].isClicked()) {
                    quit = true;

                    gBackgroundTexture.render(0, 0, &gBackgroundClip);

                    for(int j = PLAY; j < EXIT+1; j++) {
                        gMenuButtons[j].render();
                    }
                }
                else {

                    gBackgroundTexture.render(0, 0, &gBackgroundClip);

                    for(int j = PLAY; j < EXIT+1; j++) {
                        gMenuButtons[j].render();
                    }

                    if(!resetGame) {
                        resetGame = true;
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
        gWindow = SDL_CreateWindow("1v1 Basketball game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); //SDL_WINDOW_FULLSCREEN_DESKTOP);
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

    SDL_Color textColor = {0, 0, 0, 0};

    if(!gBackgroundTexture.loadFromFile("background/background.jpg")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gBackgroundClip.h = SCREEN_HEIGHT;
        gBackgroundClip.w = SCREEN_WIDTH;
        gBackgroundClip.x = SCREEN_WIDTH*0.2;
        gBackgroundClip.y = SCREEN_HEIGHT*0.4;
    }

    if(!gVictoryBackgroundTexture.loadFromFile("background/victory_background.jpg")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gVictoryBackgroundClip.h = SCREEN_HEIGHT;
        gVictoryBackgroundClip.w = SCREEN_WIDTH;
        gVictoryBackgroundClip.x = 0;
        gVictoryBackgroundClip.y = 0;
    }

    if(!gCourt.loadFromFile("background/court2.png")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gCourtClip.x = 600;
        gCourtClip.y = -150;
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

    gCheer = Mix_LoadWAV("music/Cheer.wav");
    if(gCheer == NULL) {
        printf("%s\n", Mix_GetError());
        success = false;
    }

    gFont = TTF_OpenFont("font/Pacifico.ttf", 60);
    if(gFont == NULL) {
        printf("%s\n", TTF_GetError());
        success = false;
    }
    else {
        //Play
        if(!gMenuButtons[PLAY].loadTextureFromText("Play", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[PLAY].setDimensions(SCREEN_WIDTH*0.45, SCREEN_HEIGHT*0.16, gMenuButtons[PLAY].getWidth(), gMenuButtons[PLAY].getHeight());
            gMenuButtons[PLAY].setCurrentButton(PLAY);
        }

        //Options
        if(!gMenuButtons[OPTIONS].loadTextureFromText("Options", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[OPTIONS].setDimensions(SCREEN_WIDTH*0.25, SCREEN_HEIGHT*0.5, gMenuButtons[OPTIONS].getWidth(), gMenuButtons[OPTIONS].getHeight());
            gMenuButtons[OPTIONS].setCurrentButton(OPTIONS);
        }

        if(!gOptionButton.loadTextureFromText("Back" , textColor)) {
            success = false;
        }
        else {
            gOptionButton.setDimensions(SCREEN_WIDTH*0.45, SCREEN_HEIGHT*0.85, gOptionButton.getWidth(), gOptionButton.getHeight());
        }   gOptionButton.setCurrentButton(BACK);

        if(!gOptionText[0].loadFromRenderedText("Controls :", textColor)) {
            success = false;
        }
        if(!gOptionText[1].loadFromRenderedText("Player 1 :", textColor)) {
            success = false;
        }
        if(!gOptionText[2].loadFromRenderedText("A - Goes left", textColor)) {
            success = false;
        }
        if(!gOptionText[3].loadFromRenderedText("D - Goes right", textColor)) {
            success = false;
        }
        if(!gOptionText[4].loadFromRenderedText("W - Goes away ", textColor)) {
            success = false;
        }
        if(!gOptionText[5].loadFromRenderedText("S - Comes closer", textColor)) {
            success = false;
        }
        if(!gOptionText[6].loadFromRenderedText("Space - Jumps", textColor)) {
            success = false;
        }
        if(!gOptionText[7].loadFromRenderedText("R(hold) - Shoots", textColor)) {
            success = false;
        }
        if(!gOptionText[8].loadFromRenderedText("Shift(hold) - Defense stance", textColor)) {
            success = false;
        }
        if(!gOptionText[9].loadFromRenderedText("Player 2 :", textColor)) {
            success = false;
        }
        if(!gOptionText[10].loadFromRenderedText("Left arrow - Goes left", textColor)) {
            success = false;
        }
        if(!gOptionText[11].loadFromRenderedText("Right arrow - Goes right", textColor)) {
            success = false;
        }
        if(!gOptionText[12].loadFromRenderedText("Up arrow - Goes away", textColor)) {
            success = false;
        }
        if(!gOptionText[13].loadFromRenderedText("Down arrow - Comes closer", textColor)) {
            success = false;
        }
        if(!gOptionText[14].loadFromRenderedText("Right shift - Jumps", textColor)) {
            success = false;
        }
        if(!gOptionText[15].loadFromRenderedText("Numpad 0(hold) - Defense stance", textColor)) {
            success = false;
        }
        if(!gOptionText[16].loadFromRenderedText("Enter(hold) - Shoots", textColor)) {
            success = false;
        }

        //Exit
        if(!gMenuButtons[EXIT].loadTextureFromText("Exit", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[EXIT].setDimensions(SCREEN_WIDTH*0.59, SCREEN_HEIGHT*0.5, gMenuButtons[EXIT].getWidth(), gMenuButtons[EXIT].getHeight());
            gMenuButtons[EXIT].setCurrentButton(EXIT);
        }

        //Resume
        if(!gMenuButtons[RESUME].loadTextureFromText("Resume", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[RESUME].setDimensions(SCREEN_WIDTH*0.42, SCREEN_HEIGHT*0.16, gMenuButtons[RESUME].getWidth(), gMenuButtons[RESUME].getHeight());
            gMenuButtons[RESUME].setCurrentButton(RESUME);
        }

        //Main menu
        if(!gMenuButtons[MAIN_MENU].loadTextureFromText("Main Menu", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[MAIN_MENU].setDimensions(SCREEN_WIDTH*0.40, SCREEN_HEIGHT*0.77, gMenuButtons[MAIN_MENU].getWidth(), gMenuButtons[MAIN_MENU].getHeight());
            gMenuButtons[MAIN_MENU].setCurrentButton(MAIN_MENU);
        }

        //New game
        if(!gNewGameButton.loadTextureFromText("New Game", textColor)) {
            printf("%s\n", TTF_GetError());
            success = false;
        }
        else {
            gNewGameButton.setDimensions(SCREEN_WIDTH*0.25, SCREEN_HEIGHT*0.5, gNewGameButton.getWidth(), gNewGameButton.getHeight());
            gNewGameButton.setCurrentButton(NEW_GAME);
        }
    }

    if(!gBasketballPole.getTexture().loadFromFile("entity/BasketballPole/BasketballPole.png")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gBasketballPole.setBoardDimensions(0, 0, 120,240);
        gBasketballPole.setDimensionsBelowBoard(0, gBasketballPole.getBoard().y + gBasketballPole.getBoard().h, 40,495);
        gBasketballPole.setRimDimensions(0, 220, 100, 10);
        gBasketballPole.setPosition(SCREEN_WIDTH -
                                    gBasketballPole.getBoard().w -
                                    (gBasketballPole.getTexture().getWidth() - gBasketballPole.getBoard().w)/2 - 70,
                                    SCREEN_HEIGHT - gBasketballPole.getTexture().getHeight());
        gBasketballPole.getBoard().x = gBasketballPole.getX() + (gBasketballPole.getTexture().getWidth() - gBasketballPole.getBoard().w)/2 - gBasketballPole.getBoard().w + 20;
        gBasketballPole.getBelowBoard().x = SCREEN_WIDTH - gBasketballPole.getBelowBoard().w - 70;
        gBasketballPole.getRim().x = gBasketballPole.getBoard().x - gBasketballPole.getRim().w;
    }

    if(!gBall.getTexture().loadFromFile("entity/Basketball/Ball.png") || !gBall.loadSoundEffects()) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gBall.setPosition(100, SCREEN_HEIGHT - gBall.getTexture().getHeight());
        gBall.passThePole(&gBasketballPole);
    }

    if(!troy.setNormalStance("player/Troy/NormalRunning/Run.1.png") || !troy.loadSoundEffects()) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        troy.setTextureRealDimensions(100,290);
//        troy.setInitialPosition((SCREEN_WIDTH - troy.getTexture().getWidth())/2,(SCREEN_HEIGHT - troy.getTextureRealHeight()));
//        troy.setFacingDirection(RIGHT);
        if(troy.setDefenceStance("player/Troy/DefenceStance/DefenceStance.png")) {
            troy.setDefenceScenes();
        }
        troy.setRunningScenes();
        troy.setJumpingScenes();
        troy.setDribblingScenes();
        troy.setStandDribbleScenes();
        troy.setShootingScenes();
        troy.setStealingScenes();
        troy.passTheBall(&gBall);
        troy.setScoreIndicator();
        troy.setScoreIndicatorPosition(0, 90);
        troy.setInitialScore();
    }

    if(!zac.setNormalStance("player/Zac/Jump/Jump_1.png") || !zac.loadSoundEffects()) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        zac.setTextureRealDimensions(100,290);
//        zac.setInitialPosition((SCREEN_WIDTH - (zac.getTexture().getWidth())/3),(SCREEN_HEIGHT - zac.getHeight()));
//        zac.setFacingDirection(LEFT);
        if(zac.setDefenceStance("player/Zac/DefenceStance/Stance.png")) {
            zac.setDefenceScenes();
        }
        zac.setRunningScenes();
        zac.setJumpingScenes();
        zac.setDribblingScenes();
        zac.setStandDribbleScenes();
        zac.setShootingScenes();
        zac.setStealingScenes();
        zac.passTheBall(&gBall);
        zac.setScoreIndicator();
        zac.setScoreIndicatorPosition(troy.getScoreIndicator().x + troy.getScoreIndicator().w, 90);
        zac.setInitialScore();
    }

    return success;
}

void shutdown() {
    gMusicButton.free();
    gCourt.free();
    gBackgroundTexture.free();
    gBasketballPole.free();
    for(int i=0; i<TOTAL_MENU_BUTTONS; i++) {
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
