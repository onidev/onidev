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
