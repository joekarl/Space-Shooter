//
//  CollisionComponent.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 9/2/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef CollisionComponent_h
#define CollisionComponent_h

#include <stdio.h>

class CollisionComponent {
private:
    void (*collisionFunction)(size_t owningEntity, size_t otherEntity);
public:
    
    CollisionComponent() {
        this->collisionFunction = [](size_t owningEntity, size_t otherEntity) {
            //printf("unhandled collision occured between %zu and %zu\n", owningEntity, otherEntity);
        };
    }
    
    CollisionComponent(void (*collisionFunction)(size_t owningEntity, size_t otherEntity)) {
        this->collisionFunction = collisionFunction;
    }
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity) {
        
    }
    
    template <typename... EntityComponentTypes>
    void collisionOccured(Entity<EntityComponentTypes...> &owningEntity, Entity<EntityComponentTypes...> &otherEntity) {
        collisionFunction(owningEntity.getId(), otherEntity.getId());
    }
    
};

#endif /* CollisionComponent_h */
