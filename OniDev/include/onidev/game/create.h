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

#ifndef ONIDEV_GAME_OBJECTBASE_H_INCLUDED
#define ONIDEV_GAME_OBJECTBASE_H_INCLUDED

#include <onidev/game/context.h>

namespace od
{

template <class C, class... Args>
std::weak_ptr<C> instanceCreateWeak(Args&&... args)
{
    std::shared_ptr<C> ins = std::make_shared<C>(std::forward<Args>(args)...);
    ins->context().add(ins->objectIndex(), ins, ins->objectPriority());
    ins->eventCreated();
    return ins;
}

template <class C, class... Args>
std::shared_ptr<C> instanceCreateShared(Args&&... args)
{
    std::shared_ptr<C> ins = std::make_shared<C>(std::forward<Args>(args)...);
    ins->context().add(ins->objectIndex(), ins, ins->objectPriority());
    ins->eventCreated();
    return ins;
}

template <class C, class... Args>
C* instanceCreate(Args&&... args)
{
    std::shared_ptr<C> ins = std::make_shared<C>(std::forward<Args>(args)...);
    ins->context().add(ins->objectIndex(), ins, ins->objectPriority());
    ins->eventCreated();
    return ins.get();
}

template <class C, class... Args>
std::weak_ptr<C> instanceCreateUidWeak(unsigned uid, Args&&... args)
{
    std::shared_ptr<C> ins = std::make_shared<C>(std::forward<Args>(args)...);
    ins->context().add(ins->objectIndex(), ins, ins->objectPriority(), uid);
    ins->eventCreated();
    return ins;
}

template <class C, class... Args>
std::shared_ptr<C> instanceCreateUidShared(unsigned uid, Args&&... args)
{
    std::shared_ptr<C> ins = std::make_shared<C>(std::forward<Args>(args)...);
    ins->context().add(ins->objectIndex(), ins, ins->objectPriority(), uid);
    ins->eventCreated();
    return ins;
}

template <class C, class... Args>
C* instanceCreateUid(unsigned uid, Args&&... args)
{
    std::shared_ptr<C> ins = std::make_shared<C>(std::forward<Args>(args)...);
    ins->context().add(ins->objectIndex(), ins, ins->objectPriority(), uid);
    ins->eventCreated();
    return ins.get();
}

}

#endif
