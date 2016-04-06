// Copyright 2013-2015 Boris Marmontel, All Rights Reserved.

/*=============================================================================
	disk.cpp
=============================================================================*/

#include <onidev/collision/rect.h>
#include <onidev/collision/disk.h>
#include <onidev/collision/line.h>

namespace od
{

Disk::Disk(): x(0), y(0), r(0) {}
Disk::Disk(const Disk & d): x(d.x), y(d.y), r(d.r) {}
Disk::Disk(float x, float y, float r): x(x), y(y), r(r) {}

bool Disk::intersect(float x, float y) const
{
    float dx = this->x - x, dy = this->y - y;
    return dx*dx + dy*dy <= r*r;
}

bool Disk::intersect(const Line & line, float dx, float dy) const
{
    return line.intersect(*this, dx, dy);
}
    
bool Disk::intersect(const Rect & b, float dx, float dy) const
{
    /// optimisation possible avec boite englobante
    float x = this->x - dx;
    float y = this->y - dy;
    
    float maxx = b.x2;
    float maxy = b.y2;

    float closestPointx = x;
    float closestPointy = y;
        
    if( x < b.x1 )
        closestPointx = b.x1;
    else if( x > maxx )
        closestPointx = maxx;

    if( y < b.y1 )
        closestPointy = b.y1;
    else if( y > maxy )
        closestPointy = maxy;
        
    float diffx = closestPointx - x;
    float diffy = closestPointy - y;

    return ( diffx * diffx + diffy * diffy <= r * r );
}

bool Disk::intersect(const Disk & disk, float dx, float dy) const
{
    float xx = x - disk.x + dx;
    float yy = y - disk.y + dy;
    float rr = r + disk.r;
    return xx*xx + yy*yy <= rr*rr;
}

bool Disk::intersect(const Mask & mask, float dx, float dy) const
{
    return false;
}

bool Disk::collide(const Shape & s, float dx, float dy) const
{
    return s.intersect(*this, dx, dy);
}

const od::Rect Disk::boundingBox() const
{
    return Rect(x-r, y-r, x+r, y+r);
}

void Disk::translate(float x, float y)
{
    this->x -= x;
    this->y -= y;
}

Disk & Disk::operator=(const Disk & d)
{
    x = d.x;
    y = d.y;
    r = d.r;
    return *this;
}

} // namespace od
