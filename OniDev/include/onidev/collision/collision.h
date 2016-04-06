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
