#include "snaky.hpp"
#include "Input.hpp"
#include <ctime>
#include <cmath>

using namespace std;

void Snake::setUp()
{
    clock.start();
    gameState=menu;
    gameSpeed=100;
    srand(static_cast<unsigned int>(time(nullptr)));

    magnetState=Waiting;
    magnetX=1000;
    magnetY=1000;
    foodX=0.0f;
    foodY=0.0f;

    magnetClock.restart();
    frameClock.restart();
    food();
}

void Snake::update()
{
    float dt=frameClock.restart().asSeconds();

    if(clock.getElapsedTime().asMilliseconds()>=gameSpeed)
    {
        logic();
        clock.restart();
    }

    magnet();
    magnetEffect(dt);
}

void Snake::logic()
{
    dir=nextDir;
    calcNextHeadPos();

    if(wallCollision()||bodyCollison())
    {
        gameState=end;
        clock.stop();
        return;
    }

    moveSnake();
    checkMagnet();
    checkFood();
}

void Snake::applyInput(const InputState& in)
{
    if(in.gridToggle)
        grid=!grid;

    if(in.wrapToggle)
        wrap=!wrap;

    if(in.pausePressed)
        pauseUnpause(in,false);

    if(!in.hasDirRequest)
        return;

    if(in.requestedDir==Up&&dir!=Down)
        nextDir=Up;
    else if(in.requestedDir==Down&&dir!=Up)
        nextDir=Down;
    else if(in.requestedDir==Right&&dir!=Left)
        nextDir=Right;
    else if(in.requestedDir==Left&&dir!=Right)
        nextDir=Left;
}

void Snake::food()
{
    bool invalidFood=true;

    while(invalidFood)
    {
        invalidFood=false;

        foodX=static_cast<float>(rand()%(cols-2)+2);
        foodY=static_cast<float>(rand()%(rows-2)+2);

        for(int i=0;i<snakeLen;i++)
        {
            if(static_cast<int>(foodX)==snakeX[i]&&
               static_cast<int>(foodY)==snakeY[i])
            {
                invalidFood=true;
                break;
            }
        }

        if(magnetState==Available&&
           static_cast<int>(foodX)==magnetX&&
           static_cast<int>(foodY)==magnetY)
        {
            invalidFood=true;
        }
    }
}

void Snake::magnet()
{
    const float spawnDelay=10.0f;
    const float powerDuration=20.0f;
    float elapsed=magnetClock.getElapsedTime().asSeconds();

    if(magnetState==Waiting)
    {
        if(elapsed>=spawnDelay)
        {
            spawnMagnet();
            magnetState=Available;
            magnetClock.restart();
        }

        return;
    }

    if(magnetState==Active&&elapsed>=powerDuration)
        deactivateMagnet();
}

void Snake::spawnMagnet()
{
    bool invalidMagnet=true;

    while(invalidMagnet)
    {
        invalidMagnet=false;

        magnetX=rand()%(cols-2)+2;
        magnetY=rand()%(rows-2)+2;

        for(int i=0;i<snakeLen;i++)
        {
            if(magnetX==snakeX[i]&&magnetY==snakeY[i])
            {
                invalidMagnet=true;
                break;
            }
        }

        if(magnetX==foodX&&magnetY==foodY)
            invalidMagnet=true;
    }
}

void Snake::activateMagnet()
{
    magnetState=Active;
    magnetX=1000;
    magnetY=1000;
    magnetClock.restart();
}

void Snake::deactivateMagnet()
{
    magnetState=Waiting;
    magnetX=1000;
    magnetY=1000;
    magnetClock.restart();
}

void Snake::calcNextHeadPos()
{
    nextHeadX=snakeX[0];
    nextHeadY=snakeY[0];

    if(dir==Right)
        nextHeadX++;
    else if(dir==Left)
        nextHeadX--;
    else if(dir==Up)
        nextHeadY--;
    else if(dir==Down)
        nextHeadY++;
}

void Snake::moveSnake()
{
    for(int i=0;i<snakeLen;i++)
    {
        prevSnakeX[i]=snakeX[i];
        prevSnakeY[i]=snakeY[i];
    }

    for(int i=snakeLen-1;i>0;i--)
    {
        snakeX[i]=snakeX[i-1];
        snakeY[i]=snakeY[i-1];
    }

    snakeX[0]=nextHeadX;
    snakeY[0]=nextHeadY;
}

bool Snake::wallCollision()
{
    if(nextHeadX==1||nextHeadX==cols||
       nextHeadY==1||nextHeadY==rows)
    {
        if(wrap)
        {
            if(nextHeadX==1)
                nextHeadX=cols-1;
            else if(nextHeadX==cols)
                nextHeadX=2;

            if(nextHeadY==1)
                nextHeadY=rows-1;
            else if(nextHeadY==rows)
                nextHeadY=2;

            return false;
        }

        return true;
    }

    return false;
}

