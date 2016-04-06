// Copyright 2013-2015 Boris Marmontel, All Rights Reserved.

/*=============================================================================
	mask.cpp
=============================================================================*/

#include <onidev/collision/rect.h>
#include <onidev/collision/mask.h>
//#include <onidev/image/image.h>

// optimisation possible: réduire la mémoire en reduisant le masque
// de façon a ce que la bounding box l'englobe totalement

namespace od
{

// optimisations: http://www.clownfrogfish.com/2012/09/22/efficient-aabb-bitmask-intersection-queries/
Mask::Mask(const Mask & m) : wx(m.wx), wy(m.wy), x1(m.x1), y1(m.y1), x2(m.x2), y2(m.y2), wid(m.wid), hei(m.hei), data(0)
{
    data = new char[wid * hei];
    for(int i=0; i<wid*hei; i++)
        data[i] = m.data[i];
}

Mask::Mask(const void * image_data, size_t image_width, size_t image_height, int wx, int wy):
    wx(wx), wy(wy), x1(0), y1(0), x2(image_width-1), y2(image_height-1),
    wid(image_width), hei(image_height)
{
    // copie du masque
    size_t size = image_width * image_height;
    data = new char[size];
    const unsigned char * ptr = static_cast<const unsigned char*>(image_data);
    for(size_t i=0; i<size; i++)
    {
        unsigned char pixel = *ptr; // red
        ptr += 4;
        data[i] = pixel != 0;
    }
    // Optimisation
    calculateBoundingBox();
}

Mask::Mask(const void * image_data, size_t image_width, size_t image_height, int x, int y, int width, int height, int wx, int wy):
    wx(wx), wy(wy), x1(0), y1(0), x2(width-1), y2(height-1),
    wid(width), hei(height)
{
    // copie
    data = new char[width * height];
    const unsigned char * ptr = static_cast<const unsigned char*>(image_data);
    for(int j=0; j<height; j++)
    {
        for(int i=0; i<width; i++)
        {
            unsigned char pixel = ptr[4 * (x+i + (y+j)*image_width)]; // red
            data[i + j*width] = pixel != 0;
        }
    }
    // Optimisation
    calculateBoundingBox();
}

Mask::Mask(const char * mask, int wid, int hei, int wx, int wy)
: wx(wx), wy(wy), x1(0), y1(0), x2(wid-1), y2(hei-1), wid(wid), hei(hei), data(0)
{
    // Copie du masque
    data = new char[wid * hei];
    for(int i=0; i<wid * hei; i++)
        data[i] = mask[i];
    // Optimisation
    calculateBoundingBox();
    
    //printf("Left:%d, Top:%d, Right:%d, Bottom:%d", x1, y1, x2, y2);
}

Mask::~Mask()
{
    delete[] data;
}

void Mask::calculateBoundingBox()
{
    /// ==> fait toujours tout bugger
    /// Il faudra trouver rapidement d'ou viens le soucis
    return;
    
    // Optimisation du masque en récupérant la bouding box qui l'entoure
    
    // Scanlines Gauche->Droite Haut->Bas
    // ↓↓↓
    // ↓↓. -> left = 2
    // ↓↓ 
    bool find = false;
    for(int i=0; i<wid && !find; i++)
    {
        for(int j=0; j<hei; j++)
        {
            if(data[i + j*wid])
            {
                x1 = i;
                find = true;
                break;
            }
        }
    }
    
    // Scanlines Droite->Gauche Haut->Bas
    // ↓↓
    // .↓ -> right = wid - 1
    //  ↓
    find = false;
    for(int i=wid-1; i>=0 && !find; i--)
    {
        for(int j=0; j<hei; j++)
        {
            if(data[i + j*wid])
            {
                x2 = i;
                find = true;
                break;
            }
        }
    }
    
    // Scanlines Haut->Bas Gauche->Droite
    // → → →
    // → → → -> top = 2
    // → → .
    find = false;
    for(int j=0; j<hei && !find; j++)
    {
        for(int i=0; i<wid; i++)
        {
            if(data[i + j*wid])
            {
                y1 = j;
                find = true;
                break;
            }
        }
    }
    
    // Scanlines Bas->Haut Gauche->Droite
    // → → .
    // → → → -> bottom = hei - 2
    // → → →
    find = false;
    for(int j=hei-1; j>=0 && !find; j--)
    {
        for(int i=0; i<wid; i++)
        {
            if(data[i + j*wid])
            {
                y2 = j;
                find = true;
                break;
            }
        }
    }
}

bool Mask::intersect(float x, float y) const
{
    /*if(x < x1 || y < y1 || x > x2 || y > y2)
        return false;
    return data[ (int)x + ((int)y) * wid ];*/
    
    if(x < x1-wx || y < y1-wy || x > x2-wx || y > y2-wy)
        return false;
    return data[ (int)x+wx + ((int)y+wy) * wid ];
}
/// todo: verifier si pas de bugs a cause de l'offset
bool Mask::intersect(const Rect & b, float dx, float dy) const
{
    dx += wx;
    dy += wy;
    
    const float bx1 = b.x1 + dx;
    const float by1 = b.y1 + dy;
    const float bx2 = b.x2 + dx - 1;
    const float by2 = b.y2 + dy - 1;
    
    // test avec boite englobante
    if ( bx2 < x1 || by2 < y1 || bx1 > x2 || by1 > y2 )
        return false;
    
    // test au pixel près
    for(int i=bx1; i<=bx2; i++)
    for(int j=by1; j<=by2; j++)
    {
        if(i < x1 || j < y1 || i > x2 || j > y2)
            continue;
        
        if(data[i + j*wid])
            return true;
    }
    return false;
}

/*bool Mask::intersect(const Rect & b, float dx, float dy) const
{
    dx += wx;
    dy += wy;
    
    const float bx1 = b.x1 + dx;
    const float by1 = b.y1 + dy;
    const float bx2 = b.x2 + dx;
    const float by2 = b.y2 + dy;
    
    // test avec boite englobante
    if ( bx2 < x1 || by2 < y1 || bx1 > x2 || by1 > y2 )
        return false;
    
    // test au pixel près
    for(int i=bx1; i<=bx2; i++)
    for(int j=by1; j<=by2; j++)
    {
        if(i < x1 || j < y1 || i > x2 || j > y2)
            continue;
        
        if(data[i + j*wid])
            return true;
    }
    return false;
}*/

bool Mask::intersect(const Disk & disk, float dx, float dy) const
{
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

/// a optimiser
bool Mask::intersect(const Mask & mask, float dx, float dy) const
{
    //printf("wx:%d wy:%d dx:%d dy:%d\n", mask.wx, mask.wy, (int)dx, (int)dy);
    dx -= wx - mask.wx;
    dy -= wy - mask.wy;
    
    const float bx1 = mask.x1 - dx;
    const float by1 = mask.y1 - dy;
    const float bx2 = mask.x2 - dx;
    const float by2 = mask.y2 - dy;
    
    //if ( bx2 < x1 || by2 < y1 || bx1 > x2 || by1 > y2 )
    //    return false;
    //printf("[%d %d %d %d][%d %d %d %d]\n", x1, y1, x2, y2, (int)bx1, (int)by1, (int)bx2, (int)by2);
    
    int xmin, xmax, ymin, ymax;
    xmin = max(x1, bx1);
    ymin = max(y1, by1);
    
    xmax = min(x2, bx2);
    ymax = min(y2, by2);
    
    if (xmax < xmin || ymax < ymin) {
        return false;
    }
    
    //printf("%d %d %d %d\n", xmin, ymin, xmax, ymax);
    
    for(int y=ymin; y<=ymax; y++)
    for(int x=xmin; x<=xmax; x++)
    {
        if(data[x-x1 + (y-y1)*wid]==1 && mask.data[ (x-(int)bx1) + (y-(int)by1)*mask.wid]==1)
            return true;
    }
    return false;
}

bool Mask::collide(const Shape & s, float dx, float dy) const
{
    return s.intersect(*this, dx, dy);
}

const od::Rect Mask::boundingBox() const
{
    return Rect(x1-wx, y1-wy, x2-wx+1, y2-wy+1);
}

Mask & Mask::operator=(const Mask & m)
{
    if(&m == this)
        return *this;
    
    if(data) delete[] data;
    
    wx = m.wx;
    wy = m.wy;
    x1 = m.x1;
    y1 = m.y1;
    x2 = m.x2;
    y2 = m.y2;
    wid = m.wid;
    hei = m.hei;
    
    data = new char[wid * hei];
    for(int i=0; i<wid*hei; i++)
        data[i] = m.data[i];
        
    return *this;
}

} // namespace od
