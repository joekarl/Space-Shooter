//
//  EnemyAISystem.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 9/13/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef EnemyAISystem_h
#define EnemyAISystem_h

#include "EntityManager.hpp"
#include "ComponentTypes.hpp"

template <typename... EntityManagerTypes>
class EnemyAISystem {
private:
    EntityManager<EntityManagerTypes...> *entityManagerRef;
    
    int componentMask = 0;
    
    void updateDumbAI(double dt, Entity<EntityManagerTypes...> &entity) {
        auto &transform = entity.template getComponent<TransformComponent>();
        if (transform.dy == 0) {
            transform.dy = 0.2;
        }
        
        transform.y += transform.dy * dt;
        
        if (transform.y > SCREEN_HEIGHT) {
            transform.dy = -0.2;
        } else if (transform.y < 0) {
            transform.dy = 0.2;
        }
    }
    
    void updateFleeAI(double dt, Entity<EntityManagerTypes...> &entity) {
        auto &transform = entity.template getComponent<TransformComponent>();
        auto &aiComponent = entity.template getComponent<EnemyAIComponent>();
        auto &playerEntity = entityManagerRef->getEntity(aiComponent.playerId);
        auto &playerTransform = playerEntity.template getComponent<TransformComponent>();
        
        
    }
    
    void updateFollowFireAI(double dt, Entity<EntityManagerTypes...> &entity) {
        
    }
    
public:
    void init(EntityManager<EntityManagerTypes...> *entityManagerRef) {
        this->entityManagerRef = entityManagerRef;
        
        componentMask |= 1 << getTypeId<EnemyAIComponent>();
        componentMask |= 1 << getTypeId<TransformComponent>();
        componentMask |= 1 << getTypeId<AABBComponent>();
    }
    
    void update(double dt) {
        entityManagerRef->visitEntitiesWithTypeMask(componentMask, [&](Entity<EntityManagerTypes...> &entity){
            auto &aiComponent = entity.template getComponent<EnemyAIComponent>();
            
            if (aiComponent.playerId == -1) {
                int playerMask = 0;
                playerMask |= 1 << getTypeId<PlayerDetailsComponent>();
                playerMask |= 1 << getTypeId<TransformComponent>();
                auto entities = entityManagerRef->getEntityIDsWithTypeMask(playerMask);
                aiComponent.playerId = entities[0];
            }
            
            switch (aiComponent.aiType) {
                case EnemyAIComponent::AIType::DUMB:
                    updateDumbAI(dt, entity);
                    break;
                case EnemyAIComponent::AIType::FLEE:
                    updateDumbAI(dt, entity);
                    break;
                case EnemyAIComponent::AIType::FOLLOW_FIRE:
                    updateDumbAI(dt, entity);
                    break;
                    
                default:
                    break;
            }
        });
    }
    
};

#endif /* EnemyAISystem_h */
