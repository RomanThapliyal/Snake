#include "snaky.hpp"
#include "Input.hpp"
#include <ctime>
#include <iostream>
using namespace std;

void Snake::setUp()
{
    gameState=menu;
    gameSpeed = 90;
    srand(time(0));
    food();
}
void Snake::update()
{
    if (clock.getElapsedTime().asMilliseconds() >= gameSpeed)
    {
        logic();
        clock.restart();
    }
}
void Snake::logic()
{
    dir = nextDir;
    calcNextHeadPos();
    if (wallCollision() || bodyCollison())
    {
        gameState = end;
        return;
    }
    moveSnake();
    checkFood();
}
void Snake::applyInput(const InputState& in)
{
    if(in.gridToggle){std::cout<<"Toggle\n";if(grid==true){grid=false;}else{grid=true;}}
    if(in.pausePressed){pauseUnpause(in);}
    if (!in.hasDirRequest) return; 
    if (in.requestedDir == Up && dir != Down){nextDir = Up;}
    else if (in.requestedDir == Down && dir != Up){nextDir = Down;}
    else if (in.requestedDir == Right && dir != Left){nextDir = Right;}
    else if (in.requestedDir == Left && dir != Right){nextDir = Left;}
}
void Snake::food()
{
    bool invalidFood = true;

    while (invalidFood)
    {
        invalidFood = false;

        foodX = rand() % cols + 1;
        foodY = rand() % rows + 1;

        for (int i = 0; i < snakeLen; i++)
        {
            if (foodX == snakeX[i] && foodY == snakeY[i])
            {
                invalidFood = true;
            }
        }
        if (foodX == 1 || foodX == cols || foodY == 1 || foodY == rows)
        {
            invalidFood = true;
        }
    }
}
void Snake::calcNextHeadPos()
{
    nextHeadX = snakeX[0], nextHeadY = snakeY[0];
    if (dir == Right)
    {
        nextHeadX++;
    }
    else if (dir == Left)
    {
        nextHeadX--;
    }
    else if (dir == Up)
    {
        nextHeadY--;
    }
    else if (dir == Down)
    {
        nextHeadY++;
    }
}
void Snake::moveSnake()
{ 
    for(int i=0;i<snakeLen;i++){
        prevSnakeX[i]=snakeX[i];
        prevSnakeY[i]=snakeY[i];
    }
    for (int i = snakeLen - 1; i > 0; i--)
    {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    snakeX[0] = nextHeadX;
    snakeY[0] = nextHeadY;
}
bool Snake::wallCollision()
{
    if (nextHeadX == 1 || nextHeadX == cols || nextHeadY == 1 || nextHeadY == rows)
    {
        return true;
    }
    return false;
}
bool Snake::bodyCollison()
{
    for (int i = 1; i < snakeLen; i++)
    {
        if (nextHeadX == snakeX[i] && nextHeadY == snakeY[i])
        {
            return true;
        }
    }
    return false;
}
void Snake::checkFood()
{
    oldTailX = snakeX[snakeLen - 1], oldTailY = snakeY[snakeLen - 1];
    if (snakeX[0] == foodX && snakeY[0] == foodY)
    {
        food();
        score++;
        snakeLen++;
        gameSpeed-=1;
        snakeX[snakeLen - 1] = oldTailX;
        snakeY[snakeLen - 1] = oldTailY;
        prevSnakeX[snakeLen-1]=oldTailX;
        prevSnakeY[snakeLen-1]=oldTailY;
    }
}
void Snake::menue(const InputState& menue){
    if(menue.startPressed)
       gameState=gameOn;
    else if(menue.exitPressed)
       gameState=exit;
}
void Snake::gameOver(const InputState& in)
{
    if (in.restartPressed)
        restart();
    else if (in.exitPressed)
        gameState = exit; 
}
void Snake::restart()
{
    food();
    gameSpeed=90;
    cout << "restart\n";
    gameState = gameOn;
    dir = Right;
    nextDir = Right;
    score = 0;
    for (int i = 0; i < 3; i++)
    {
        snakeX[i] = 9 - i;
        snakeY[i] = 6;
    }
    for (int i = 3; i < snakeLen; i++)
    {
        snakeX[i] = 0;
        snakeY[i] = 0;
    }
    snakeLen = 3;
}
void Snake::pauseUnpause(const InputState& Pause){
    if(Pause.pausePressed){
        if(gameState==pause){gameState=gameOn;}
        else gameState=pause;
    }
}
int Snake::getScore()const{return score;}
int Snake::getSnakeLen()const{return snakeLen;}
float Snake::getCellSize()const{return cellSize;}
int Snake::getRows() const { return rows; }
int Snake::getCols() const { return cols; }
Snake::Direction Snake::getDir()const{return dir;}
Snake::Direction Snake::getNextDir()const{return nextDir;}
Snake::GameState Snake::getGameState(){return gameState;}
int Snake::getFoodX()const{return foodX;}
int Snake::getFoodY()const{return foodY;}
float Snake::getInterPolation()const{
    float t= clock.getElapsedTime().asMilliseconds()/(float)gameSpeed;
    if(t>1.f) t=1.f;
    return t;
}