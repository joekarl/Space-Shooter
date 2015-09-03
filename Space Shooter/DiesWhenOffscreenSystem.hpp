//
//  DiesWhenOffscreenSystem.h
//  Space Shooter
//
//  Created by Karl Kirch on 8/28/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef DiesWhenOffscreenSystem_h
#define DiesWhenOffscreenSystem_h

#include "EntityManager.hpp"

template <typename... EntityManagerTypes>
class DiesWhenOffscreenSystem {
private:
    EntityManager<EntityManagerTypes...> *entityManagerRef;
    
    int componentMask = 0;
    
public:
    void init(EntityManager<EntityManagerTypes...> *entityManagerRef) {
        this->entityManagerRef = entityManagerRef;
        
        componentMask |= 1 << getTypeId<DiesWhenOffscreenComponent>();
        componentMask |= 1 << getTypeId<TransformComponent>();
        componentMask |= 1 << getTypeId<AABBComponent>();
    }

    void checkOffscreenPositions() {
        entityManagerRef->visitEntitiesWithTypeMask(componentMask, [](Entity<EntityManagerTypes...> &entity){
            auto &transformComponent = entity.template getComponent<TransformComponent>();
            auto &aabbComponent = entity.template getComponent<AABBComponent>();
            auto &diesWhenOffscreenComponent = entity.template getComponent<DiesWhenOffscreenComponent>();
            
            auto halfWidth = aabbComponent.width / 2.0;
            auto halfHeight = aabbComponent.height / 2.0;
            
            if ((transformComponent.x - halfWidth > SCREEN_WIDTH ||
                 transformComponent.x + halfWidth < 0 ||
                 transformComponent.y - halfHeight > SCREEN_HEIGHT ||
                 transformComponent.y + halfHeight < 0) &&
                diesWhenOffscreenComponent.cameOnscreen) {
                // only kill if we're offscreen and have been onscreen at some point
                entity.kill();
            } else if (!diesWhenOffscreenComponent.cameOnscreen) {
                diesWhenOffscreenComponent.cameOnscreen = true;
            } else if (transformComponent.x > 10000 ||
                       transformComponent.x < -1000 ||
                       transformComponent.y > 10000 ||
                       transformComponent.y < -1000) {
                // just some failsafe stuff here
                entity.kill();
            }
        });
    }
};

#endif /* DiesWhenOffscreenSystem_h */
