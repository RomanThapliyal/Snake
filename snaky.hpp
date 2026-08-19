#ifndef SNAKY_HPP
#define SNAKY_HPP
#include <SFML/Graphics.hpp>
#include <string>
namespace GameColours{
    const sf::Color Red=sf::Color::Red;
    const sf::Color White=sf::Color::White;
    const sf::Color Black=sf::Color::Black;
    const sf::Color Green=sf::Color::Green;
    const sf::Color Yellow=sf::Color::Yellow;
    const sf::Color Magenta=sf::Color::Magenta;
    const sf::Color Blue=sf::Color::Blue;
    const sf::Color Brown = sf::Color(139, 69, 19);
}
struct InputState;
class Snake{
    public:
    bool grid=false;
    int snakeX[100]={9,8,7};
    int snakeY[100]={6,6,6};
    int prevSnakeX[100];
    int prevSnakeY[100];
    enum Direction{Up,Down,Left,Right};
    enum GameState{menu,gameOn,pause,end,exit};
    private:
    const float cellSize=40.f;
    int gameSpeed;
    int score=0;
    int oldTailX,oldTailY,nextHeadX,nextHeadY;
    int foodX,foodY;
    sf::Clock clock;
    const int rows=20, cols=40;
    int snakeLen=3;
    Direction dir=Right;
    Direction nextDir=Right;
    public:
    GameState gameState=gameOn;
    void setUp();
    void update();
    void logic();
    void applyInput(const InputState& in);
    void food();
    void calcNextHeadPos();
    void moveSnake();
    bool wallCollision();
    bool bodyCollison();
    void checkFood();
    void menue(const InputState& menue);
    void gameOver(const InputState& in);
    void restart();
    void pauseUnpause(const InputState& pause);
    int getScore()const;
    int getSnakeLen()const;
    float getCellSize()const;
    int getRows() const;   
    int getCols() const;  
    int getFoodX()const;
    int getFoodY()const;
    float getInterPolation()const;
    Direction getDir()const;
    Direction getNextDir()const;
    GameState getGameState();

};
#endif