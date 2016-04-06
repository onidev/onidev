// Copyright 2013-2015 Boris Marmontel, All Rights Reserved.

/*=============================================================================
	rect.cpp
=============================================================================*/

#include <onidev/collision/rect.h>
#include <cstdio>
#include <cmath>
#include <onidev/collision/convex_polygon.h>
#include <onidev/collision/polygon.h>
#include <onidev/collision/mask.h>
#include <onidev/collision/line.h>
#include <onidev/collision/disk.h>

//extern int number;

namespace od
{

Rect::Rect(): x1(0), y1(0), x2(0), y2(0) {}
Rect::Rect(const Rect & rect): x1(rect.x1), y1(rect.y1), x2(rect.x2), y2(rect.y2) {}
Rect::Rect(float x1, float y1, float x2, float y2): x1(x1), y1(y1), x2(x2), y2(y2) {}

// Duplication code Collision pour optimisation
bool Rect::intersect(float x, float y) const
{
    return x > x1 && y > y1 && x <= x2 && y <= y2;
}

bool Rect::intersect(const Line & line, float dx, float dy) const
{
    float line_x1 = line.x1 + dx, line_x2 = line.x2 + dx;
    float line_y1 = line.y1 + dy, line_y2 = line.y2 + dy;
    
    // Find min and max X for the segment
    float minX = line_x1;
    float maxX = line_x2;
    
    if(line_x1 > line_x2)
    {
        minX = line_x2;
        maxX = line_x1;
    }
    
    // Find the intersection of the segment's and rectangle's x-projections
    if(maxX > x2)
    {
        maxX = x2;
    }
    
    if(minX < x1)
    {
        minX = x1;
    }
    
    // If their projections do not intersect return false
    if(minX > maxX)
    {
        return false;
    }
    
    // Find corresponding min and max Y for min and max X we found before
    float minY = line_y1;
    float maxY = line_y2;
    float Dx = line_x2 - line_x1;
    
    if(std::abs(Dx) > 0.0000001)
    {
        float a = (line_y2 - line_y1) / Dx;
        float b = line_y1 - a * line_x1;
        minY = a * minX + b;
        maxY = a * maxX + b;
    }
    
    if(minY > maxY)
    {
        float tmp = maxY;
        maxY = minY;
        minY = tmp;
    }
    
    // Find the intersection of the segment's and rectangle's y-projections
    if(maxY > y2)
    {
        maxY = y2;
    }
    
    if(minY < y1)
    {
        minY = y1;
    }
    
    // If Y-projections do not intersect return false
    if(minY > maxY)
    {
        return false;
    }
    
    return true;
}

// Duplication code Collision pour optimisation
bool Rect::intersect(const Rect & b, float dx, float dy) const
{
    return (x2 > b.x1 + dx) && (x1 < b.x2 + dx)
        && (y2 > b.y1 + dy) && (y1 < b.y2 + dy);
}

bool Rect::intersect(const Disk & disk, float dx, float dy) const
{
    return disk.intersect(*this, dx, dy);
}

bool Rect::intersect(const Mask & mask, float dx, float dy) const
{
    return mask.intersect(*this, dx, dy);
}

bool Rect::intersect(const ConvexPolygon & poly, float dx, float dy) const
{
    return poly.intersect(*this, -dx, -dy);
}

bool Rect::intersect(const Polygon & poly, float dx, float dy) const
{
    return poly.intersect(*this, -dx, -dy);
}

bool Rect::collide(const Shape & s, float dx, float dy) const
{
    //number++;
    return s.intersect(*this, -dx, -dy);
}

const Rect Rect::boundingBox() const
{
    return *this;
}

float Rect::width() const
{
    return x2 - x1;
}

float Rect::height() const
{
    return y2 - y1;
}

Pos Rect::center() const
{
    return Pos((x1+x2)/2.f, (y1+y2)/2.f);
}

const Rect Rect::translate(float x, float y) const
{
    return Rect(x1-x, y1-y, x2-x, y2-y);
}

void Rect::translate(float x, float y)
{
    x1 -= x; x2 -= x;
    y1 -= y; y2 -= y;
}

void Rect::scale(float x, float y)
{
    x1 *= x; x2 *= x;
    y1 *= y; y2 *= y;
}

Rect & Rect::operator=(const Rect & rect)
{
    x1 = rect.x1;
    y1 = rect.y1;
    x2 = rect.x2;
    y2 = rect.y2;
    return *this;
}

} // namespace od
