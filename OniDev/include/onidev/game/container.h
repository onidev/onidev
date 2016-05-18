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

#ifndef ONIDEV_GAME_CONTAINER_H_INCLUDED
#define ONIDEV_GAME_CONTAINER_H_INCLUDED

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <cassert>

namespace od
{

template<class T>
struct InstanceContainer
{
    std::vector<std::vector<std::shared_ptr<T>>> instances;
    
    // unique identifiers for multiplayer engine
    /// @todo can be deactivated with template parameter
    std::vector<std::map<unsigned, T*>> uid_map;
    std::vector<unsigned> last_uid;
    
    bool destroyed;
    
    InstanceContainer();
    ~InstanceContainer();
    
    void add(size_t object, std::shared_ptr<T> & ins);
    void add(size_t object, std::shared_ptr<T> & ins, unsigned uid);
    void remove(size_t object, T * ins, bool inside_loop, int loop_object, int& loop_cursor);
    void purge();
    
    T* find(size_t object, unsigned uid);
    
    size_t number(int object) const;
    size_t number() const;
};

#include "container.inc"

}

#endif
