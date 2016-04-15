
## Using the instance manager

The instance manager allows you to:
- contain different object instances inheriting from the same object
- call their update and draw events from the main loop automatically
- get informations about instances and check if an instance exists
- handle collisions between instances
- partition the space to optimize collision detection
- handle collisions between instances and collision layers

### Creating the instance context

An instance context will contain instances created after that it has been defined as the global context.

On its creation, a context is automatically defined as the global context.

```c++
od::InstanceContext<Object> ic;
od::instanceCreate<Foo>();
```

Using multiple contexts is rarely useful yet it allows you to run multiple "worlds" at once without having instances from a world being affected by ones in another world.

This may be useful to create a minigame in a game, or a game that can be played in multiple dimensions at once for example.

```c++
od::InstanceContext<Object> context1;
od::InstanceContext<Object> context2;

context1.setGlobal();
od::instanceCreate<Foo>();
od::instanceCreate<Bar>();

context2.setGlobal();
od::instanceCreate<Bar>();
```

If you don't create a context, a default one will be used.

You can access it using:

```c++
auto& context = od::InstanceContext<Object>::getDefault();
```

Once the context was created, you can update instances contained by it and draw them.
```c++
od::InstanceContext<Object> ic;
od::instanceCreate<Foo>();

while(mainLoop())
{
    ic.update( deltaTime() );
    ic.render();
}
```


### Implementing entities

When you implement an entity, you can make it inherit:
- either from od::Object2d
- either from od::GenericObject2d<T> if od::Object2d does not fit your requirements

Every entity requires an unique identifier. Doing so manually is quite restrictive and a class exists to make it easier.

```c++
class Entity:
    public od::AutoIndex<od::Object2d, Entity>
{
public:
    Entity(float x, float y):
        od::AutoIndex<od::Object2d, Entity>(x, y) {}
};
```

od::AutoIndex needs 3 template parameters including a facultative one:
- The type from which we should inherit
- The type of the inheriting object
- An integer defining the draw priority of the object.

To make an object inherit you need to do so:
```c++
class EntityBase:
    public od::AutoIndex<od::Object2d, EntityBase>
{
public:
    EntityBase(float x, float y):
        od::AutoIndex<od::Object2d, EntityBase>(x, y) {}
};

class Entity:
    public od::AutoIndex<EntityBase, Entity>
{
public:
    Entity(float x, float y):
        od::AutoIndex<EntityBase, Entity>(x, y) {}
};
```

Many methods can be redefined, the most important ones being:
- collisionMask() which defines the collision mask of the instance
- step() that will be called by InstanceContext::update()
- draw() that will be called by InstanceContext::render()

Here is a complete example:
```c++
class Player:
    public od::AutoIndex<od::Object2d, Player, 10>
{
public:
    Player(float x, float y):
        od::AutoIndex<od::Object2d, Player, 10>(x, y) {}
    
    od::Shape* collisionMask(int p) const override {
        static od::Rect rect(0, 0, 16, 16);
        return &rect;
    }
    void step(float dt) override {
        if(od::keyCheck(od::vk_left)) pos.x -= dt;
        if(od::keyCheck(od::vk_right)) pos.x += dt;
        if(od::keyCheck(od::vk_up)) pos.y -= dt;
        if(od::keyCheck(od::vk_down)) pos.y += dt;
    }
    void draw() const override {
        glColor3f(1, 1, 1); 
        od::drawRectangle(pos.x, pos.y, pos.x+16, pos.y+16);
    }
};
```


### Implementing a custom entity type

In some cases you may need to implement a custom entity type, for example if you want to add custom events or virtual methods.
You also can create a draw event that takes custom parameters, which can be extremely useful for some libraries that requires accessing to a RenderTarget.

To do so, you just need to make your class inherit from od::GenericObject2d<T> so:
```c++
class sfObject:
    public od::GenericObject2d<sfObject>
{
    template<class U, class V, int P>
    friend class od::AutoIndex;
    static size_t GlobalCounter;
    
public:
    sfObject(float x, float y):
        GenericObject2d<sfObject>(x, y) {}
    
    virtual void draw(sf::RenderTarget& render_target) const {}
};
```

GlobalCounter is required for od::AutoIndex to work. Don't forget its implementation in the matching .cpp:
```c++
size_t sfObject::GlobalCounter = 0;
```

You now can use sfObject as a base class for all your entities:
```c++
class Player:
    public od::AutoIndex<sfObject, Player>
{
public:
    Player(float x, float y):
        od::AutoIndex<sfObject, Player>(x, y) {}
    
    od::Shape* collisionMask(int p) const override {
        static od::Rect rect(0, 0, 16, 16);
        return &rect;
    }
    void step(float dt) override {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) pos.x -= dt;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) pos.x += dt;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) pos.y -= dt;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) pos.y += dt;
    }
    void draw(sf::RenderTarget& render_target) const override
    {
        sf::RectangleShape rectangle;
        rectangle.setPosition(sf::Vector2f(pos.x, pos.y));
        rectangle.setSize(sf::Vector2f(16, 16));
        render_target.draw(rectangle);
    }
};

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    
    sf::RenderWindow window(sf::VideoMode(640, 480), "Project", sf::Style::Close, contextSettings);
    window.setVerticalSyncEnabled(true);
    window.setActive();
    
    od::InstanceContext<sfObject> ic;
    od::instanceCreate<Player>(64, 64);
    
    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if((event.type == sf::Event::Closed)
            ||((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
                window.close();
        }
        
        window.clear(sf::Color(150, 150, 150, 0));
        
        ic.update(1.f);
        ic.render(true, window);
        
        window.display();
    }
    
    return 0;
}
```


