//
//  DiesAfterTimePeriodSystem.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 9/13/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef DiesAfterTimePeriodSystem_h
#define DiesAfterTimePeriodSystem_h

#include "EntityManager.hpp"

template <typename... EntityManagerTypes>
class DiesAfterTimePeriodSystem {
private:
    EntityManager<EntityManagerTypes...> *entityManagerRef;
    
    int componentMask = 0;
    
public:
    void init(EntityManager<EntityManagerTypes...> *entityManagerRef) {
        this->entityManagerRef = entityManagerRef;
        
        componentMask |= 1 << getTypeId<DiesAfterTimePeriodComponent>();
    }
    
    void update(double dt) {
        entityManagerRef->visitEntitiesWithTypeMask(componentMask, [&](Entity<EntityManagerTypes...> &entity){
            auto &component = entity.template getComponent<DiesAfterTimePeriodComponent>();
            
            component.deathTimeout -= dt;
            
            if (component.deathTimeout < 0) {
                entity.kill();
            }
        });
    }
    
};

#endif /* DiesAfterTimePeriodSystem_h */
