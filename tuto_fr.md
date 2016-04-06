
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

Ainsi pour dériver un objet vous devez proceder comme il suit:
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