### Instance lifetime

Once an instance was created, it is very simple to destroy it. You can do so in two ways:
- instanceDestroy() that will call the eventDestroyed() event and destroy the instance.
- instanceKill() that will destroy the instance without calling any event.

If you need to know if an instance still exists or if it has been destroyed, you need to keep std::weak_ptr of it:
```c++
std::weak_ptr<Object> ins = od::instanceCreateWeak<Object>();
...
ins.lock()->instanceDestroy();
...
if(!ins.expired())
{
    ins.lock()->foo();
}
```

You can get a std::weak_ptr from an Object*, for this you need to use the shared() method:
```c++
void Foo::bar(Enemy* obj)
{
    _target = obj->shared();
}
```

Caution though; this method has a linear complexity according to the count of instances of the object.


### Collision detection

Once your entities are implemented, and that you gave them a collision mask, you easily can make them interact. Some functions allows you to detect easily collisions between two instances.

Example 1 - Detect if there is a collision with an instance of a specific object
```c++
if(keyCheck(vk_right) && !placeMeeting<Block>(pos.x+1, pos.y))
{
    pos.x++;
}
```

Exemple 2 - Detect if there is a collision with an instance of different objects
```c++
if(keyCheck(vk_right) && !placeMeeting<Block, Ground, Wall>(pos.x+1, pos.y))
{
    pos.x++;
}
```

Exemple 3 - Get a pointer of the colliding instance
```c++
od::Object2d* ins = instancePosition<Bonus>(pos.x, pos.y);
if(ins)
{
    Bonus* bonus = static_cast<Bonus*>(ins);
    getPoints(bonus->points());
    bonus->instanceDestroy();
}
```


### Space partitionning

You easily can assign a space partition system to a specific object type:
```c++
od::InstanceContext<od::Object2d> ic;
ic.setSpacePartitioning<Block, od::CollisionGrid>(128, 128, world_width/128+1, world_height/128+1);
```

The objects will be automatically added to the partition system when created, and removed from those when they're destroyed.
Warning though, if you move an instance, you will need to update it manually in the partition system.
It's recommended to use a partition system for static objects for optimization purposes.

Once the partition system is defined for an object, collision functions will use it automatically without having to do it manually.
You also can get instances from a specific zone easily using:
```c++
auto t = ic.retrieveInstances<Block>(x1, y1, x2, y2, od::CollisionGrid::Intersection);
for(Block* ii: t)
{
    ii->foo();
}
```

This can be very useful to show instances only available in the view for example, without having to handle all instances of the context.

### Draw priorities and draw layers

You can assign to every object (don't mix this up with an instance) a draw priority.
If you use render() without specifying anything, the instances will be automatically drawed using a draw order.
You can also can also draw instances with a given priority. It allows you to handle rendering a more manual way, and can be useful to set the draw depth of different objects.
You won't be able to handle the draw priority a local way (for specific instances) with this system.

Example - Set the draw priority of the object Entity:
```c++
class Entity:
    public od::AutoIndex<od::Object2d, Entity, 10>
{
public:
    Entity(float x, float y):
        od::AutoIndex<od::Object2d, Entity>(x, y) {}
};
```

Example - Draw instances of a given draw priority interval:
```c++
ic.renderLayers(5, 10);
```

Nb: a higher draw priority makes a object show above others.

### Using UIDS

UIDs or Unique Identifiers are useful when you need to add multiplayer in your game, to identify objects from a distant client easily, and to sync them to the server.

By default UIDs are deactivated for all objects. To enable the UID system for an object you have to implement the 3 following methods:
- generateUid() which allows to tell if we use the UID system
- setUid() to set the instance's UID
- uid() to get the instance's UID

Here is an entity example using the UID system:
```c++
class Entity:
    public od::AutoIndex<od::Object2d, Entity>
{
    unsigned _uid = 0;
public:
    Entity(float x, float y):
        od::AutoIndex<od::Object2d, Entity>(x, y) {}
    
    bool generateUid() const override { return true; }
    void setUid(unsigned uid) override { _uid = uid; }
    unsigned uid() const { return _uid; }
};
```

If you create an instance serverside, you need to send the UID through the network to create the same entity clientside (which implies creating an instance with the same UID).
Here is how to proceed:

Serverside:
```c++
Entity* ins = od::instanceCreate<Entity>();
Packet net_packet;
net_packet << NetMessage::CreateEntity << ins->uid();
client.sendPacket(net_packet);
```
Clientside:
```c++
if(message == NetMessage::CreateEntity)
{
    unsigned uid;
    net_packet >> uid;
    od::instanceCreateUid<Entity>(uid);
}
```

Then you easily can identify this instance through the network.

Serverside:
```c++
Packet net_packet;
net_packet << NetMessage::UpdateEntity << ins->uid();
ins->evSend(net_packet);
client.sendPacket(net_packet);
```

Clientside:
```c++
if(message == NetMessage::UpdateEntity)
{
    unsigned uid;
    net_packet >> uid;
    od::Object2d* ins = ic.find(Entity::ObjectIndex, uid);
    if(ins)
    {
        ins->evReceive(net_packet);
    }
}
```
