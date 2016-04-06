// Copyright 2013-2015 Boris Marmontel, All Rights Reserved.

/*=============================================================================
	container.inc
=============================================================================*/

/*-----------------------------------------------------------------------------
	Constructor & destructor.
-----------------------------------------------------------------------------*/

template<class T>
InstanceContainer<T>::InstanceContainer():
    destroyed(false)
{
}

template<class T>
InstanceContainer<T>::~InstanceContainer()
{
    destroyed = true;
    
    purge();
}

/*-----------------------------------------------------------------------------
	InstanceContainer methods.
-----------------------------------------------------------------------------*/

template<class T>
void InstanceContainer<T>::add(size_t object, std::shared_ptr<T> & ins)
{
    assert( !destroyed );
    
    if(instances.size() <= object)
    {
        instances.resize(object + 1);
        uid_map.resize(object + 1);
        last_uid.resize(object + 1);
    }
    // Add the instance pointer to the container
    instances[object].push_back(ins);
    /// @todo can be deactivated with template parameter
    if(ins->generateUid())
    {
        auto& last = last_uid[object];
        uid_map[object][last] = ins.get();
        ins->setUid(last);
        last++;
    }
}

template<class T>
void InstanceContainer<T>::add(size_t object, std::shared_ptr<T> & ins, unsigned uid)
{
    assert( !destroyed );
    
    if(instances.size() <= object)
    {
        instances.resize(object + 1);
        uid_map.resize(object + 1);
        last_uid.resize(object + 1);
    }
    
    // Add the instance pointer to the container
    instances[object].push_back(ins);
    
    if(ins->generateUid())
    {
        auto it = uid_map[object].find(uid);
        if(it == uid_map[object].end())
        {
            uid_map[object][uid] = ins.get();
            ins->setUid(uid);
            
            auto& last = last_uid[object];
            if(last <= uid)
                last = uid + 1;
        }
        else
        {
            throw std::runtime_error("InstanceContainer: uid is already assigned.");
        }
    }
    else
    {
        throw std::runtime_error("InstanceContainer: try to assign an uid but generateUid() return false.");
    }
}

template<class T>
void InstanceContainer<T>::remove(size_t object, T * ins, bool inside_loop, int loop_object, int& loop_cursor)
{
    assert( !destroyed && object < instances.size() );
    
    // Remove instance
    auto& t = instances[object];
    for(auto it=t.begin(); it!=t.end(); ++it)
    {
        if(&(**it) == ins)
        {
            if(ins->generateUid())
            {
                unsigned uid = ins->uid();
                uid_map[object].erase(uid);
            }
            
            if(inside_loop && loop_object == object)
            {
                int cursor = std::distance(t.begin(), it);
                if(loop_cursor <= cursor)
                {
                    loop_cursor--;
                }
            }
            
            t.erase(it);
            
            // on ne supprime pas l'element du tableau pour ne pas créer de decalage pendant
            // que le moteur effectue la boucle principale d'execution
            // en revanche avant la main loop, on enleve automatiquement les instances 'vides'
            //it->reset();
            return;
        }
    }
}

template<class T>
void InstanceContainer<T>::purge()
{
    instances.clear();
    uid_map.clear();
    last_uid.clear();
}

template<class T>
T* InstanceContainer<T>::find(size_t object, unsigned uid)
{
    if(object < uid_map.size())
    {
        auto it = uid_map[object].find(uid);
        if(it != uid_map[object].end())
            return it->second;
    }
    return nullptr;
}

template<class T>
size_t InstanceContainer<T>::number(int object) const
{
    assert( object < instances.size() );
    return instances[object].size();
}

template<class T>
size_t InstanceContainer<T>::number() const
{
    size_t ret = 0;
    for(size_t i=0; i<instances.size(); ++i)
    {
        ret += instances[i].size();
    }
    return ret;
}
