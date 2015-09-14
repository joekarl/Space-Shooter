//
//  PlayerMovementSystem.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/28/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef PlayerMovementSystem_hpp
#define PlayerMovementSystem_hpp

#include "GameTypes.hpp"
#include "ComponentTypes.hpp"

#define SHIP_SPEED 0.4
#define FIRE_INTERVAL 100

template <typename... EntityManagerTypes>
class PlayerAISystem {
private:
    EntityManager<EntityManagerTypes...> *entityManagerRef;
    
    int componentMask = 0;
    
public:
    void init(EntityManager<EntityManagerTypes...> *entityManagerRef) {
        this->entityManagerRef = entityManagerRef;
        
        componentMask |= 1 << getTypeId<TransformComponent>();
        componentMask |= 1 << getTypeId<PlayerDetailsComponent>();
        componentMask |= 1 << getTypeId<AABBComponent>();
    }
    
    void updatePosition(double dt, double gameTime, GameInput &input) {
        entityManagerRef->visitEntitiesWithTypeMask(componentMask, [&](Entity<EntityManagerTypes...> &entity){
            
            auto &transformComponent = entity.template getComponent<TransformComponent>();
            auto &playerDetailsComponent = entity.template getComponent<PlayerDetailsComponent>();
            auto &aabbComponent = entity.template getComponent<AABBComponent>();
            
            if (input.keys[VK_LEFT] == KS_DOWN && input.keys[VK_RIGHT] == KS_DOWN) {
                transformComponent.dx = 0;
            } else if (input.keys[VK_LEFT] == KS_DOWN) {
                transformComponent.dx = -1;
            } else if (input.keys[VK_RIGHT] == KS_DOWN) {
                transformComponent.dx = 1;
            } else if (input.keys[VK_LEFT] == KS_UP && input.keys[VK_LEFT] == KS_UP) {
                transformComponent.dx = 0;
            }
            
            if (input.keys[VK_DOWN] == KS_DOWN && input.keys[VK_UP] == KS_DOWN) {
                transformComponent.dy = 0;
            } else if (input.keys[VK_DOWN] == KS_DOWN) {
                transformComponent.dy = -1;
            } else if (input.keys[VK_UP] == KS_DOWN) {
                transformComponent.dy = 1;
            } else if (input.keys[VK_DOWN] == KS_UP && input.keys[VK_UP] == KS_UP) {
                transformComponent.dy = 0;
            }
            
            if (transformComponent.dx != 0 && transformComponent.dy != 0) {
                // moving diagonally, must normalize movement
                auto length = sqrtf(transformComponent.dx * transformComponent.dx + transformComponent.dy * transformComponent.dy);
                if (length >= 0) {
                    transformComponent.dx /= length;
                    transformComponent.dy /= length;
                }
            }
            
            transformComponent.dx *= SHIP_SPEED;
            transformComponent.dy *= SHIP_SPEED;
            
            transformComponent.x += transformComponent.dx * dt;
            transformComponent.y += transformComponent.dy * dt;
            transformComponent.z += transformComponent.dz * dt;
            
            auto halfWidth = aabbComponent.width / 2.0;
            auto halfHeight = aabbComponent.height / 2.0;
            
            if (transformComponent.x - halfWidth < 0) {
                transformComponent.x = halfWidth;
            } else if (transformComponent.x + halfWidth > SCREEN_WIDTH) {
                transformComponent.x = SCREEN_WIDTH - halfWidth;
            }
            if (transformComponent.y - halfHeight < 0) {
                transformComponent.y = halfHeight;
            } else if (transformComponent.y + halfHeight > SCREEN_HEIGHT) {
                transformComponent.y = SCREEN_HEIGHT - halfHeight;
            }
            
            if (input.keys[VK_SPACE] == KS_DOWN &&
                (gameTime - playerDetailsComponent.lastFireTime) > FIRE_INTERVAL) {
                
                playerDetailsComponent.lastFireTime = gameTime;
                
                auto &laser = entityManagerRef->createEntity();
                SpriteRenderComponent laserSpriteComponent;
                switch (playerDetailsComponent.laserType) {
                    case RED:
                        laserSpriteComponent.sprite = SpriteType::RED_LASER;
                        break;
                        
                    case BLUE:
                        laserSpriteComponent.sprite = SpriteType::BLUE_LASER;
                        break;
                        
                    case GREEN:
                        laserSpriteComponent.sprite = SpriteType::GREEN_LASER;
                        break;
                        
                    default:
                        break;
                }
                
                laserSpriteComponent.width = 8;
                laserSpriteComponent.height = 8;
                TransformComponent laserTransformComponent;
                laserTransformComponent.x = transformComponent.x + 40;
                laserTransformComponent.y = transformComponent.y;
                laserTransformComponent.dx = 0.8;
                laserTransformComponent.dy = 0;
                DiesWhenOffscreenComponent diesWhenOffscreenComponent;
                AABBComponent laserBounds;
                laserBounds.width = 8;
                laserBounds.height = 8;
                AutoMovementComponent autoMoveComponent;
                CollisionComponent collisionComponent;
                LaserDetailsComponent laserDetailsComponent;
                laserDetailsComponent.firedByPlayer = true;
                laser.addComponent(laserDetailsComponent);
                laser.addComponent(collisionComponent);
                laser.addComponent(autoMoveComponent);
                laser.addComponent(laserBounds);
                laser.addComponent(laserSpriteComponent);
                laser.addComponent(laserTransformComponent);
                laser.addComponent(diesWhenOffscreenComponent);
                
                printf("Spawned a laser with id %zu\n", laser.getId());
            }
        });
    }
};

#endif /* PlayerMovementSystem_hpp */
