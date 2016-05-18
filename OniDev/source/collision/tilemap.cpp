// OniDev - Copyright (c) 2013-2016 Boris Marmontel
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.

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
