#ifndef SNAKY_HPP
#define SNAKY_HPP

#include <SFML/Graphics.hpp>
#include <string>

namespace GameColours
{
const sf::Color Red=sf::Color::Red;
const sf::Color White=sf::Color::White;
const sf::Color Black=sf::Color::Black;
const sf::Color Green=sf::Color::Green;
const sf::Color Yellow=sf::Color::Yellow;
const sf::Color Magenta=sf::Color::Magenta;
const sf::Color Blue=sf::Color::Blue;
const sf::Color Brown=sf::Color(139,69,19);
}

struct InputState;

class Snake
{
public:
    static const int rows=20;
    static const int cols=40;
    static const int maxSnakeLength=(rows-2)*(cols-2);
    bool wrap=false;
    bool grid=false;
    int snakeX[maxSnakeLength]={9,8,7};
    int snakeY[maxSnakeLength]={6,6,6};
    int prevSnakeX[maxSnakeLength];
    int prevSnakeY[maxSnakeLength];

    enum Direction{Up,Down,Left,Right};
    enum GameState{menu,gameOn,pause,end,exit};
    enum MagnetState{Waiting,Available,Active};

private:
    MagnetState magnetState=Waiting;
    sf::Clock clock;
    sf::Clock frameClock;
    sf::Clock magnetClock;
    const float cellSize=40.0f;
    float gameSpeed=100;
    int score=0;
    int oldTailX=0;
    int oldTailY=0;
    int nextHeadX=0;
    int nextHeadY=0;
    int magnetX=1000;
    int magnetY=1000;
    float foodX=0.0f;
    float foodY=0.0f;
    int snakeLen=3;
    Direction dir=Right;
    Direction nextDir=Right;

    void spawnMagnet();
    void activateMagnet();
    void deactivateMagnet();

public:
    GameState gameState=menu;

    void setUp();
    void update();
    void logic();
    void applyInput(const InputState& in);
    void food();
    void magnet();
    void calcNextHeadPos();
    void moveSnake();
    bool wallCollision();
    bool bodyCollison();
    void growSnake();
    void checkFood();
    void checkMagnet();
    void magnetEffect(float dt);
    void menue(const InputState& menue);
    void gameOver(const InputState& in);
    void restart();
    void pauseUnpause(const InputState& pause,bool resume);

    int getScore()const;
    int getSnakeLen()const;
    float getCellSize()const;
    int getRows()const;
    int getCols()const;
    float getFoodX()const;
    float getFoodY()const;
    int getMagnetX()const;
    int getMagnetY()const;
    float getInterPolation()const;
    Direction getDir()const;
    Direction getNextDir()const;
    GameState getGameState()const;
    bool getMagnetPowerActive()const;
    MagnetState getMagnetState()const;
};

#endif