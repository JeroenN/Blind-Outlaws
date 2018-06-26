#include "wall.h"

wall::wall(const float height, const float width, const float celX, const float celY, const float celSize)
    :block()
{
    block.setSize(sf::Vector2f(height, width));
    int posX = celX*celSize;
    int posY = celY*celSize;
    block.setPosition(sf::Vector2f(posX, posY));
    block.setFillColor(sf::Color(255, 255, 255));
}

