// Copyright 2013-2015 Boris Marmontel, All Rights Reserved.

/*=============================================================================
	polygon.cpp
=============================================================================*/

#include <onidev/collision/polygon.h>
#include <cmath>

static const float m_pi = 3.14159265f;

namespace od
{

Polygon::Polygon(const Polygon & p) : poly(p.poly), box(p.box) {}
//Polygon::Polygon(const ConvexPolygon & p) : poly(), box(p.boundingBox())
Polygon::Polygon(const float pts[], const int n[], int count, bool centerShape) : poly(), box()
{
    int p=0;
    for(int i=0; i<count; i++)
    {
        poly.push_back( ConvexPolygon(&pts[p], n[i]) );
        p += n[i]*2;
    }
    updateBoundingBox();
    if(centerShape)
        center();
}

bool Polygon::intersect(float x, float y) const
{
    return false;
}

bool Polygon::intersect(const Rect & rect, float dx, float dy) const
{
    if(box.intersect(rect, dx, dy) == false)
        return false;
        
    for(unsigned i=0; i<poly.size(); i++)
    {
        if(poly[i].intersect(rect, dx, dy))
            return true;
    }
    return false;
}

bool Polygon::intersect(const Disk & disk, float dx, float dy) const
{
    return false;
}

bool Polygon::intersect(const Mask & mask, float dx, float dy) const
{
    return false;
}

bool Polygon::intersect(const ConvexPolygon & p, float dx, float dy) const
{
    if(box.intersect(p.boundingBox(), dx, dy) == false)
        return false;
        
    for(unsigned i=0; i<poly.size(); i++)
    {
        if(poly[i].intersect(p, dx, dy))
            return true;
    }
        
    return false;
}

bool Polygon::intersect(const Polygon & p, float dx, float dy) const
{
    if(box.intersect(p.box, dx, dy) == false)
        return false;
    
    for(unsigned i=0; i<poly.size(); i++)
    {
        for(unsigned j=0; j<p.poly.size(); j++)
        {
            if(poly[i].intersect(p.poly[j], dx, dy))
                return true;
        }
    }
    
    return false;
}

bool Polygon::collide(const Shape & s, float dx, float dy) const
{
    return s.intersect(*this, -dx, -dy);
}

const od::Rect Polygon::boundingBox() const
{
    return box;
}

Polygon & Polygon::operator=(const Polygon & d)
{
    poly = d.poly;
    box = d.box;
    
    return *this;
}


void Polygon::translate(float x, float y)
{
    for(unsigned i=0; i<poly.size(); i++)
    {
        poly[i].translate(x, y);
    }
    box.translate(x, y);
}

void Polygon::rotate(const Polygon & model, float angle)
{
    if(poly.size() != model.poly.size())
        poly.resize( model.poly.size() );
    
    const float cos0 = cosf(angle * m_pi/180.0);
    const float sin0 = sinf(angle * m_pi/180.0);
    
    for(unsigned i=0; i<poly.size(); i++)
    {
        poly[i].rotate(model.poly[i], cos0, sin0);
    }
    
    updateBoundingBox();
}

void Polygon::updateBoundingBox()
{
    if(poly.empty())
        return;
    
    box = poly[0].boundingBox();
    
    for(unsigned i=1; i<poly.size(); i++)
    {
        const Rect & r = poly[i].boundingBox();
        if(r.x1 < box.x1) box.x1 = r.x1;
        if(r.y1 < box.y1) box.y1 = r.y1;
        if(r.x2 > box.x2) box.x2 = r.x2;
        if(r.y2 > box.y2) box.y2 = r.y2;
    }
}

void Polygon::center()
{
    if(poly.empty())
        return;
    
    float dx = (box.x1 + box.x2)/2.0, dy = (box.y1 + box.y2)/2.0;
    for(unsigned i=0; i<poly.size(); i++)
    {
        poly[i].translate(dx, dy);
    }
    box.translate(dx, dy);
}
} // namespace od