bool Snake::bodyCollison()
{
    for(int i=1;i<snakeLen;i++)
    {
        if(nextHeadX==snakeX[i]&&nextHeadY==snakeY[i])
            return true;
    }

    return false;
}

void Snake::growSnake()
{
    oldTailX=snakeX[snakeLen-1];
    oldTailY=snakeY[snakeLen-1];

    score++;
    snakeLen++;

    if(gameSpeed>1)
        gameSpeed-=0.1;

    snakeX[snakeLen-1]=oldTailX;
    snakeY[snakeLen-1]=oldTailY;
    prevSnakeX[snakeLen-1]=oldTailX;
    prevSnakeY[snakeLen-1]=oldTailY;
}

void Snake::checkFood()
{
    float dx=static_cast<float>(snakeX[0])-foodX;
    float dy=static_cast<float>(snakeY[0])-foodY;
    const float collectRadius=0.5f;

    if(dx*dx+dy*dy<=collectRadius*collectRadius)
    {
        growSnake();
        food();
    }
}

void Snake::checkMagnet()
{
    if(magnetState==Available&&
       snakeX[0]==magnetX&&
       snakeY[0]==magnetY)
    {
        activateMagnet();
    }
}

void Snake::magnetEffect(float dt)
{
    float collectRadius=0.25f;
    float attractionRange=4.0f;
    float minimumPullSpeed=2.0f;
    float pullStrength=6.0f;
    float minimumDistance=0.0001f;

    if(magnetState!=Active)
        return;

    float dx=static_cast<float>(snakeX[0])-foodX;
    float dy=static_cast<float>(snakeY[0])-foodY;
    float distanceSquared=dx*dx+dy*dy;

    if(distanceSquared<=collectRadius*collectRadius)
    {
        growSnake();
        food();
        return;
    }

    float distance=sqrt(distanceSquared);

    if(distance>attractionRange||distance<=minimumDistance)
        return;

    float speed=minimumPullSpeed+(attractionRange-distance)*pullStrength;

    foodX+=(dx/distance)*speed*dt;
    foodY+=(dy/distance)*speed*dt;
}

void Snake::menue(const InputState& menue)
{
    if(menue.startPressed)
        gameState=gameOn;
    else if(menue.exitPressed)
        gameState=exit;
}

void Snake::gameOver(const InputState& in)
{
    if(in.restartPressed)
        restart();
    else if(in.exitPressed)
        gameState=exit;
}

void Snake::restart()
{
    gameSpeed=100;
    score=0;
    snakeLen=3;
    gameState=gameOn;
    dir=Right;
    nextDir=Right;

    magnetState=Waiting;
    magnetX=1000;
    magnetY=1000;
    foodX=0.0f;
    foodY=0.0f;

    clock.restart();
    frameClock.restart();
    magnetClock.restart();

    for(int i=0;i<maxSnakeLength;i++)
    {
        snakeX[i]=0;
        snakeY[i]=0;
        prevSnakeX[i]=0;
        prevSnakeY[i]=0;
    }

    for(int i=0;i<snakeLen;i++)
    {
        snakeX[i]=9-i;
        snakeY[i]=6;
        prevSnakeX[i]=snakeX[i];
        prevSnakeY[i]=snakeY[i];
    }

    food();
}

void Snake::pauseUnpause(const InputState& pauseInput,bool resume)
{
    if(pauseInput.pausePressed||resume)
    {
        if(gameState==pause)
        {
            clock.start();
            frameClock.start();
            magnetClock.start();
            gameState=gameOn;
        }
        else
        {
            clock.stop();
            frameClock.stop();
            magnetClock.stop();
            gameState=pause;
        }
    }
}

int Snake::getScore()const
{
    return score;
}

int Snake::getSnakeLen()const
{
    return snakeLen;
}

float Snake::getCellSize()const
{
    return cellSize;
}

int Snake::getRows()const
{
    return rows;
}

int Snake::getCols()const
{
    return cols;
}

float Snake::getFoodX()const
{
    return foodX;
}

float Snake::getFoodY()const
{
    return foodY;
}

int Snake::getMagnetX()const
{
    return magnetX;
}

int Snake::getMagnetY()const
{
    return magnetY;
}

float Snake::getInterPolation()const
{
    float t=clock.getElapsedTime().asMilliseconds()/
            static_cast<float>(gameSpeed);

    if(t>1.0f)
        t=1.0f;

    return t;
}

Snake::Direction Snake::getDir()const
{
    return dir;
}

Snake::Direction Snake::getNextDir()const
{
    return nextDir;
}

Snake::GameState Snake::getGameState()const
{
    return gameState;
}

bool Snake::getMagnetPowerActive()const
{
    return magnetState==Active;
}

Snake::MagnetState Snake::getMagnetState()const
{
    return magnetState;
}