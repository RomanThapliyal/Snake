#include "Renderer.hpp"
#include <iostream>
void Renderer::setUp(){
    if (!gamefont.openFromFile("Assets/Fonts/PressStart2P-Regular.ttf"))
    std::cout << "failed to load font\n";
}
void Renderer::draw(sf::RenderWindow &window,const Snake& snake)
{
    window.clear(GameColours::Black);
    if(snake.grid==true)
        drawGrid(window,snake);
    drawSnake(window,snake);
    drawWalls(window,snake);
    drawFood(window,snake);
    drawText(window, gamefont, "Score:" + std::to_string(snake.getScore()), 34,Anchor::TopRight, GameColours::White,{-50.f,-50.f});
    if(snake.wrap)
    drawText(window, gamefont, "Wrap", 14 ,Anchor::TopRight, GameColours::White,{-70.f,-100.f});
    if(snake.grid)
    drawText(window, gamefont, "Grid", 14 ,Anchor::TopRight, GameColours::White,{-70.f,-140.f});
}
void Renderer::drawGrid(sf::RenderWindow &window, const Snake &snake){
    for(int i=1;i<snake.getRows();i++){
        for(int j=1;j<snake.getCols();j++){
            sf::RectangleShape cellGrid({snake.getCellSize(),snake.getCellSize()});
            cellGrid.setFillColor(sf::Color::Black);
            cellGrid.setOutlineColor(sf::Color(255, 255, 255, 90));
            cellGrid.setOutlineThickness(1.f);
            cellGrid.setPosition({(j - 1) * snake.getCellSize(), (i - 1) * snake.getCellSize()});
            window.draw(cellGrid);
        }
    }
}
void Renderer::drawSnake(sf::RenderWindow &window,const Snake& snake)
{
    for (int i = 0; i < snake.getSnakeLen(); i++)
    {
        sf::RectangleShape segment({snake.getCellSize(),snake.getCellSize()});

        if(i==0)
            segment.setFillColor(GameColours::Green);
        else
            segment.setFillColor(sf::Color(0,180,0));

        float t=snake.getInterPolation();

        float renderX=snake.prevSnakeX[i]+(snake.snakeX[i]-snake.prevSnakeX[i])*t;
        float renderY=snake.prevSnakeY[i]+(snake.snakeY[i]-snake.prevSnakeY[i])*t;

        bool wrapX=std::abs(snake.snakeX[i]-snake.prevSnakeX[i])>1;
        bool wrapY=std::abs(snake.snakeY[i]-snake.prevSnakeY[i])>1;

        if(wrapX)
        {
            if(snake.prevSnakeX[i]==2)
                renderX=2-t;
            else
                renderX=snake.getCols()-1+t;
        }

        if(wrapY)
        {
            if(snake.prevSnakeY[i]==2)
                renderY=2-t;
            else
                renderY=snake.getRows()-1+t;
        }

        if(snake.gameState==snake.end){
            renderX=snake.snakeX[i];
            renderY=snake.snakeY[i];
        }

        float x=(renderX-1)*snake.getCellSize();
        float y=(renderY-1)*snake.getCellSize();

        segment.setPosition({x,y});
        window.draw(segment);

        if(wrapX)
        {
            float oppositeX;

            if(renderX<2)
                oppositeX=renderX+(snake.getCols()-2);
            else
                oppositeX=renderX-(snake.getCols()-2);

            segment.setPosition({(oppositeX-1)*snake.getCellSize(),y});
            window.draw(segment);
        }

        if(wrapY)
        {
            float oppositeY;

            if(renderY<2)
                oppositeY=renderY+(snake.getRows()-2);
            else
                oppositeY=renderY-(snake.getRows()-2);

            segment.setPosition({x,(oppositeY-1)*snake.getCellSize()});
            window.draw(segment);
        }
        if (i == 0)
{
    float cell = snake.getCellSize();

    sf::CircleShape eye(cell * 0.15f);
    eye.setFillColor(sf::Color::Black);

    sf::CircleShape pupil(cell * 0.06f);
    pupil.setFillColor(sf::Color::White);

    if (snake.getDir() == snake.Right)
    {
        eye.setPosition({x + cell * 0.60f, y + cell * 0.15f});
        pupil.setPosition({x + cell * 0.65f, y + cell * 0.20f});
        window.draw(eye);
        window.draw(pupil);

        eye.setPosition({x + cell * 0.60f, y + cell * 0.60f});
        pupil.setPosition({x + cell * 0.65f, y + cell * 0.65f});
        window.draw(eye);
        window.draw(pupil);
    }
    else if (snake.getDir() == snake.Left)
    {
        eye.setPosition({x + cell * 0.15f, y + cell * 0.15f});
        pupil.setPosition({x + cell * 0.20f, y + cell * 0.20f});
        window.draw(eye);
        window.draw(pupil);

        eye.setPosition({x + cell * 0.15f, y + cell * 0.60f});
        pupil.setPosition({x + cell * 0.20f, y + cell * 0.65f});
        window.draw(eye);
        window.draw(pupil);
    }
    else if (snake.getDir() == snake.Up)
    {
        eye.setPosition({x + cell * 0.15f, y + cell * 0.15f});
        pupil.setPosition({x + cell * 0.20f, y + cell * 0.20f});
        window.draw(eye);
        window.draw(pupil);

        eye.setPosition({x + cell * 0.60f, y + cell * 0.15f});
        pupil.setPosition({x + cell * 0.65f, y + cell * 0.20f});
        window.draw(eye);
        window.draw(pupil);
    }
    else if (snake.getDir() == snake.Down)
    {
        eye.setPosition({x + cell * 0.15f, y + cell * 0.60f});
        pupil.setPosition({x + cell * 0.20f, y + cell * 0.65f});
        window.draw(eye);
        window.draw(pupil);

        eye.setPosition({x + cell * 0.60f, y + cell * 0.60f});
        pupil.setPosition({x + cell * 0.65f, y + cell * 0.65f});
        window.draw(eye);
        window.draw(pupil);
    }
}
    }
}
void Renderer::drawWalls(sf::RenderWindow &window,const Snake& snake)
{
    for (int i = 1; i <= snake.getRows(); i++)
    {
        for (int j = 1; j <= snake.getCols(); j++)
        {
            if (i == 1 || i == snake.getRows() || j == 1 || j == snake.getCols())
            {
                sf::RectangleShape wall({snake.getCellSize(), snake.getCellSize()});
                wall.setFillColor(GameColours::Brown);
                wall.setPosition({(j - 1) * snake.getCellSize(), (i - 1) * snake.getCellSize()});
                window.draw(wall);
            }
        }
    }
}
void Renderer::drawFood(sf::RenderWindow &window,const Snake& snake)
{
    float cell = snake.getCellSize();
    float centerX = (snake.getFoodX() - 1) * cell + cell/2;
    float centerY = (snake.getFoodY() - 1) * cell + cell/2;

    float glowRadius = cell * 0.7f;
    sf::CircleShape glow(glowRadius);
    glow.setFillColor(sf::Color(255,0,0,70));
    glow.setPosition({centerX-glowRadius,centerY-glowRadius});
    window.draw(glow);

    float fruitRadius = cell * 0.4f;
    sf::CircleShape fruit(fruitRadius);
    fruit.setFillColor(sf::Color::Red);
    fruit.setPosition({centerX-fruitRadius,centerY-fruitRadius});
    window.draw(fruit);
}
void Renderer::drawText(sf::RenderWindow &window,const sf::Font &font,std::string text,int size,Anchor anchor,sf::Color colour,sf::Vector2f offset)
{
    float x,y;
    sf::Text Text(font);
    Text.setString(text);
    Text.setCharacterSize(size);
    Text.setFillColor(colour);
    sf::Vector2u windowSize=window.getSize();
    sf::FloatRect bounds=Text.getLocalBounds();
    if(anchor==Anchor::TopLeft){x=offset.x;y=-offset.y;}
    else if(anchor==Anchor::TopRight){x=windowSize.x-bounds.size.x+offset.x;y=-offset.y;}
    else if(anchor==Anchor::Center){x=(windowSize.x-bounds.size.x)/2+offset.x;y=(windowSize.y-bounds.size.y)/2-offset.y;}
    else if(anchor==Anchor::BottomLeft){x=offset.x;y=windowSize.y-bounds.size.y-offset.y;}
    else if(anchor==Anchor::BottomRight){x=windowSize.x-bounds.size.x+offset.x;y=windowSize.y-bounds.size.y-offset.y;}
    Text.setPosition({x,y});
    window.draw(Text);
}
void Renderer::drawButton(sf::RenderWindow &window, std::string label,sf::Color color, Anchor anchor, sf::Vector2f offset, sf::Vector2f size, ButtonAction action)
{
    sf::Vector2u windowSize=window.getSize();
    float x,y;
    if (anchor == Anchor::TopLeft)      { x = offset.x; y = -offset.y; }
    else if (anchor == Anchor::TopRight){ x = windowSize.x - size.x + offset.x; y = -offset.y; }
    else if (anchor == Anchor::Center)  { x = (windowSize.x - size.x) / 2 + offset.x; y = (windowSize.y - size.y) / 2 -offset.y; }
    else if (anchor == Anchor::BottomLeft) { x = offset.x; y = windowSize.y - size.y - offset.y; }
    else if (anchor == Anchor::BottomRight){ x = windowSize.x - size.x + offset.x; y = windowSize.y - size.y - offset.y; }

    sf::RectangleShape rect(size);
    rect.setPosition({x,y});
    rect.setFillColor(color);
    rect.setOutlineColor(GameColours::White);
    rect.setOutlineThickness(2.f);
    window.draw(rect);

    sf::Text text(gamefont);
    text.setString(label);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition({x + (size.x - textBounds.size.x) / 2.f, y + (size.y - textBounds.size.y) / 2.f});
    window.draw(text);
    menuButtons.push_back({sf::FloatRect({x,y},size),action});
}
std::optional<ButtonAction> Renderer::getClickedAction(sf::Vector2f clickPos)
{ 
    for (const Button& b : menuButtons)
    {
        if (b.bounds.contains(clickPos))
        {
            std::cout<<"Button clicked\n";
            return b.action;
        }
    }
    return std::nullopt;
}
void Renderer::gameOverScreen(sf::RenderWindow& window,const Snake &snake){
    draw(window,snake);
    sf::RectangleShape overlay({static_cast<float>(window.getSize().x),static_cast<float>(window.getSize().y)});
    overlay.setFillColor(sf::Color(0,0,0,150));
    window.draw(overlay);
    drawText(window, gamefont, "Game Over", 50,Anchor::Center,GameColours::Red,{0.f,0.f});
    drawText(window, gamefont, "Score:" + std::to_string(snake.getScore()), 40, Anchor::Center,GameColours::White,{0.f,-100.f});
    drawButton(window,"Restart",GameColours::Magenta,Anchor::BottomLeft,{600.f,190.f},{180.f,50.f},ButtonAction::Start);
    drawButton(window,"Quit",GameColours::Magenta,Anchor::BottomRight,{-600.f,190.f},{180.f,50.f},ButtonAction::Exit);
    window.display();
}
void Renderer::menueScreen(sf::RenderWindow& window){
    window.clear(GameColours::Black);
    drawText(window,gamefont,"Snaky",55,Anchor::Center,GameColours::Green,{0.f,30.f});
    drawText(window,gamefont,"Developer's first 2D snake game made in C++!",20,Anchor::Center,GameColours::Yellow,{0.f,-40.f});
    drawText(window,gamefont,"Press enter to start or X to exit",15,Anchor::Center,GameColours::White,{0.f,-110.f});
    drawButton(window,"Start",GameColours::Magenta,Anchor::BottomLeft,{600.f,190.f},{180.f,50.f},ButtonAction::Start);
    drawButton(window,"Quit",GameColours::Magenta,Anchor::BottomRight,{-600.f,190.f},{180.f,50.f},ButtonAction::Exit);
    drawText(window, gamefont, "WASD / Arrow Keys: Move | G: Toggle Grid | P: Pause | T: Toggle Wrap", 9 ,Anchor::BottomLeft, GameColours::White,{50.f,30.f});
    window.display();
}
void Renderer::pauseScreen(sf::RenderWindow& window, const Snake &snake){
    draw(window,snake);
    sf::RectangleShape overlay({static_cast<float>(window.getSize().x),static_cast<float>(window.getSize().y)});
    overlay.setFillColor(sf::Color(0,0,0,150));
    window.draw(overlay);

    drawText(window,gamefont,"Paused",60,Anchor::Center,GameColours::Yellow,{0.f,30.f});
    drawButton(window,"Resume",GameColours::Magenta,Anchor::BottomLeft,{600.f,190.f},{180.f,50.f},ButtonAction::Start);
    drawButton(window,"Quit",GameColours::Magenta,Anchor::BottomRight,{-600.f,190.f},{180.f,50.f},ButtonAction::Exit);

    window.display();
}