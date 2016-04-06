// Copyright 2013-2015 Boris Marmontel, All Rights Reserved.

/*=============================================================================
	tilemap.cpp
=============================================================================*/

#include <onidev/collision/tilemap.h>
#include <onidev/collision.h>

namespace od
{

TileMap::TileMap()
{
    
}

TileMap::~TileMap()
{
    
}

float TileMap::xoffset() const
{
    return 0.f;
}

float TileMap::yoffset() const
{
    return 0.f;
}

bool TileMap::collision(const od::Shape * shape, float x, float y) const
{
    od::Rect box = shape->boundingBox();
    //od::Rect box = shape->boundingBox().translate(xoffset(), yoffset());
    
    // + ou -? => a verifier
    x -= xoffset();
    y -= yoffset();
    
    int x1 = (x + box.x1    ) / tileWidth();
    int x2 = (x + box.x2 - 1) / tileWidth();
    int y1 = (y + box.y1    ) / tileHeight();
    int y2 = (y + box.y2 - 1) / tileHeight();
    
    if(x1 < 0) x1 = 0;
    if(y1 < 0) y1 = 0;
    if(x2 > width()- 1) x2 = width()-1;
    if(y2 > height()-1) y2 = height()-1;
    
    for(int i=x1; i<=x2; ++i)
    {
        for(int j=y1; j<=y2; ++j)
        {
            const od::Shape * tile = tileCollisionMask(i, j);
            if(tile && tileSolid(i, j) && shape->collide(*tile, i * tileWidth() - x, j * tileHeight() - y))
                return true;
        }
    }
    return false;
}

}
