#include "player.h"

player::player(const float height, const float width, const float posX, const float posY)
    :block()
{
    block.setSize(sf::Vector2f(height, width));
    block.setPosition(sf::Vector2f(posX, posY));
    block.setFillColor(sf::Color(255, 127, 63));
}
