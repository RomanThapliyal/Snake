#include <SFML/Graphics.hpp>
#include "snaky.hpp"
#include <string>
enum class Anchor{TopLeft,TopRight,Center,BottomLeft,BottomRight,none};
enum class ButtonAction{Start,Exit,Restart};
struct Button{
    sf::FloatRect bounds;
    ButtonAction action;
};
class Renderer
{
private:
    sf::Font gamefont;
    std::vector<Button> menuButtons;
public:
    void setUp();
    void draw(sf::RenderWindow &window, const Snake &snake);
    void drawGrid(sf::RenderWindow &window, const Snake &snake);
    void drawSnake(sf::RenderWindow &window,const Snake &snake);
    void drawWalls(sf::RenderWindow &window,const Snake &snake);
    void drawFood(sf::RenderWindow &window,const Snake &snake);
    void drawMagnet(sf::RenderWindow &window,const Snake &snake);
    void drawText(sf::RenderWindow &window,const sf::Font &font, std::string text, int size, Anchor anchor ,sf::Color colour, sf::Vector2f offset);
    void drawButton(sf::RenderWindow &window, std::string label,sf::Color color, Anchor anchor, sf::Vector2f offset, sf::Vector2f size, ButtonAction action);
    std::optional<ButtonAction> getClickedAction(sf::Vector2f clickPos);
    void gameOverScreen(sf::RenderWindow& window,const Snake &snake);
    void menueScreen(sf::RenderWindow& window);
    void pauseScreen(sf::RenderWindow& window,const Snake &snake);
};