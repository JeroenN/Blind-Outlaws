#ifndef PLAYER
#define PLAYER

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
#include <conio.h>
class player
{
    sf::RectangleShape block;

public:
    player(const float height, const float width, const float posX, const float posY);

    void setPlayerPosition(float posX, float posY)
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

    void display(sf::RenderWindow &window)
    {
        window.draw(block);
    }
};
#endif // PLAYER
