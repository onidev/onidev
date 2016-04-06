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
