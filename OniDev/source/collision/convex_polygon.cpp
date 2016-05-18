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
	convexPolygon.cpp
=============================================================================*/

#include <onidev/collision/polygon.h>
#include <cmath>
#include <cfloat>

static const float m_pi = 3.14159265f;

static float sign(float x)
{
    if(x < 0) return -1;
    if(x > 0) return 1;
    return 0;
}

namespace od
{

ConvexPolygon::ConvexPolygon() : points(), box() {}

ConvexPolygon::ConvexPolygon(const float t[], int n, bool centerShape) : points(), box()
{
    for(int i=0; i<n*2; i++) points.push_back(t[i]);
    points.push_back(t[0]);
    points.push_back(t[1]);
    updateBoundingBox();
    if(centerShape) center();
}

ConvexPolygon::ConvexPolygon(const ConvexPolygon & poly) : points(poly.points), box(poly.box) {}

ConvexPolygon & ConvexPolygon::operator=(const ConvexPolygon & poly)
{
    points = poly.points;
    box = poly.box;
    return *this;
}

bool ConvexPolygon::intersect(float x, float y) const { return false; }

bool ConvexPolygon::intersect(const Rect & rect, float dx, float dy) const
{
    if(box.intersect(rect, dx, dy) == false)
        return false;
        
    std::vector<float> poly;
    poly.push_back(rect.x1); poly.push_back(rect.y1);
    poly.push_back(rect.x2); poly.push_back(rect.y1);
    poly.push_back(rect.x2); poly.push_back(rect.y2);
    poly.push_back(rect.x1); poly.push_back(rect.y2);
    
    for(unsigned i=0; i<points.size() - 2; i+=2)
    {
        float nx = points[i+2] - points[i];
        float ny = points[i+3] - points[i+1];
        if( getMinimumTranslationVector(poly, -ny, nx, dx, dy) > 0 )
            return false;
    }
    
    if( getMinimumTranslationVector(poly, 0,-1, dx, dy) > 0 )
        return false;
    if( getMinimumTranslationVector(poly, 0, 1, dx, dy) > 0 )
        return false;
    if( getMinimumTranslationVector(poly,-1, 0, dx, dy) > 0 )
        return false;
    if( getMinimumTranslationVector(poly, 1, 0, dx, dy) > 0 )
        return false;
    
    return true;
}

bool ConvexPolygon::intersect(const Disk & disk, float dx, float dy) const { return false; }
bool ConvexPolygon::intersect(const Mask & mask, float dx, float dy) const { return false; }

bool ConvexPolygon::intersect(const ConvexPolygon & poly, float dx, float dy) const
{
    // Premier test avec les bounding box pour voir si une collision est probable
    // permet d'optimiser considérablement les performances
    if(box.intersect(poly.box, dx, dy) == false)
        return false;
    
    // normal : (-y, x) | (y, -x)
    for(unsigned i=0; i<points.size() - 2; i+=2)
    {
        float nx = points[i+2] - points[i];
        float ny = points[i+3] - points[i+1];
        if( getMinimumTranslationVector(poly.points, -ny, nx, dx, dy) > 0 )
            return false;
    }
    for(unsigned i=0; i<poly.points.size() - 2; i+=2)
    {
        float nx = poly.points[i+2] - poly.points[i];
        float ny = poly.points[i+3] - poly.points[i+1];
        if( getMinimumTranslationVector(poly.points, -ny, nx, dx, dy) > 0 )
            return false;
    }
    return true;
}
bool ConvexPolygon::intersect(const Polygon & poly, float dx, float dy) const
{
    return poly.intersect(*this, -dx, -dy);
}

bool ConvexPolygon::collide(const Shape & s, float dx, float dy) const
{
    return s.intersect(*this, -dx, -dy);
}

void ConvexPolygon::updateBoundingBox()
{
    if(points.empty())
        return;
    
    box.x1=points[0]; box.x2=points[0];
    box.y1=points[1]; box.y2=points[1];
    
    for(unsigned i=2; i<points.size()-2; i+=2)
    {
        float x = points[i];
        float y = points[i+1];
        if(x < box.x1) box.x1 = x;
        if(x > box.x2) box.x2 = x;
        if(y < box.y1) box.y1 = y;
        if(y > box.y2) box.y2 = y;
    }
}

const od::Rect ConvexPolygon::boundingBox() const
{
    return box;
}

void ConvexPolygon::center()
{
    if(points.empty())
        return;
    
    float dx = (box.x1 + box.x2)/2.0, dy = (box.y1 + box.y2)/2.0;
    for(unsigned i=0; i<points.size(); i+=2)
    {
        points[i]  -= dx;
        points[i+1]-= dy;
    }
    
    box.translate(dx, dy);
}

void ConvexPolygon::set(const float t[], int n)
{
    for(int i=0; i<n*2; i++)
        points.push_back(t[i]);
    points.push_back(t[0]);
    points.push_back(t[1]);
    
    updateBoundingBox();
}

void ConvexPolygon::clear() { points.clear(); } // // //

// changement d'echelle
void ConvexPolygon::scale(float x, float y)
{
    for(unsigned i=0; i<points.size(); i+=2)
    {
        points[i]  *= x;
        points[i+1]*= y;
    }
    
    // on applique aussi la tranformation a la bounding box
    box.scale(x, y);
}

// translation
void ConvexPolygon::translate(float x, float y)
{
    for(unsigned i=0; i<points.size(); i+=2)
    {
        points[i]  -= x;
        points[i+1]-= y;
    }
    
    // on applique aussi la translation a la bounding box
    box.translate(x, y);
}

// rotation de centre O
void ConvexPolygon::rotate(float angle)
{
    const float cos0 = cos(angle * m_pi/180.0);
    const float sin0 = sin(angle * m_pi/180.0);
    
    for(unsigned i=0; i<points.size(); i+=2)
    {
        float t = cos0*points[i] - sin0*points[i+1];
        points[i+1] = sin0*points[i] + cos0*points[i+1];
        points[i] = t;
    }
    
    // trouver un moyen d'update la bounding box plus rapidement
    updateBoundingBox();
}


// rotation de centre O en se basant sur le modèle passé en parametre
void ConvexPolygon::rotate(const ConvexPolygon & poly, float angle)
{
    const float cos0 = cos(angle * m_pi/180.0);
    const float sin0 = sin(angle * m_pi/180.0);
    
    if(points.size() != poly.points.size())
        points.resize(poly.points.size());
    
    for(unsigned i=0; i<points.size(); i+=2)
    {
        points[i]  = cos0*poly.points[i] - sin0*poly.points[i+1];
        points[i+1]= sin0*poly.points[i] + cos0*poly.points[i+1];
    }
    
    // trouver un moyen d'update la bounding box plus rapidement
    updateBoundingBox();
}

void ConvexPolygon::rotate(const ConvexPolygon & poly, float cos0, float sin0)
{
    if(points.size() != poly.points.size())
        points.resize(poly.points.size());
    
    for(unsigned i=0; i<points.size(); i+=2)
    {
        points[i]  = cos0*poly.points[i] - sin0*poly.points[i+1];
        points[i+1]= sin0*poly.points[i] + cos0*poly.points[i+1];
    }
    
    // trouver un moyen d'update la bounding box plus rapidement
    updateBoundingBox();
}

// rotation de centre (wx, wy) en se basant sur le modèle passé en parametre
void ConvexPolygon::rotate(const ConvexPolygon & poly, float angle, float wx, float wy)
{
    const float cos0 = cos(angle * m_pi/180.0);
    const float sin0 = sin(angle * m_pi/180.0);
    
    if(points.size() != poly.points.size())
        points.resize(poly.points.size());
    
    for(unsigned i=0; i<points.size(); i+=2)
    {
        points[i]  = cos0*(poly.points[i] - wx) - sin0*(poly.points[i+1] - wy);
        points[i+1]= sin0*(poly.points[i] - wx) + cos0*(poly.points[i+1] - wy);
    }
    
    // trouver un moyen d'update la bounding box plus rapidement
    updateBoundingBox();
}

float ConvexPolygon::getMinimumTranslationVector(const std::vector<float> & poly, float nx, float ny, float dx, float dy) const
{
    float min1 = DBL_MAX, max1 = -DBL_MAX;
    float min2 = DBL_MAX, max2 = -DBL_MAX;
    
    for(unsigned i=0; i<points.size(); i+=2)
    {
        // projection du point sur l'axe donné en parametre
        float scalar = points[i]*nx + points[i+1]*ny; // px*nx + py*ny
        float x = scalar * nx;
        float y = scalar * ny;
        
        float val = 0.0;
        if(sign(x) == sign(nx) && sign(y) == sign(ny))
            val = sqrt(x*x + y*y);
        else
            val =-sqrt(x*x + y*y);
        
        if(val < min1) min1 = val;
        if(val > max1) max1 = val;
    }
    
    for(unsigned i=0; i<poly.size(); i+=2)
    {
        float scalar = (poly[i] + dx)*nx + (poly[i+1] + dy)*ny;
        float x = scalar * nx;
        float y = scalar * ny;
        
        float val = 0.0;
        if(sign(x) == sign(nx) && sign(y) == sign(ny))
            val = sqrt(x*x + y*y);
        else
            val =-sqrt(x*x + y*y);
        
        if(val < min2) min2 = val;
        if(val > max2) max2 = val;
    }
    
    float min, max;
    if(min1 < min2) min = min1; else min = min2;
    if(max1 > max2) max = max1; else max = max2;
    return max - min - (max2 - min2 + max1 - min1);
}
} // namespace od
