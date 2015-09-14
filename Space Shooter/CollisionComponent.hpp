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
    std::function<void (size_t owningEntity, size_t otherEntity)> collisionFunction;
public:
    
    CollisionComponent() {
        this->collisionFunction = [](size_t owningEntity, size_t otherEntity) {
            //printf("unhandled collision occured between %zu and %zu\n", owningEntity, otherEntity);
        };
    }
    
    CollisionComponent(std::function<void (size_t owningEntity, size_t otherEntity)> collisionFunction) {
        this->collisionFunction = collisionFunction;
    }
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity) {
        
    }
    
    template <typename... EntityComponentTypes>
    void collisionOccured(size_t owningEntity, size_t otherEntity) {
        collisionFunction(owningEntity, otherEntity);
    }
    
};

#endif /* CollisionComponent_h */
