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
	collision.cpp
=============================================================================*/

#include <onidev/collision/collision.h>
#include <cmath>

namespace od
{

bool Collision::intersect(float x, float y, const Rect & box)
{
    return x > box.x1 && y > box.y1 && x <= box.x2 && y <= box.y2;
}

bool Collision::intersect(const Rect & a, const Rect & b, float dx, float dy)
{
    return (a.x2 > b.x1 + dx) && (a.x1 < b.x2 + dx)
        && (a.y2 > b.y1 + dy) && (a.y1 < b.y2 + dy);
}

/// @todo Delete this function in other object
bool Collision::intersect(const Rect & a, const Line & line, float dx, float dy)
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
    if(maxX > a.x2)
    {
        maxX = a.x2;
    }
    
    if(minX < a.x1)
    {
        minX = a.x1;
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
    if(maxY > a.y2)
    {
        maxY = a.y2;
    }
    
    if(minY < a.y1)
    {
        minY = a.y1;
    }
    
    // If Y-projections do not intersect return false
    if(minY > maxY)
    {
        return false;
    }
    
    return true;
}

/// @todo Delete this function in other object
bool Collision::intersect(const Rect & b, const Disk & disk, float dx, float dy)
{
    float x = disk.x - dx;
    float y = disk.y - dy;
    
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

    return ( diffx * diffx + diffy * diffy <= disk.r * disk.r );
}

/// @todo Delete this function in other object
bool Collision::intersect(const Rect & b, const Mask & mask, float dx, float dy)
{
    dx += mask.wx;
    dy += mask.wy;
    
    const float bx1 = b.x1 + dx;
    const float by1 = b.y1 + dy;
    const float bx2 = b.x2 + dx - 1;
    const float by2 = b.y2 + dy - 1;
    
    // test avec boite englobante
    if ( bx2 < mask.x1 || by2 < mask.y1 || bx1 > mask.x2 || by1 > mask.y2 )
        return false;
    
    // test au pixel près
    for(int i=bx1; i<=bx2; i++)
    for(int j=by1; j<=by2; j++)
    {
        if(i < mask.x1 || j < mask.y1 || i > mask.x2 || j > mask.y2)
            continue;
        
        if(mask.data[i + j*mask.wid])
            return true;
    }
    return false;
}

/// @todo Delete this function in other object
bool Collision::intersect(const Rect & rect, const ConvexPolygon & p, float dx, float dy)
{
    if(p.box.intersect(rect, dx, dy) == false)
        return false;
        
    std::vector<float> poly;
    poly.push_back(rect.x1); poly.push_back(rect.y1);
    poly.push_back(rect.x2); poly.push_back(rect.y1);
    poly.push_back(rect.x2); poly.push_back(rect.y2);
    poly.push_back(rect.x1); poly.push_back(rect.y2);
    
    for(unsigned i=0; i<p.points.size() - 2; i+=2)
    {
        float nx = p.points[i+2] - p.points[i];
        float ny = p.points[i+3] - p.points[i+1];
        if( p.getMinimumTranslationVector(poly, -ny, nx, dx, dy) > 0 )
            return false;
    }
    
    if( p.getMinimumTranslationVector(poly, 0,-1, dx, dy) > 0 )
        return false;
    if( p.getMinimumTranslationVector(poly, 0, 1, dx, dy) > 0 )
        return false;
    if( p.getMinimumTranslationVector(poly,-1, 0, dx, dy) > 0 )
        return false;
    if( p.getMinimumTranslationVector(poly, 1, 0, dx, dy) > 0 )
        return false;
    
    return true;
}

/// @todo Delete this function in other object
bool Collision::intersect(const Rect & rect, const Polygon & p, float dx, float dy)
{
    if(p.box.intersect(rect, dx, dy) == false)
        return false;
        
    for(unsigned i=0; i<p.poly.size(); i++)
    {
        if(p.poly[i].intersect(rect, dx, dy))
            return true;
    }
    return false;
}

/// @todo Delete this function in other object
bool Collision::intersect(const Line & me, const Line & line, float dx, float dy)
{
    float x3 = line.x1 + dx;
    float y3 = line.y1 + dy;
    float x4 = line.x2 + dx;
    float y4 = line.y2 + dy;

    float a = (y4 - y3)*(me.x2 - me.x1) - (x4 - x3)*(me.y2 - me.y1);
    if (a == 0)
        return false;
    
    float b = ( (x4 - x3)*(me.y1 - y3) - (y4 - y3)*(me.x1 - x3) ) / a;
    float d = ( (me.x2 - me.x1)*(me.y1 - y3) - (me.y2 - me.y1)*(me.x1 - x3) ) / a;
    
    //ix = x1 + b*(x2-x1);
    //iy = y1 + b*(y2-y1);
    
    if (b > 0 && b < 1 && d > 0 && d < 1)
        return true;
    
    return false;
}

/// @todo Delete this function in other object
bool Collision::intersect(const Line & line, const Disk & disk, float dx, float dy)
{
    float diskx = disk.x - dx;
    float disky = disk.y - dy;
    
    float cx, cy; // closest point
    
    float seg_vx = line.x2 - line.x1;
    float seg_vy = line.y2 - line.y1;
    float pt_vx = diskx - line.x1;
    float pt_vy = disky - line.y1;
    float len = sqrt(seg_vx*seg_vx + seg_vy*seg_vy);
    float seg_vx_unit = seg_vx / len;
    float seg_vy_unit = seg_vy / len;
    
    float proj = pt_vx*seg_vx_unit + pt_vy*seg_vy_unit;
    if(proj <= 0)
    {
        cx = line.x1;
        cy = line.y1;
    }
    else if(proj >= len)
    {
        cx = line.x2;
        cy = line.y2;
    }
    else
    {
        cx = line.x1 + seg_vx_unit*proj;
        cy = line.y1 + seg_vy_unit*proj;
    }
    
    float vx = diskx - cx;
    float vy = disky - cy;
    return vx*vx + vy*vy <= disk.r*disk.r;
}

bool Collision::intersect(const Line & a, const Mask & b, float dx, float dy)
{
    /// @todo Implement this function
    return false;
}

bool Collision::intersect(const Line & a, const ConvexPolygon & b, float dx, float dy)
{
    /// @todo Implement this function
    return false;
}

bool Collision::intersect(const Line & a, const Polygon & b, float dx, float dy)
{
    /// @todo Implement this function
    return false;
}

bool Collision::intersect(const Disk & me, const Disk & disk, float dx, float dy)
{
    float xx = me.x - disk.x + dx;
    float yy = me.y - disk.y + dy;
    float rr = me.r + disk.r;
    return xx*xx + yy*yy <= rr*rr;
}

bool Collision::intersect(const Disk & a, const Mask & b, float dx, float dy)
{
    /// @todo Implement this function
    return false;
}

bool Collision::intersect(const Disk & a, const ConvexPolygon & b, float dx, float dy)
{
    /// @todo Implement this function
    return false;
}

bool Collision::intersect(const Disk & a, const Polygon & b, float dx, float dy)
{
    /// @todo Implement this function
    return false;
}

static float min(int x, int y)
{
    if(x < y) return x; return y;
}
static float max(int x, int y)
{
    if(x > y) return x; return y;
}
/// @todo Delete this function in other object
bool Collision::intersect(const Mask & me, const Mask & mask, float dx, float dy)
{
    dx -= me.wx - mask.wx;
    dy -= me.wy - mask.wy;
    
    const float bx1 = mask.x1 - dx;
    const float by1 = mask.y1 - dy;
    const float bx2 = mask.x2 - dx;
    const float by2 = mask.y2 - dy;
    
    int xmin, xmax, ymin, ymax;
    xmin = max(me.x1, bx1);
    ymin = max(me.y1, by1);
    
    xmax = min(me.x2, bx2);
    ymax = min(me.y2, by2);
    
    if (xmax < xmin || ymax < ymin) {
        return false;
    }
    
    for(int y=ymin; y<=ymax; y++)
    for(int x=xmin; x<=xmax; x++)
    {
        if(me.data[x-me.x1 + (y-me.y1)*me.wid]==1 && mask.data[ (x-(int)bx1) + (y-(int)by1)*mask.wid]==1)
            return true;
    }
    return false;
}

bool Collision::intersect(const Mask & a, const ConvexPolygon & b, float dx, float dy)
{
    /// @todo Implement this function
    return false;
}

bool Collision::intersect(const Mask & a, const Polygon & b, float dx, float dy)
{
    /// @todo Implement this function
    return false;
}

/// @todo Delete this function in other object
bool Collision::intersect(const ConvexPolygon & me, const ConvexPolygon & poly, float dx, float dy)
{
    // Premier test avec les bounding box pour voir si une collision est probable
    // permet d'optimiser considérablement les performances
    if(me.box.intersect(poly.box, dx, dy) == false)
        return false;
    
    // normal : (-y, x) | (y, -x)
    for(unsigned i=0; i<me.points.size() - 2; i+=2)
    {
        float nx = me.points[i+2] - me.points[i];
        float ny = me.points[i+3] - me.points[i+1];
        if( me.getMinimumTranslationVector(poly.points, -ny, nx, dx, dy) > 0 )
            return false;
    }
    for(unsigned i=0; i<poly.points.size() - 2; i+=2)
    {
        float nx = poly.points[i+2] - poly.points[i];
        float ny = poly.points[i+3] - poly.points[i+1];
        if( me.getMinimumTranslationVector(poly.points, -ny, nx, dx, dy) > 0 )
            return false;
    }
    return true;
}
/// @todo Delete this function in other object
bool Collision::intersect(const ConvexPolygon & p, const Polygon & me, float dx, float dy)
{
    if(me.box.intersect(p.boundingBox(), dx, dy) == false)
        return false;
        
    for(unsigned i=0; i<me.poly.size(); i++)
    {
        if(me.poly[i].intersect(p, dx, dy))
            return true;
    }
    return false;
}
/// @todo Delete this function in other object
bool Collision::intersect(const Polygon & me, const Polygon & p, float dx, float dy)
{
    if(me.box.intersect(p.box, dx, dy) == false)
        return false;
    
    for(unsigned i=0; i<me.poly.size(); i++)
    {
        for(unsigned j=0; j<p.poly.size(); j++)
        {
            if(me.poly[i].intersect(p.poly[j], dx, dy))
                return true;
        }
    }
    return false;
}

}
