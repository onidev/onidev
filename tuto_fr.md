
## Utilisation de l'instance manager

L'instance manager permet de:
- contenir des instances d'objets différents qui dérivent du même objet
- lancer leur event d'update et d'affichage dans la boucle principale automatiquement
- obtenir des informations sur les instances, savoir si une instance existe
- gérer les collisions entre instances
- partitionner l'espace pour optimiser la détection des collisions
- gérer des collisions entre instances et calques de collision


### Création d'un contexte d'instance

Un contexte d'instances contiendra les instances créées après qu'il ait été défini
comme le contexte global.
À sa création un contexte est automatiquement passé comme contexte global.

```c++
od::InstanceContext<Object> ic;
od::instanceCreate<Foo>();
```

Utiliser plusieurs contextes est rarement utile, mais cela permet de faire tourner
plusieurs "mondes" en parallèle sans que les instances de chacun d'eux n'influent
sur celles des autres.
Cela peut être pratique pour faire un mini-jeu dans un jeu, ou un jeu
qui se joue dans plusieurs dimensions simultanément par exemple.

```c++
od::InstanceContext<Object> context1;
od::InstanceContext<Object> context2;

context1.setGlobal();
od::instanceCreate<Foo>();
od::instanceCreate<Bar>();

context2.setGlobal();
od::instanceCreate<Bar>();
```

Si vous ne créez pas de contexte, un contexte par défaut sera accessible.
Vous pourrez y accéder avec:

```c++
auto& context = od::InstanceContext<Object>::getDefault();
```

Une fois le contexte créé vous pouvez donc update les instances qu'il contient, et les afficher.
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

Lorsque vous implémentez une entité, vous devez la faire dériver soit:
- de od::Object2d 
- de od::GenericObject2d<T> si od::Object2d ne vous conviens pas en tant que tel

Chaque entité doit posséder un identifiant unique. Le faire manuellement étant une tache très contraignante, il existe
une classe prévue à cet effet.

```c++
class Entity:
    public od::AutoIndex<od::Object2d, Entity>
{
public:
    Entity(float x, float y):
        od::AutoIndex<od::Object2d, Entity>(x, y) {}
};
```

od::AutoIndex attend 3 paramètres templates dont 1 facultatif:
- Le type dont on doit dériver
- Le type de l'objet dérivé
- Un entier qui définit la priorité d'affichage de l'objet

Ainsi pour dériver un objet vous devez procéder ainsi:
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

De nombreuses méthodes peuvent être redéfinies, les plus importantes étant:
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

Dans certains cas il peut être nécessaire d'implementer un type custom d'entité, par exemple si vous souhaitez intégrer de nouveaux events ou méthodes virtuelles. Il est aussi possible de créer un event draw qui prend des paramètres customs, cela peut être très utile pour certaines bibliothèques qui ont besoin d'un accès à un RenderTarget.

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

Vous pouvez désormais utiliser sfObject comme classe de base pour toutes vos entités:
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

Une fois une instance créée, il est très simple de la détruire. Pour cela vous avez 2 méthodes:
- instanceDestroy() qui va appeler l'event eventDestroyed() et détruire l'instance
- instanceKill() qui va détruire l'instance sans appeler aucun event

Si vous avez besoin de savoir si une instance existe toujours ou a été détruite, vous devez garder un std::weak_ptr d'elle:
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

Vous pouvez obtenir un std::weak_ptr depuis un Object*, pour cela vous devrez utiliser la méthode shared():
```c++
void Foo::bar(Enemy* obj)
{
    _target = obj->shared();
}
```

Attention cependant à ne pas abuser de cette méthode, qui a une complexité linéaire en fonction du nombre d'instances
de l'objet.


### Détection des collisions

Une fois vos entités implémentées, et que vous leur avez définis un masque de collision, vous pouvez très facilement les faire interagir entre elles. Plusieurs fonctions permettent de détecter simplement s'il y a collision entre une instance et une autre.

Exemple 1 - Détecter s'il y a collision avec une instance d'un objet particulier:
```c++
if(keyCheck(vk_right) && !placeMeeting<Block>(pos.x+1, pos.y))
{
    pos.x++;
}
```

Exemple 2 - Détecter s'il y a collision avec une instance de plusieurs types d'objets
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

Vous pouvez facilement assigner un système de partitionnement de l'espace à un type d'objet:
```c++
od::InstanceContext<od::Object2d> ic;
ic.setSpacePartitioning<Block, od::CollisionGrid>(128, 128, world_width/128+1, world_height/128+1);
```

Les objets seront automatiquement ajoutés au système de partitionnement à leur création, et supprimés de celui-ci a leur destruction.
Attention cependant, si vous bougez une instance, vous devrez l'update manuellement dans ce dernier.
Il est surtout conseillé d'utiliser un système de partitionnement pour les objets statiques, afin de profiter au maximum des optimisations apportées.


### Priorités d'affichage et calques de rendu

Vous pouvez assigner à chaque object (à ne pas confondre avec instance) une priorité d'affichage.
Cette priorité est aussi appelée calque, car il va être possible d'afficher des objets d'un certain calque, ou encore d'un calque a un autre. Cela permet de gérer le rendu d'une manière un peu plus manuelle, et peut être pratique pour facilement gérer la profondeur d'affichage de divers objets.
Vous ne pourrez par contre pas gérer la priorité d'affichage de manière locale (pour chaque instance) avec ce système.

Exemple - afficher les instances d'un intervalle entre un calque et un autre:
```c++
ic.renderLayers(5, 10);
```

