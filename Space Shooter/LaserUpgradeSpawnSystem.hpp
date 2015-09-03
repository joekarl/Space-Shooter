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
#define SPAWN_RATE 10000

template <typename ...EntityManagerTypes>
class LaserUpgradeSpawnSystem {
private:
    EntityManager<EntityManagerTypes...> *entityManagerRef;
    
    double spawnTimer;
    bool firstTime = true;
    
public:
    void init(EntityManager<EntityManagerTypes...> *entityManagerRef) {
        this->entityManagerRef = entityManagerRef;
    }
    
    void update(double dt) {
        spawnTimer += dt;
        
        if (spawnTimer - SPAWN_RATE > 0 || firstTime) {
            spawnTimer = 0;
            firstTime = false;
            
            // spawn a laser powerup
            auto &powerup = entityManagerRef->createEntity();
            SpriteRenderComponent spriteComponent;
            spriteComponent.sprite = SpriteType::RED_POWERUP;
            spriteComponent.scaleX = 64.0 / SCREEN_WIDTH / 2.0;
            spriteComponent.scaleY = 64.0 / SCREEN_HEIGHT / 2.0;
            TransformComponent transformComponent;
            transformComponent.x = SCREEN_WIDTH;
            transformComponent.y = SCREEN_HEIGHT / 2.0;
            transformComponent.dx = -0.2;
            transformComponent.dy = 0;
            DiesWhenOffscreenComponent diesWhenOffscreenComponent;
            AABBComponent bounds;
            bounds.width = 32.0;
            bounds.height = 32.0;
            AutoMovementComponent autoMoveComponent;
            CollisionComponent collisionComponent;
            LaserUpgradeComponent laserUpgradeComponent;
            powerup.addComponent(laserUpgradeComponent);
            powerup.addComponent(collisionComponent);
            powerup.addComponent(autoMoveComponent);
            powerup.addComponent(bounds);
            powerup.addComponent(spriteComponent);
            powerup.addComponent(transformComponent);
            powerup.addComponent(diesWhenOffscreenComponent);
            
        }
    }
};

#endif /* LaserUpgradeSpawnSystem_h */
