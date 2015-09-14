//
//  GameManager.cpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/21/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#include "GameManager.hpp"

#include <stdlib.h>
#include <boost/variant.hpp>

#include "ComponentTypes.hpp"
#include "GlobalDefines.hpp"
#include "GlUtils.hpp"
#include "Shaders.hpp"
#include "GlSprite.hpp"
#include "EntityManager.hpp"
#include "SpriteRenderSystem.hpp"
#include "PlayerAISystem.hpp"
#include "AutoMovementSystem.hpp"
#include "DiesWhenOffscreenSystem.hpp"
#include "LaserUpgradeSpawnSystem.hpp"
#include "CollisionSystem.hpp"
#include "EnemySpawningSystem.hpp"
#include "DebugCollisionRenderSystem.hpp"
#include "DiesAfterTimePeriodSystem.hpp"
#include "SpriteAnimationSystem.hpp"
#include "EnemyAISystem.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define COMPONENT_TYPES TransformComponent, SpriteRenderComponent, PlayerDetailsComponent, DiesWhenOffscreenComponent, AABBComponent, AutoMovementComponent, CollisionComponent, LaserUpgradeComponent, LaserDetailsComponent, DiesAfterTimePeriodComponent, EnemyAIComponent

EntityManager<COMPONENT_TYPES> entityManager;
SpriteRenderSystem<COMPONENT_TYPES> renderSystem;
PlayerAISystem<COMPONENT_TYPES> playerAISystem;
AutoMovementSystem<COMPONENT_TYPES> autoMovementSystem;
DiesWhenOffscreenSystem<COMPONENT_TYPES> diesWhenOffscreenSystem;
LaserUpgradeSpawnSystem<COMPONENT_TYPES> laserUpgradeSpawnSystem;
CollisionSystem<COMPONENT_TYPES> collisionSystem;
EnemySpawningSystem<COMPONENT_TYPES> enemySpawningSystem;
DebugCollisionRenderSystem<COMPONENT_TYPES> debugCollisionRenderSystem;
DiesAfterTimePeriodSystem<COMPONENT_TYPES> diesAfterTimePeriodSystem;
SpriteAnimationSystem<COMPONENT_TYPES> spriteAnimationSystem;
EnemyAISystem<COMPONENT_TYPES> enemyAISystem;

double gameTime = 0;
bool initialized = false;

void GameManager::init(std::string resourcesPath) {
    
    renderSystem.init(&entityManager, resourcesPath);
    playerAISystem.init(&entityManager);
    autoMovementSystem.init(&entityManager);
    diesWhenOffscreenSystem.init(&entityManager);
    laserUpgradeSpawnSystem.init(&entityManager);
    collisionSystem.init(&entityManager);
    enemySpawningSystem.init(&entityManager);
    debugCollisionRenderSystem.init(&entityManager, resourcesPath);
    diesAfterTimePeriodSystem.init(&entityManager);
    spriteAnimationSystem.init(&entityManager);
    enemyAISystem.init(&entityManager);
    
    auto &shipEntity = entityManager.createEntity();
    TransformComponent shipTransformComponent;
    shipTransformComponent.x = 90;
    shipTransformComponent.y = HALF_SCREEN_HEIGHT;
    AABBComponent playerBounds;
    playerBounds.width = 32;
    playerBounds.height = 32;
    SpriteRenderComponent shipSpriteComponent;
    shipSpriteComponent.sprite = SpriteType::SPACESHIP;
    shipSpriteComponent.width = playerBounds.width;
    shipSpriteComponent.height = playerBounds.height;
    PlayerDetailsComponent playerComponent;
    CollisionComponent collisionComponent([](size_t owningEntity, size_t otherEntity){
        auto &ship = entityManager.getEntity(owningEntity);
        auto &other = entityManager.getEntity(otherEntity);
        if (other.matchesMask(1 << getTypeId<LaserUpgradeComponent>())) {
            // it's an upgrade
            other.kill();
            auto &otherTransform = other.template getComponent<TransformComponent>();
            printf("Ship collided with an upgrade at X: %f Y: %f\n", otherTransform.x, otherTransform.y);
            auto &laserUpgrade = other.template getComponent<LaserUpgradeComponent>();
            auto &playerDetails = ship.template getComponent<PlayerDetailsComponent>();
            
            switch (laserUpgrade.upgradeType) {
                case RED:
                    playerDetails.laserType = RED;
                    break;
                    
                case BLUE:
                    playerDetails.laserType = BLUE;
                    break;
                    
                case GREEN:
                    playerDetails.laserType = GREEN;
                    break;
                    
                default:
                    break;
            }
        }
        
    });
    shipEntity.addComponent(playerBounds);
    shipEntity.addComponent(shipTransformComponent);
    shipEntity.addComponent(shipSpriteComponent);
    shipEntity.addComponent(playerComponent);
    shipEntity.addComponent(collisionComponent);
    
    printf("Spawned a player ship with id %zu\n", shipEntity.getId());
    
    // must clean entities here before update loop is run the first time...
    // why you ask? because the the ship entity hasn't been initialized we'll overwrite it's innards :/
    entityManager.cleanEntities();
    
    // setup defaults
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    initialized = true;
}

void GameManager::update(double dt, GameInput &input) {
    
// debug keystate
//    for (int i = 0; i < 255; ++i) {
//        if (input.keys[i] == KS_DOWN) {
//            printf("key %d\n", i);
//        }
//    }
    
    gameTime += dt;
    
    // update player position
    playerAISystem.updatePosition(dt, gameTime, input);
    
    enemyAISystem.update(dt);
    
    // update dumb entities
    autoMovementSystem.moveEntities(dt);
    
    enemySpawningSystem.update(dt);
    
    // update entities that should die when the go offscreen
    diesWhenOffscreenSystem.checkOffscreenPositions();
    
    diesAfterTimePeriodSystem.update(dt);
    
    laserUpgradeSpawnSystem.update(dt);
    
    collisionSystem.checkCollisions(dt);
    
    spriteAnimationSystem.update(dt);
    
    entityManager.cleanEntities();
}

void GameManager::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (!initialized) {
        return;
    }
    
    renderSystem.renderSprites();

    //debugCollisionRenderSystem.renderSprites();
}