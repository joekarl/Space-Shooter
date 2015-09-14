//
//  EnemySpawningSystem.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 9/9/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef EnemySpawningSystem_h
#define EnemySpawningSystem_h

#define ENEMY_SPAWN_RATE 1000

template <typename... EntityManagerTypes>
class EnemySpawningSystem {
private:
    EntityManager<EntityManagerTypes...> *entityManagerRef;
    
    size_t activeEnemy = -1;
    
    void createExplosion(const TransformComponent &transformComponent) {
        
    }
    
public:
    void init(EntityManager<EntityManagerTypes...> *entityManagerRef) {
        this->entityManagerRef = entityManagerRef;
        entityManagerRef->addDeadEntityListener([this](size_t deadEntity){
            this->onDeadEntity(deadEntity);
        });
    }
    
    void onDeadEntity(size_t deadEntity) {
        if (deadEntity == activeEnemy) {
            activeEnemy = -1;
        }
    }
    
    void update(double dt) {
        
        if (activeEnemy == -1) {
            
            
            
            // spawn an enemy
            auto &enemy = entityManagerRef->createEntity();
            SpriteRenderComponent spriteComponent;
            spriteComponent.sprite = SpriteType::ENEMY_SPACESHIP;
            spriteComponent.width = 32;
            spriteComponent.height = 32;
            TransformComponent transformComponent;
            transformComponent.x = SCREEN_WIDTH - 60;
            transformComponent.y = HALF_SCREEN_HEIGHT;
            transformComponent.dx = -0.2;
            transformComponent.dy = 0;
            transformComponent.rotationAngle = M_PI / 180.0 * 180;
            DiesWhenOffscreenComponent diesWhenOffscreenComponent;
            AABBComponent bounds;
            bounds.width = 32;
            bounds.height = 32;
            CollisionComponent collisionComponent([&](size_t owningEntity, size_t otherEntity){
                auto &enemy = entityManagerRef->getEntity(owningEntity);
                auto &other = entityManagerRef->getEntity(otherEntity);
                if (other.matchesMask(1 << getTypeId<LaserDetailsComponent>())) {
                    enemy.kill();
                    other.kill();
                    
                    auto &transformComponent = enemy.template getComponent<TransformComponent>();
                    
                    auto &explosion = entityManagerRef->createEntity();
                    SpriteRenderComponent explosionSpriteComponent;
                    explosionSpriteComponent.sprite = SpriteType::EXPLOSION;
                    explosionSpriteComponent.width = 32;
                    explosionSpriteComponent.height = 32;
                    explosionSpriteComponent.frameDuration = 75;
                    explosionSpriteComponent.numberOfFrames = 5;
                    explosionSpriteComponent.loop = false;
                    DiesAfterTimePeriodComponent deathTimeoutComponent;
                    deathTimeoutComponent.deathTimeout = 500;
                    TransformComponent explosionTransform;
                    explosionTransform.x = transformComponent.x;
                    explosionTransform.y = transformComponent.y;
                    explosionTransform.rotationAngle = 0;
                    explosion.addComponent(explosionSpriteComponent);
                    explosion.addComponent(deathTimeoutComponent);
                    explosion.addComponent(explosionTransform);
                    
                    printf("Spawned an explosion with id %zu\n", explosion.getId());
                }
            });
            EnemyAIComponent aiComponent;
            enemy.addComponent(aiComponent);
            enemy.addComponent(collisionComponent);
            enemy.addComponent(bounds);
            enemy.addComponent(spriteComponent);
            enemy.addComponent(transformComponent);
            enemy.addComponent(diesWhenOffscreenComponent);
            
            activeEnemy = enemy.getId();
            
            printf("Spawned an enemy with id %zu\n", activeEnemy);
        }
    }
};

#endif /* EnemySpawningSystem_h */
