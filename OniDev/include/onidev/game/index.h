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

#ifndef ONIDEV_GAME_AUTO_INDEX_H_INCLUDED
#define ONIDEV_GAME_AUTO_INDEX_H_INCLUDED

#include <onidev/game/object2d.h>

namespace od
{

template<class U, class V, int P = 0>
class AutoIndex: public U
{
public:
    static const size_t ObjectIndex;
    static const size_t ObjectPriority = P;
    
    template<class... Args>
    AutoIndex(Args&&... args): // cppcheck: add explicit
        U(std::forward<Args>(args)...) {}
    
    size_t objectIndex() const {
        return ObjectIndex;
    }
    
    virtual size_t objectPriority() const {
        return ObjectPriority;
    }
};
template<class U, class V, int P>
const size_t AutoIndex<U, V, P>::ObjectIndex = U::GlobalCounter++;

}

#endif
