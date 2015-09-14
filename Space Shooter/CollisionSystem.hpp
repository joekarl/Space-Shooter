//
//  CollisionSystem.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 9/2/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef CollisionSystem_h
#define CollisionSystem_h

#include "EntityManager.hpp"
#include <tuple>

template <typename... EntityManagerTypes>
class CollisionSystem {
private:
    EntityManager<EntityManagerTypes...> *entityManagerRef;
    
    int componentMask = 0;
    
public:
    void init(EntityManager<EntityManagerTypes...> *entityManagerRef) {
        this->entityManagerRef = entityManagerRef;
        
        componentMask |= 1 << getTypeId<TransformComponent>();
        componentMask |= 1 << getTypeId<AABBComponent>();
        componentMask |= 1 << getTypeId<CollisionComponent>();
    }

    void checkCollisions(double dt) {
        
        auto collideableEntities = entityManagerRef->getEntityIDsWithTypeMask(componentMask);
        auto entityNum = collideableEntities.size();
        
        for (int i = 0; i < entityNum; ++i) {
            for (int j = i; j < entityNum; ++j) {
                auto entityId1 = collideableEntities[i];
                auto entityId2 = collideableEntities[j];
                
                if (entityId1 == entityId2) {
                    continue;
                }
                
                auto &entity1 = entityManagerRef->getEntity(entityId1);
                auto &entity2 = entityManagerRef->getEntity(entityId2);
                
                // check if aabb components overlapf
                auto &entity1AABB = entity1.template getComponent<AABBComponent>();
                auto &entity2AABB = entity2.template getComponent<AABBComponent>();
                auto &entity1Transform = entity1.template getComponent<TransformComponent>();
                auto &entity2Transform = entity2.template getComponent<TransformComponent>();
                
                auto entity1HalfWidth = entity1AABB.width / 2.0;
                auto entity1HalfHeight = entity1AABB.height / 2.0;
                auto entity2HalfWidth = entity2AABB.width / 2.0;
                auto entity2HalfHeight = entity2AABB.height / 2.0;
                
                auto entity1Left = entity1Transform.x - entity1HalfWidth;
                auto entity1Right = entity1Transform.x + entity1HalfWidth;
                auto entity1Bottom = entity1Transform.y - entity1HalfHeight;
                auto entity1Top = entity1Transform.y + entity1HalfHeight;
                
                auto entity2Left = entity2Transform.x - entity2HalfWidth;
                auto entity2Right = entity2Transform.x + entity2HalfWidth;
                auto entity2Bottom = entity2Transform.y - entity2HalfHeight;
                auto entity2Top = entity2Transform.y + entity2HalfHeight;
                
                bool collides = !(entity2Left > entity1Right ||
                                  entity2Right < entity1Left ||
                                  entity2Top < entity1Bottom ||
                                  entity2Bottom > entity1Top);
                
                if (collides) {
                    // always get entity ref here because it could be borked by the collision
                    entityManagerRef->getEntity(entityId1).template getComponent<CollisionComponent>().collisionOccured(entityId1, entityId2);
                    entityManagerRef->getEntity(entityId2).template getComponent<CollisionComponent>().collisionOccured(entityId2, entityId1);
                }
            }
        }
    }
};

#endif /* CollisionSystem_h */
