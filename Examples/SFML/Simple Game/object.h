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

#endif
