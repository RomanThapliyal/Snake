#include "snaky.hpp"
#include "Input.hpp"
#include <ctime>
#include <cmath>

using namespace std;

void Snake::setUp()
{
    clock.start();
    gameState = Menu;
    gameSpeed = sc::DEFAULT_GAME_SPEED;
    srand(static_cast<unsigned int>(time(nullptr)));

    magnetState = Waiting;
    magnetX = sc::MAGNET_INACTIVE_POS;
    magnetY = sc::MAGNET_INACTIVE_POS;
    foodX = 0.0f;
    foodY = 0.0f;
}

void Snake::update()
{
    float dt = frameClock.restart().asSeconds();

    if(clock.getElapsedTime().asMilliseconds() >= gameSpeed)
    {
        logic();
        clock.restart();
    }

    magnet();
    magnetEffect(dt);
}

void Snake::logic()
{
    dir = nextDir;
    calcNextHeadPos();

    if(wallCollision() || bodyCollision())
    {
        gameState = End;
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
        grid = !grid;
    if(in.wrapToggle)
        wrap = !wrap;
    if(in.pausePressed)
        pauseUnpause(in, false);
    if(!in.hasDirRequest)
        return;

    if(in.requestedDir == Up && dir != Down)
        nextDir = Up;
    else if(in.requestedDir == Down && dir != Up)
        nextDir = Down;
    else if(in.requestedDir == Right && dir != Left)
        nextDir = Right;
    else if(in.requestedDir == Left && dir != Right)
        nextDir = Left;
}

void Snake::food()
{
    bool invalidFood = true;

    while(invalidFood)
    {
        invalidFood = false;

        foodX = static_cast<float>(rand() % (cols - sc::PLAY_AREA_MARGIN) + sc::PLAY_AREA_MARGIN);
        foodY = static_cast<float>(rand() % (rows - sc::PLAY_AREA_MARGIN) + sc::PLAY_AREA_MARGIN);

        for(int i = 0; i < snakeLen; i++)
        {
            if(static_cast<int>(foodX) == snakeX[i] &&
               static_cast<int>(foodY) == snakeY[i])
            {
                invalidFood = true;
                break;
            }
        }

        if(magnetState == Available &&
           static_cast<int>(foodX) == magnetX &&
           static_cast<int>(foodY) == magnetY)
        {
            invalidFood = true;
        }
    }
}

void Snake::magnet()
{
    const float spawnDelay = sc::MAGNET_SPAWN_DELAY;
    const float powerDuration = sc::MAGNET_ACTIVE_DURATION;
    float elapsed = magnetClock.getElapsedTime().asSeconds();

    if(magnetState == Waiting)
    {
        if(elapsed >= spawnDelay)
        {
            spawnMagnet();
            magnetState = Available;
            magnetClock.restart();
        }

        return;
    }

    if(magnetState == Active && elapsed >= powerDuration)
        deactivateMagnet();
}


void Snake::spawnMagnet()
{
    bool invalidMagnet = true;

    while(invalidMagnet)
    {
        invalidMagnet = false;

        magnetX = rand() % (cols - sc::PLAY_AREA_MARGIN) + sc::PLAY_AREA_MARGIN;
        magnetY = rand() % (rows - sc::PLAY_AREA_MARGIN) + sc::PLAY_AREA_MARGIN;

        for(int i = 0; i < snakeLen; i++)
        {
            if(magnetX == snakeX[i] && magnetY == snakeY[i])
            {
                invalidMagnet = true;
                break;
            }
        }

        if(magnetX == static_cast<int>(foodX) && magnetY == static_cast<int>(foodY))
            invalidMagnet = true;
    }
}

void Snake::activateMagnet()
{
    magnetState = Active;
    magnetX = sc::MAGNET_INACTIVE_POS;
    magnetY = sc::MAGNET_INACTIVE_POS;
    magnetClock.restart();
}

void Snake::deactivateMagnet()
{
    magnetState = Waiting;
    magnetX = sc::MAGNET_INACTIVE_POS;
    magnetY = sc::MAGNET_INACTIVE_POS;
    magnetClock.restart();
}

void Snake::calcNextHeadPos()
{
    nextHeadX = snakeX[0];
    nextHeadY = snakeY[0];

    if(dir == Right)
        nextHeadX++;
    else if(dir == Left)
        nextHeadX--;
    else if(dir == Up)
        nextHeadY--;
    else if(dir == Down)
        nextHeadY++;
}

void Snake::moveSnake()
{
    for(int i = 0; i < snakeLen; i++)
    {
        prevSnakeX[i] = snakeX[i];
        prevSnakeY[i] = snakeY[i];
    }

    for(int i = snakeLen - 1; i > 0; i--)
    {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }

    snakeX[0] = nextHeadX;
    snakeY[0] = nextHeadY;
}

bool Snake::wallCollision()
{
    if(nextHeadX == sc::WALL_EDGE || nextHeadX == cols ||
       nextHeadY == sc::WALL_EDGE || nextHeadY == rows)
    {
        if(wrap)
        {
            if(nextHeadX == sc::WALL_EDGE)
                nextHeadX = cols - sc::WALL_EDGE;
            else if(nextHeadX == cols)
                nextHeadX = sc::PLAY_AREA_MARGIN;

            if(nextHeadY == sc::WALL_EDGE)
                nextHeadY = rows - sc::WALL_EDGE;
            else if(nextHeadY == rows)
                nextHeadY = sc::PLAY_AREA_MARGIN;

            return false;
        }

        return true;
    }

    return false;
}

bool Snake::bodyCollision()
{
    for(int i = 1; i < snakeLen; i++)
    {
        if(nextHeadX == snakeX[i] && nextHeadY == snakeY[i])
            return true;
    }

    return false;
}

void Snake::growSnake()
{
    oldTailX = snakeX[snakeLen - 1];
    oldTailY = snakeY[snakeLen - 1];

    score++;
    snakeLen++;

    if(gameSpeed > sc::MIN_GAME_SPEED)
        gameSpeed -= sc::SPEED_DECREMENT;

    snakeX[snakeLen - 1] = oldTailX;
    snakeY[snakeLen - 1] = oldTailY;
    prevSnakeX[snakeLen - 1] = oldTailX;
    prevSnakeY[snakeLen - 1] = oldTailY;
}

void Snake::checkFood()
{
    float dx = static_cast<float>(snakeX[0]) - foodX;
    float dy = static_cast<float>(snakeY[0]) - foodY;
    const float collectRadius = sc::FOOD_COLLECT_RADIUS;

    if(dx * dx + dy * dy <= collectRadius * collectRadius)
    {
        growSnake();
        food();
    }
}


void Snake::checkMagnet()
{
    if(magnetState == Available &&
       snakeX[0] == magnetX &&
       snakeY[0] == magnetY)
    {
        activateMagnet();
    }
}


void Snake::magnetEffect(float dt)
{
    const float collectRadius = sc::MAGNET_COLLECT_RADIUS_SQUARED;
    const float attractionRange = sc::MAGNET_ATTRACTION_RANGE;
    const float minimumPullSpeed = sc::MAGNET_MIN_PULL_SPEED;
    const float pullStrength = sc::MAGNET_PULL_STRENGTH;
    const float minimumDistance = sc::MAGNET_MIN_DISTANCE;

    if(magnetState != Active) return;

    float dx = static_cast<float>(snakeX[0]) - foodX;
    float dy = static_cast<float>(snakeY[0]) - foodY;
    float distanceSquared = dx * dx + dy * dy;

    if(distanceSquared <= collectRadius * collectRadius)
    {
        growSnake();
        food();
        return;
    }

    float distance = sqrt(distanceSquared);

    if(distance > attractionRange || distance <= minimumDistance)
        return;

    float speed = minimumPullSpeed + (attractionRange - distance) * pullStrength;

    foodX += (dx / distance) * speed * dt;
    foodY += (dy / distance) * speed * dt;
}

void Snake::menu(const InputState& menue)
{
    if(menue.startPressed)
        gameState = GameOn;
    else if(menue.exitPressed)
        gameState = Exit;
}

void Snake::gameOver(const InputState& in)
{
    if(in.restartPressed)
        restart();
    else if(in.exitPressed)
        gameState = Exit;
}

void Snake::restart()
{
    gameSpeed = sc::DEFAULT_GAME_SPEED;
    score = sc::INITIAL_SCORE;
    snakeLen = sc::INITIAL_SNAKE_LENGTH;
    gameState = GameOn;
    dir = Right;
    nextDir = Right;

    magnetState = Waiting;
    magnetX = sc::MAGNET_INACTIVE_POS;
    magnetY = sc::MAGNET_INACTIVE_POS;
    foodX = 0.0f;
    foodY = 0.0f;

    clock.restart();
    frameClock.restart();
    magnetClock.restart();

    for(int i = 0; i < maxSnakeLength; i++)
    {
        snakeX[i] = 0;
        snakeY[i] = 0;
        prevSnakeX[i] = 0;
        prevSnakeY[i] = 0;
    }

    for(int i = 0; i < snakeLen; i++)
    {
        snakeX[i] = 9 - i;
        snakeY[i] = 6;
        prevSnakeX[i] = snakeX[i];
        prevSnakeY[i] = snakeY[i];
    }

    food();
}

void Snake::pauseUnpause(const InputState& pauseInput, bool resume)
{
    if(pauseInput.pausePressed || resume)
    {
        if(gameState == Pause)
        {
            clock.start();
            frameClock.start();
            magnetClock.start();
            gameState = GameOn;
        }
        else
        {
            clock.stop();
            frameClock.stop();
            magnetClock.stop();
            gameState = Pause;
        }
    }
}

int Snake::getScore() const
{
    return score;
}

int Snake::getSnakeLen() const
{
    return snakeLen;
}

float Snake::getCellSize() const
{
    return cellSize;
}

int Snake::getRows() const
{
    return rows;
}

int Snake::getCols() const
{
    return cols;
}

float Snake::getFoodX() const
{
    return foodX;
}

float Snake::getFoodY() const
{
    return foodY;
}

int Snake::getMagnetX() const
{
    return magnetX;
}

int Snake::getMagnetY() const
{
    return magnetY;
}

float Snake::getInterpolation() const
{
    float t = clock.getElapsedTime().asMilliseconds()/static_cast<float>(gameSpeed);
    if(t > 1.0f)
        t = 1.0f;
    return t;
}

Snake::Direction Snake::getDir() const
{
    return dir;
}

Snake::Direction Snake::getNextDir() const
{
    return nextDir;
}

Snake::GameState Snake::getGameState() const
{
    return gameState;
}

bool Snake::getMagnetPowerActive() const
{
    return magnetState == Active;
}

Snake::MagnetState Snake::getMagnetState() const
{
    return magnetState;
}