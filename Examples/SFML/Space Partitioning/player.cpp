#include "player.h"
#include "block.h"
#include <SFML/Graphics.hpp>

Player::Player(float x, float y):
    od::AutoIndex<sfObject, Player>(x, y)
{
}

od::Shape * Player::collisionMask(int p) const {
    static od::Rect rect(0, 0, 16, 16);
    return &rect;
}

void Player::step(float dt)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !placeMeeting<Block>(pos.x-dt, pos.y)) pos.x -= dt;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)&& !placeMeeting<Block>(pos.x+dt, pos.y)) pos.x += dt;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)   && !placeMeeting<Block>(pos.x, pos.y-dt)) pos.y -= dt;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !placeMeeting<Block>(pos.x, pos.y+dt)) pos.y += dt;
}

void Player::draw(sf::RenderTarget& render_target) const
{
    sf::RectangleShape rectangle;
    rectangle.setPosition(sf::Vector2f(pos.x, pos.y));
    rectangle.setSize(sf::Vector2f(16, 16));
    rectangle.setFillColor(sf::Color(0, 128, 0));
    render_target.draw(rectangle);
}
