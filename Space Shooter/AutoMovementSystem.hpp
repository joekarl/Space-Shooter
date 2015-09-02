//
//  MovementSystem.h
//  Space Shooter
//
//  Created by Karl Kirch on 8/28/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef MovementSystem_h
#define MovementSystem_h

#include "EntityManager.hpp"
#define OFFSCREEN_PADDING 20.0

template <typename... EntityManagerTypes>
class AutoMovementSystem {
private:
    EntityManager<EntityManagerTypes...> *entityManagerRef;
    
    int componentMask = 0;
    
public:
    void init(EntityManager<EntityManagerTypes...> *entityManagerRef) {
        this->entityManagerRef = entityManagerRef;
        
        componentMask |= 1 << getTypeId<TransformComponent>();
        componentMask |= 1 << getTypeId<AutoMovementComponent>();
    }

    void moveEntities(double dt) {
        entityManagerRef->findEntitiesWithTypeMask(componentMask, [&](Entity<EntityManagerTypes...> &entity){
            auto &transformComponent = entity.template getComponent<TransformComponent>();
            
            transformComponent.x += transformComponent.dx * dt;
            transformComponent.y += transformComponent.dy * dt;
            transformComponent.z += transformComponent.dz * dt;
        });
    }
};

#endif /* MovementSystem_h */
