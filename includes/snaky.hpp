#ifndef SNAKY_HPP
#define SNAKY_HPP

#include <SFML/Graphics.hpp>
#include <string>

namespace GameColours
{
    const sf::Color Red = sf::Color::Red;
    const sf::Color White = sf::Color::White;
    const sf::Color Black = sf::Color::Black;
    const sf::Color Green = sf::Color::Green;
    const sf::Color Yellow = sf::Color::Yellow;
    const sf::Color Magenta = sf::Color::Magenta;
    const sf::Color Blue = sf::Color::Blue;
    const sf::Color Brown = sf::Color(139, 69, 19);
}

namespace SnakeConfig
{
    // Game settings
    constexpr float DEFAULT_GAME_SPEED = 100.f;
    constexpr float SPEED_DECREMENT = 0.1f;
    constexpr float MIN_GAME_SPEED = 1.0f; 
    constexpr int INITIAL_SNAKE_LENGTH = 3; 
    constexpr int INITIAL_SCORE = 0;

    // Magnet settings
    constexpr int MAGNET_INACTIVE_POS = -1;
    constexpr float MAGNET_SPAWN_DELAY = 10.f;
    constexpr float MAGNET_ACTIVE_DURATION = 20.0f;
    constexpr float MAGNET_ATTRACTION_RANGE = 4.0f;
    constexpr float MAGNET_PULL_STRENGTH = 6.0f;
    constexpr float MAGNET_MIN_PULL_SPEED = 2.0f;
    constexpr float MAGNET_MIN_DISTANCE = 0.0001f;

    // Food settings
    constexpr float FOOD_COLLECT_RADIUS = 0.5f;
    constexpr float MAGNET_COLLECT_RADIUS_SQUARED = 0.25f;

    // Grid settings
    constexpr int WALL_EDGE = 1;
    constexpr int PLAY_AREA_MARGIN = 2; 
}

struct InputState;

namespace sc = SnakeConfig;

class Snake
{
public:
    static const int rows = 20;
    static const int cols = 40;
    static const int maxSnakeLength = (rows - 2) * (cols - 2);
    bool wrap = false;
    bool grid = false;
    int snakeX[maxSnakeLength] = {9, 8, 7};
    int snakeY[maxSnakeLength] = {6, 6, 6};
    int prevSnakeX[maxSnakeLength];
    int prevSnakeY[maxSnakeLength];

    enum Direction { Up, Down, Left, Right };
    enum GameState { Menu, GameOn, Pause, End, Exit };
    enum MagnetState { Waiting, Available, Active };
    GameState gameState = Menu;

private:
    // Game State
    MagnetState magnetState = Waiting;
    Direction dir = Right;
    Direction nextDir = Right;

    // Timing Systems 
    sf::Clock clock;
    sf::Clock frameClock;
    sf::Clock magnetClock;

    // Game Configuration 
    const float cellSize = 40.0f;
    float gameSpeed = sc::DEFAULT_GAME_SPEED;
    int score = sc::INITIAL_SCORE;
    int snakeLen = sc::INITIAL_SNAKE_LENGTH;  

    // Positions 
    int nextHeadX = 0;
    int nextHeadY = 0;
    int oldTailX = 0;
    int oldTailY = 0;
    int magnetX = sc::MAGNET_INACTIVE_POS;
    int magnetY = sc::MAGNET_INACTIVE_POS;
    float foodX = 0.0f;
    float foodY = 0.0f;

    void spawnMagnet();
    void activateMagnet();
    void deactivateMagnet();

    void logic();

    void food();
    void magnet();

    void calcNextHeadPos();
    void moveSnake();
    bool wallCollision();
    bool bodyCollision();

    void growSnake();
    void checkFood();
    void checkMagnet();

    void magnetEffect(float dt);

public:
    void setUp();
    void update();
    void applyInput(const InputState& in);

    void menu(const InputState& menu);
    void gameOver(const InputState& in);
    void restart();
    void pauseUnpause(const InputState& pause, bool resume);

    int getScore() const;
    int getSnakeLen() const;
    float getCellSize() const;
    int getRows() const;
    int getCols() const;
    float getFoodX() const;
    float getFoodY() const;
    int getMagnetX() const;
    int getMagnetY() const;
    float getInterpolation() const;
    Direction getDir() const;
    Direction getNextDir() const;
    GameState getGameState() const;
    bool getMagnetPowerActive() const;
    MagnetState getMagnetState() const;
};

#endif