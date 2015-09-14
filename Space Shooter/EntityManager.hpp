//
//  EntityManager.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/26/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef EntityManager_hpp
#define EntityManager_hpp

#include "Entity.hpp"
#include <vector>
#include <stdio.h>

/**
 * EntityManager is in charge of a couple of things
 * Needs to manage entities, creation/death
 */
template <typename InitVisitorType, typename... ComponentTypes>
class EntityManager {
private:
    std::vector<Entity<InitVisitorType, ComponentTypes...>> entities;
    std::vector<std::function<void(size_t deadEntity)>> onDeathListeners;
    
public:
    
    /**
     * WARNING - THIS CALL CAN OBLITERATE ANY REFERENCE TO AN ENTITY
     * OR ANY REFERENCE TO A THING IN YOUR ENTITY
     * AFTER CALLING THIS METHOD YOU SHOULD GET YOUR REFERENCES AGAIN
     */
    Entity<InitVisitorType, ComponentTypes...>& createEntity() {
        for (auto &entity : entities) {
            if (entity.isDead() && !entity.isCreating() && !entity.isDying()) {
                entity.setCreating(true);
                return entity;
            }
        }
        
        Entity<InitVisitorType, ComponentTypes...> newEntity;
        newEntity.setId(entities.size());
        entities.push_back(newEntity);
        return entities.back();
    }
    
    template <typename Func>
    void visitEntitiesWithTypeMask(int typeMask, Func func) {
        for (auto &entity : entities) {
            if (entity.isDead()) {
            }
            else {
                if ((entity.getComponentMask() & typeMask) == typeMask) {
                    func(entity);
                }
            }
        }
    }
    
    std::vector<size_t> getEntityIDsWithTypeMask(int typeMask) {
        std::vector<size_t> matchingEntities;
        for (auto &entity : entities) {
            if (entity.isDead()) {
            } else if ((entity.getComponentMask() & typeMask) == typeMask){
                matchingEntities.push_back(entity.getId());
            }
        }
        return matchingEntities;
    }
    
    Entity<InitVisitorType, ComponentTypes...>& getEntity(size_t id) {
        return entities[id];
    }
    
    void cleanEntities() {
        for (auto &entity : entities) {
            if (entity.isDying()) {
                for (auto &deathFn : onDeathListeners) {
                    deathFn(entity.getId());
                }
                entity.reset();
                // TODO:(karl)notify entity id is dead so
                printf("[entity %zu removed]\n", entity.getId());
            }
            
            // check if we should
            if (entity.isCreating()) {
                entity.init();
            }
        }
    }
    
    void addDeadEntityListener(std::function<void(size_t deadEntity)> onDeathFunction) {
        onDeathListeners.push_back(onDeathFunction);
    }
};

#endif /* EntityManager_hpp */
