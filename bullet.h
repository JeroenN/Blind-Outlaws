#ifndef BULLET
#define BULLET

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
//#include <conio.h>
class bullet
{
    sf::RectangleShape block;

public:
    bullet(const float height, const float width, const float posX, const float posY);

    void setBulletPosition(float posX, float posY) noexcept
    {
        block.setPosition(sf::Vector2f(posX, posY));
    }

    float getHeight()const noexcept {
        return block.getSize().y;
    }

    float getWidth()const noexcept{
        return block.getSize().x;
    }

    float getPosX() const noexcept{
        return block.getPosition().x;
    }

    float getPosY()const noexcept{
        return block.getPosition().y;
    }

    void setSpeed(float speedX, float speedY){
        int posX = block.getPosition().x;
        int posY = block.getPosition().y;
        block.setPosition(sf::Vector2f(posX+speedX, posY+speedY));
    }

    void display(sf::RenderWindow &window) noexcept
    {
        window.draw(block);
    }
};
#endif // BULLET
