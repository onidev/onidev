#ifndef SFML_OBJECT_H_INCLUDED
#define SFML_OBJECT_H_INCLUDED

#include <onidev/game.h>

namespace sf {
    class RenderTarget;
}

class sfObject:
    public od::GenericObject2d<sfObject>
{
    template<class U, class V, int P>
    friend class od::AutoIndex;
    static size_t GlobalCounter;
    
public:
    sfObject(float x, float y):
        GenericObject2d<sfObject>(x, y) {}
    
    virtual void draw(sf::RenderTarget& render_target) const {}
};

class sfCollisionGrid:
    public od::GenericCollisionGrid2d<sfObject>
{
public:
    sfCollisionGrid(size_t cell_wid, size_t cell_hei, size_t cols, size_t rows):
        od::GenericCollisionGrid2d<sfObject>(cell_wid, cell_hei, cols, rows) {}
};

#endif
