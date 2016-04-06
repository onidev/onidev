#ifndef ONIDEV_COLLISION_CONVEXPOLYGON_H_INCLUDED
#define ONIDEV_COLLISION_CONVEXPOLYGON_H_INCLUDED

#include <onidev/collision/shape.h>
#include <onidev/collision/rect.h>
#include <vector>

namespace od
{

/// @brief A convex polygon shape for manage collisions
class ConvexPolygon:
    public Shape
{
    friend class Collision;
    
    std::vector<float> points;
    class Rect box;
    
    float getMinimumTranslationVector(const std::vector<float> & poly, float nx, float ny, float dx, float dy) const;

public:
    ConvexPolygon();
    ConvexPolygon(const float t[], int n, bool centerShape = false);
    ConvexPolygon(const ConvexPolygon & poly);
    ConvexPolygon & operator=(const ConvexPolygon & poly);
    
    bool intersect(float x, float y) const;
    
    bool intersect(const Line & line, float dx, float dy) const {return false;}
    bool intersect(const Rect & rect, float dx, float dy) const;
    bool intersect(const Disk & disk, float dx, float dy) const;
    bool intersect(const Mask & mask, float dx, float dy) const;
    bool intersect(const ConvexPolygon & poly, float dx, float dy) const;
    bool intersect(const Polygon & poly, float dx, float dy) const;
    bool collide(const Shape & s, float dx, float dy) const;
    
    const Rect boundingBox() const;
    
    void updateBoundingBox();
    void center();
    
    void set(const float t[], int n);
    void clear();
    
    void scale(float x, float y);
    void translate(float x, float y);
    void rotate(float angle);
    // rotation de centre O en se basant sur le modèle passé en parametre
    void rotate(const ConvexPolygon & poly, float angle);
    void rotate(const ConvexPolygon & poly, float cos0, float sin0);
    // rotation de centre (wx, wy) en se basant sur le modèle passé en parametre
    void rotate(const ConvexPolygon & poly, float angle, float wx, float wy);
};
}

#endif // ONIDEV_COLLISION_CONVEXPOLYGON_H_INCLUDED
