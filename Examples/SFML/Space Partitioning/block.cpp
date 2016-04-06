#include "block.h"
#include <SFML/Graphics.hpp>

Block::Block(float x, float y):
    od::AutoIndex<sfObject, Block>(x, y)
{
}

od::Shape * Block::collisionMask(int p) const {
    static od::Rect rect(0, 0, 16, 16);
    return &rect;
}

void Block::draw(sf::RenderTarget& render_target) const
{
    sf::RectangleShape rectangle;
    rectangle.setPosition(sf::Vector2f(pos.x, pos.y));
    rectangle.setSize(sf::Vector2f(16, 16));
    rectangle.setFillColor(sf::Color(255, 255, 255));
    render_target.draw(rectangle);
}
