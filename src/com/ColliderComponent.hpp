#pragma once

#include "Component.hpp"

#include <tools/BoundingBox.hpp>
#include <enum/ColliderType.hpp>
#include <vector>

class ColliderComponent : public Component {
public:
    explicit ColliderComponent(const int id);
    ~ColliderComponent() override;

    // Estas son las capas a las que se colisiona con cada tipo de objetos, luego, al crear una entidad tendremos que poner a 1 todas las capas con las que queremos que colisione (layerMasc indica con que CollisionLayr va a colisionar este objeto)
    //  Ejemplo: Enemigo colisiona muro (0x01) y player (0x02) -> enemigo layermasc = (0x01 + 0x02 = 0x03)
    enum
    {
        NoLayer       = 0x00,
        Wall          = 0x01,
        Player        = 0x02, 
        PlayerAttack  = 0x04,
        Attack        = 0x08,
        Enemy         = 0x10,
        Weapon        = 0x20
    };


    uint8_t collisionLayer{ NoLayer }; // Este siempre tiene que ser con un tipo de collisionLayer
    uint8_t layerMasc{0xFF}; // Este es libre, es con que collisionLayers va a colisionar

    ColliderType type{ ColliderType::STATIC };
    //Puedo hacer collidable type si necesito: Dinamic, kinematic, static

    bool collide{ false };

    BoundingBoxNode boundingRoot;
};