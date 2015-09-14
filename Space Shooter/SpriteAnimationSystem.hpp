//
//  SpriteAnimationSystem.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 9/13/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef SpriteAnimationSystem_h
#define SpriteAnimationSystem_h

#include "EntityManager.hpp"

template <typename... EntityManagerTypes>
class SpriteAnimationSystem {
private:
    EntityManager<EntityManagerTypes...> *entityManagerRef;
    
    int componentMask = 0;
    
public:
    void init(EntityManager<EntityManagerTypes...> *entityManagerRef) {
        this->entityManagerRef = entityManagerRef;
        
        componentMask |= 1 << getTypeId<SpriteRenderComponent>();
    }
    
    void update(double dt) {
        entityManagerRef->visitEntitiesWithTypeMask(componentMask, [&](Entity<EntityManagerTypes...> &entity){
            auto &component = entity.template getComponent<SpriteRenderComponent>();
            
            component.advanceAnimation(dt);
        });
    }
    
};

#endif /* SpriteAnimationSystem_h */
