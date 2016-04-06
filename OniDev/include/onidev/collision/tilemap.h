#ifndef ONIDEV_COLLISION_TILEMAP_H_INCLUDED
#define ONIDEV_COLLISION_TILEMAP_H_INCLUDED

#include <onidev/collision/shape.h>

namespace od
{

class TileMap
{
public:
    TileMap();
    virtual ~TileMap();
    
    virtual bool tileSolid(int x, int y) const = 0;
    virtual od::Shape * tileCollisionMask(int x, int y) const = 0;
    
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual int tileWidth() const = 0;
    virtual int tileHeight() const = 0;
    
    virtual float xoffset() const;
    virtual float yoffset() const;
    
    bool collision(const od::Shape * shape, float dx, float dy) const;
};

}

#endif
