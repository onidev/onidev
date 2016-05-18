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
