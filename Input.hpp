#ifndef INPUT_HPP
#define INPUT_HPP
#include <SFML/Graphics.hpp>
#include "snaky.hpp"
struct InputState
{
    Snake::Direction requestedDir = Snake::Right;
    bool hasDirRequest   = false;
    bool startPressed    = false;
    bool pausePressed    = false;
    bool restartPressed  = false;
    bool exitPressed     = false;
    bool mouseClicked = false;
    bool gridToggle = false;
    sf::Vector2f clickPosition;
};
class Input
{
public:
    InputState poll(sf::RenderWindow& window);
};
#endif