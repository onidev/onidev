
## Utilisation de l'instance manager

L'instance manager permet de:
- contenir des instances d'objets differents qui dérivent du même objet
- lancer leur event d'update et d'affichage dans la boucle principale automatiquement
- obtenir des informations sur les instances, savoir si une instance existe
- gérer les collisions entre instances
- permettre de partitionner l'espace pour optimiser la detection des collisions
- permettre de gérer des collisions entre instances et calques de collision

### Creation d'un contexte d'instance

Un contexte d'instances contiendra les instances crées après qu'il ai été définis
comme le contexte global.
A sa création un contexte est automatiquement passé comme contexte global.

```c++
od::InstanceContext<Object> ic;
od::instanceCreate<Foo>();
```

Utiliser plusieurs contextes est rarement utile, mais cela permet de faire tourner
plusieurs "mondes" en parallèle sans que les instances de chacun d'eux n'influent
sur celles des autres.
Cela peut être pratique pour faire un mini jeu dans un jeu, ou un jeu
qui se joue dans plusieurs dimensions simultannément par exemple.

```c++
od::InstanceContext<Object> context1;
od::InstanceContext<Object> context2;

context1.setGlobal();
od::instanceCreate<Foo>();
od::instanceCreate<Bar>();

context2.setGlobal();
od::instanceCreate<Bar>();
```

Si vous ne créez pas de contexte, un contexte par defaut sera accessible.
Vous pourrez y accèder avec:

```c++
auto& context = od::InstanceContext<Object>::getDefault();
```

Une fois le contexte crée vous pouvez donc update les instances qu'il contient, et les afficher.
```c++
od::InstanceContext<Object> ic;
od::instanceCreate<Foo>();

while(mainLoop())
{
    ic.update( deltaTime() );
    ic.render();
}
```

### Implementation d'entités

Lorsque vous implementez une entité, vous devez la faire dériver soit:
- de od::Object2d 
- de od::GenericObject2d<T> si od::Object2d ne vous conviens pas en tant que tel

Chaque entité doit posséder un identifiant unique. Le faire manuellement étant une tache très contraignante, il existe
une classe prévue a cet effet.

```c++
class Entity:
    public od::AutoIndex<od::Object2d, Entity>
{
public:
    Entity(float x, float y):
        od::AutoIndex<od::Object2d, Entity>(x, y) {}
};
```

od::AutoIndex attends 3 paramètres templates dont 1 facultatif:
- Le type dont on doit dériver
- Le type de l'objet dérivé
- Un entier qui définis la priorité d'affichage de l'objet

Ainsi pour dériver un objet vous devez proceder ainsi:
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

De nombreuses methodes peut être redéfinies, les plus importantes étants:
- collisionMask() qui définis le masque de collision de l'instance
- step() qui sera appellé par InstanceContexte::update()
- draw() qui sera appellé par InstanceContexte::render()

Voici un exemple complet:
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

### Implementation d'un type d'entité custom

Dans certains cas il peut être necessaire d'implementer un type custom d'entité, par exemple si vous souhaitez intégrer de nouveaux events ou methodes virtuelles. Il est aussi possible de créer un event draw qui prend des paramètres customs, cela peut être très utile pour certaines bibliothèques qui ont besoin d'un accès a un RenderTarget.

Pour cela il suffit juste de faire dériver votre classe de base de od::GenericObject2d<T> comme suit:
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

GlobalCounter est nécessaire pour le fonctionnement de od::AutoIndex. N'oubliez pas son implementation dans le .cpp associé:
```c++
size_t sfObject::GlobalCounter = 0;
```

Vous pouvez desormais utiliser sfObject comme classe de base pour toutes vos entités:
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

### Durée de vie d'une instance

Une fois une instance crée, il est très simple de la détruire. Pour cela vous avez 2 méthodes:
- instanceDestroy() qui va appeller l'event eventDestroyed() et détruire l'instance
- instanceKill() qui va détruire l'instance sans appeller aucun event

Si vous avez besoin de savoir si une instance existe toujours ou a été détruite, vous devez garder un std::weak_ptr d'elle:
```c++
std::weak_ptr<Object> ins = od::instanceCreateWeak<Object>();
...
ins.lock()->instanceDestroy();
...
if(!ins.expired())
{
    auto obj = ins.lock();
    obj->foo();
}
```


### Détection des collisions

Une fois vos entités implémentées, et que vous leur avez définis un masque de collision, vous pouvez très facilement les faire interagir entre elles. Plusieurs fonctions permettent de detecter simplement s'il y a collision entre une instance et une autre.

Exemple 1 - Detecter s'il y a collision avec une instance d'un objet particulier:
```c++
if(keyCheck(vk_right) && !placeMeeting<Block>(pos.x+1, pos.y))
{
    pos.x++;
}
```

Exemple 2 - Detecter s'il y a collision avec une instance de plusieurs types d'objets
```c++
if(keyCheck(vk_right) && !placeMeeting<Block, Ground, Wall>(pos.x+1, pos.y))
{
    pos.x++;
}
```

Exemple 3 - Obtenir un pointeur de l'instance en collision
```c++
od::Object2d* ins = instancePosition<Bonus>(pos.x, pos.y);
if(ins)
{
    Bonus* bonus = static_cast<Bonus*>(ins);
    getPoints(bonus->points());
    bonus->instanceDestroy();
}
```

### Partitionnement de l'espace

### Priorités d'affichage et calques de rendu




