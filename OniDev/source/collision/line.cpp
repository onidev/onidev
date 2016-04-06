// Copyright 2013-2015 Boris Marmontel, All Rights Reserved.

/*=============================================================================
	line.cpp
=============================================================================*/

#include <onidev/collision/line.h>
#include <onidev/collision/rect.h>
#include <onidev/collision/disk.h>
#include <cmath>

namespace od
{

Line::Line(): x1(0), y1(0), x2(0), y2(0) {}
Line::Line(const Line & line): x1(line.x1), y1(line.y1), x2(line.x2), y2(line.y2) {}
Line::Line(float x1, float y1, float x2, float y2): x1(x1), y1(y1), x2(x2), y2(y2) {}


bool Line::intersect(float x, float y) const
{
    return false;
}
    
bool Line::intersect(const Line & line, float dx, float dy) const
{
    float x3 = line.x1 + dx;
    float y3 = line.y1 + dy;
    float x4 = line.x2 + dx;
    float y4 = line.y2 + dy;

    float a = (y4-y3)*(x2-x1) - (x4-x3)*(y2-y1);
    if (a == 0)
        return false;

    float b = ( (x4-x3)*(y1-y3) - (y4-y3)*(x1-x3) ) / a;
    float d = ( (x2-x1)*(y1-y3) - (y2-y1)*(x1-x3) ) / a;
    
    //ix = x1 + b*(x2-x1);
    //iy = y1 + b*(y2-y1);

    if (b > 0 && b < 1 && d > 0 && d < 1)
        return true;

    return false;
}

bool Line::intersect(const Rect & rect, float dx, float dy) const
{
    return rect.intersect(*this, -dx, -dy);
}

bool Line::intersect(const Disk & disk, float dx, float dy) const
{
    float diskx = disk.x - dx;
    float disky = disk.y - dy;
    
    float cx, cy; // closest point
    
    float seg_vx = x2 - x1;
    float seg_vy = y2 - y1;
    float pt_vx = diskx - x1;
    float pt_vy = disky - y1;
    float len = sqrt(seg_vx*seg_vx + seg_vy*seg_vy);
    float seg_vx_unit = seg_vx / len;
    float seg_vy_unit = seg_vy / len;
    
    float proj = pt_vx*seg_vx_unit + pt_vy*seg_vy_unit;
    if(proj <= 0) {
        cx = x1;
        cy = y1;
    }
    else if(proj >= len) {
        cx = x2;
        cy = y2;
    }
    else
    {
        cx = x1 + seg_vx_unit*proj;
        cy = y1 + seg_vy_unit*proj;
    }
    
    float vx = diskx - cx;
    float vy = disky - cy;
    return vx*vx + vy*vy <= disk.r*disk.r;
    /*
    // Ray VS Disk
    float diskx = disk.x - dx;
    float disky = disk.y - dy;
    
    float Dx = x2 - x1, Dy = y2 - y1;
    float Lx = x1 - diskx, Ly = y1 - disky;
    float a = Dx*Dx + Dy*Dy;
    float b = 2 * (Lx*Dx + Ly*Dy);
    float c = Lx*Lx + Ly*Ly - disk.r*disk.r;
    
    return b*b - 4*a*c > 0;*/
}

bool Line::intersect(const Mask & mask, float dx, float dy) const
{
    return false;
}

bool Line::intersect(const ConvexPolygon & poly, float dx, float dy) const
{
    return false;
}

bool Line::intersect(const Polygon & poly, float dx, float dy) const
{
    return false;
}

bool Line::collide(const Shape & shape, float dx, float dy) const
{
    return shape.intersect(*this, -dx, -dy);
}

const Rect Line::boundingBox() const
{
    return Rect(x1, y1, x2, y2);
}


}
