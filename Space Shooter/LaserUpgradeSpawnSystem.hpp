//
//  LaserUpgradeSpawnSystem.h
//  Space Shooter
//
//  Created by Karl Kirch on 9/2/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef LaserUpgradeSpawnSystem_h
#define LaserUpgradeSpawnSystem_h

#include <stdlib.h>
#include "EntityManager.hpp"

// 10 seconds
#define SPAWN_RATE 5000

template <typename ...EntityManagerTypes>
class LaserUpgradeSpawnSystem {
private:
    EntityManager<EntityManagerTypes...> *entityManagerRef;
    
    double spawnTimer;
    bool firstTime = true;
    int cycle = 0;
    
public:
    void init(EntityManager<EntityManagerTypes...> *entityManagerRef) {
        this->entityManagerRef = entityManagerRef;
    }
    
    void update(double dt) {
        spawnTimer += dt;
        
        if (spawnTimer - SPAWN_RATE > 0 || firstTime) {
            spawnTimer = 0;
            firstTime = false;
            cycle++;
            cycle %=3;
            
            // spawn a laser powerup
            auto &powerup = entityManagerRef->createEntity();
            
            LaserUpgradeComponent laserUpgradeComponent;
            switch (cycle) {
                case 0:
                    laserUpgradeComponent.upgradeType = RED;
                    break;
                case 1:
                    laserUpgradeComponent.upgradeType = BLUE;
                    break;
                case 2:
                    laserUpgradeComponent.upgradeType = GREEN;
                    break;
                    
                default:
                    break;
            }
            
            SpriteRenderComponent spriteComponent;
            
            switch (laserUpgradeComponent.upgradeType) {
                case RED:
                    spriteComponent.sprite = SpriteType::RED_POWERUP;
                    break;
                    
                case BLUE:
                    spriteComponent.sprite = SpriteType::BLUE_POWERUP;
                    break;
                    
                case GREEN:
                    spriteComponent.sprite = SpriteType::GREEN_POWERUP;
                    break;
                    
                default:
                    break;
            }
            
            spriteComponent.width = 16;
            spriteComponent.height = 16;
            TransformComponent transformComponent;
            transformComponent.x = SCREEN_WIDTH;
            transformComponent.y = SCREEN_HEIGHT / 2.0 + 120;
            transformComponent.dx = -0.2;
            transformComponent.dy = 0;
            DiesWhenOffscreenComponent diesWhenOffscreenComponent;
            AABBComponent bounds;
            bounds.width = 16;
            bounds.height = 16;
            AutoMovementComponent autoMoveComponent;
            CollisionComponent collisionComponent;
            powerup.addComponent(laserUpgradeComponent);
            powerup.addComponent(collisionComponent);
            powerup.addComponent(autoMoveComponent);
            powerup.addComponent(bounds);
            powerup.addComponent(spriteComponent);
            powerup.addComponent(transformComponent);
            powerup.addComponent(diesWhenOffscreenComponent);
            
            printf("Spawned a laser upgrade with id %zu\n", powerup.getId());
        }
    }
};

#endif /* LaserUpgradeSpawnSystem_h */
