#ifndef WALL
#define WALL

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>

class wall
{
    sf::RectangleShape block;

public:
    wall(const float height, const float width, const float celX, const float celY);

    void setPlayerPosition(float celX, float celY, const int celSize)
    {
        int posX = celX*celSize;
        int posY = celY*celSize;
        block.setPosition(sf::Vector2f(posX, posY));
    }

    float getHeight()const noexcept {
        return block.getSize().y;
    }

    float getWidth()const noexcept{
        return block.getSize().x;
    }

    float getCelX(const int celSize) const noexcept{
        return block.getPosition().x/celSize ;
    }

    float getPosY(const int celSize)const noexcept{
        return block.getPosition().y/celSize;
    }

    void display(sf::RenderWindow &window)
    {
        window.draw(block);

    }
};
#endif // PLAYER
