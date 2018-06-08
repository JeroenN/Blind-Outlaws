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
private:
    float mPosX;
    float mPosY;
    float mSpeedX;
    float mSpeedY;
public:
    bullet(const float height, const float width, const float posX, const float posY, const float speedX, const float speedY);

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

    void moveBullet(){
        int posX = block.getPosition().x;
        int posY = block.getPosition().y;
        block.setPosition(sf::Vector2f(posX+mSpeedX, posY+mSpeedY));
    }
    
    float getSpeedX()
    {
        return mSpeedX;
    }
    
    float getSpeedY()
    {
        return mSpeedY;
    }

    void display(sf::RenderWindow &window) noexcept
    {
        window.draw(block);
    }

};
#endif // BULLET
