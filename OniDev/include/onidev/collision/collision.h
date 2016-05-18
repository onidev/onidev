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

#ifndef ONIDEV_COLLISION_COLLISION_H_INCLUDED
#define ONIDEV_COLLISION_COLLISION_H_INCLUDED

#include <onidev/collision/line.h>
#include <onidev/collision/rect.h>
#include <onidev/collision/disk.h>
#include <onidev/collision/mask.h>
#include <onidev/collision/convex_polygon.h>
#include <onidev/collision/polygon.h>

/*
Shapes manquantes:
Multishape
Ray
OBB
Ellipse
OrientedEllipse
*/

namespace od
{
class Collision
{
public:
    static bool intersect(float x, float y, const Rect & box);
    static bool intersect(const Rect & a, const Rect & b, float dx=0, float dy=0);
    static bool intersect(const Rect & a, const Line & b, float dx=0, float dy=0);
    static bool intersect(const Rect & a, const Disk & b, float dx=0, float dy=0);
    static bool intersect(const Rect & a, const Mask & b, float dx=0, float dy=0);
    static bool intersect(const Rect & a, const ConvexPolygon & b, float dx=0, float dy=0);
    static bool intersect(const Rect & a, const Polygon & b, float dx=0, float dy=0);
    
    static bool intersect(const Line & a, const Line & b, float dx=0, float dy=0);
    static bool intersect(const Line & a, const Disk & b, float dx=0, float dy=0);
    /// @todo Implement this function
    static bool intersect(const Line & a, const Mask & b, float dx=0, float dy=0);
    /// @todo Implement this function
    static bool intersect(const Line & a, const ConvexPolygon & b, float dx=0, float dy=0);
    /// @todo Implement this function
    static bool intersect(const Line & a, const Polygon & b, float dx=0, float dy=0);
    
    static bool intersect(const Disk & a, const Disk & b, float dx=0, float dy=0);
    /// @todo Implement this function
    static bool intersect(const Disk & a, const Mask & b, float dx=0, float dy=0);
    /// @todo Implement this function
    static bool intersect(const Disk & a, const ConvexPolygon & b, float dx=0, float dy=0);
    /// @todo Implement this function
    static bool intersect(const Disk & a, const Polygon & b, float dx=0, float dy=0);
    
    static bool intersect(const Mask & a, const Mask & b, float dx=0, float dy=0);
    /// @todo Implement this function
    static bool intersect(const Mask & a, const ConvexPolygon & b, float dx=0, float dy=0);
    /// @todo Implement this function
    static bool intersect(const Mask & a, const Polygon & b, float dx=0, float dy=0);
    
    static bool intersect(const ConvexPolygon & a, const ConvexPolygon & b, float dx=0, float dy=0);
    static bool intersect(const ConvexPolygon & a, const Polygon & b, float dx=0, float dy=0);
    
    static bool intersect(const Polygon & a, const Polygon & b, float dx=0, float dy=0);
};
}


#endif // ONIDEV_COLLISION_COLLISION_H_INCLUDED
