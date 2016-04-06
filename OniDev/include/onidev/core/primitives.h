#ifndef ONIDEV_CORE_PRIMITIVES_H_INCLUDED
#define ONIDEV_CORE_PRIMITIVES_H_INCLUDED

namespace od
{

struct Size
{
    float wid;
    float hei;
    
    Size():wid(0),hei(0){}
    Size(float wid, float hei):wid(wid),hei(hei) {}
};

struct Pos
{
    float x;
    float y;
    
    Pos() {}
    Pos(float x, float y):x(x),y(y) {}
};

}

#endif // ONIDEV_CORE_PRIMITIVES_H_INCLUDED
