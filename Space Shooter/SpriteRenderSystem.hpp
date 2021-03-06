//
//  SpriteRenderSystem.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/27/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef SpriteRenderSystem_hpp
#define SpriteRenderSystem_hpp

#include "EntityManager.hpp"
#include <Eigen/Geometry>
#include "GlUtils.hpp"
#include <string>

template <typename... EntityManagerTypes>
class SpriteRenderSystem {
private:
    EntityManager<EntityManagerTypes...> *entityManagerRef;
    GLuint spriteShaderProgramId;
    
    GlSprite shipSprite;
    GlSprite enemyShipSprite;
    GlSprite laserBlueSprite;
    GlSprite laserRedSprite;
    GlSprite laserGreenSprite;
    GlSprite redPowerupSprite;
    GlSprite greenPowerupSprite;
    GlSprite bluePowerupSprite;
    GlSprite explosionSprite;
    
    int componentMask = 0;
    
public:
    void init(EntityManager<EntityManagerTypes...> *entityManagerRef, std::string resourcesPath) {
        this->entityManagerRef = entityManagerRef;
        
        spriteShaderProgramId = compileShaders("sprite shaders", SPRITE_VERTEX_SHADER, SPRITE_FRAGMENT_SHADER);
        
        if (spriteShaderProgramId == -1) {
            // we're boned :/
            printf("OH NOES, SHADERS FAILED TO COMPILE...\n");
            exit(1);
        }
        
        shipSprite.init(spriteShaderProgramId, resourcesPath + "spaceship.png");
        enemyShipSprite.init(spriteShaderProgramId, resourcesPath + "enemySpaceship.png");
        laserBlueSprite.init(spriteShaderProgramId, resourcesPath + "laser.png");
        laserRedSprite.init(spriteShaderProgramId, resourcesPath + "laserRed.png");
        laserGreenSprite.init(spriteShaderProgramId, resourcesPath + "laserGreen.png");
        redPowerupSprite.init(spriteShaderProgramId, resourcesPath + "redPowerup.png");
        greenPowerupSprite.init(spriteShaderProgramId, resourcesPath + "greenPowerup.png");
        bluePowerupSprite.init(spriteShaderProgramId, resourcesPath + "bluePowerup.png");
        explosionSprite.init(spriteShaderProgramId, resourcesPath + "explosion_sprites.png", 5);
        
        componentMask |= 1 << getTypeId<SpriteRenderComponent>();
        componentMask |= 1 << getTypeId<TransformComponent>();
    }
    
    void renderSprites() {
        glUseProgram(spriteShaderProgramId);
        entityManagerRef->visitEntitiesWithTypeMask(componentMask, [&](Entity<EntityManagerTypes...> &entity){
            auto &spriteComponent = entity.template getComponent<SpriteRenderComponent>();
            auto &transformComponent = entity.template getComponent<TransformComponent>();
            
            Eigen::Translation<GLfloat, 3> translationMat((transformComponent.x - HALF_SCREEN_WIDTH) / HALF_SCREEN_WIDTH,
                                                          (transformComponent.y - HALF_SCREEN_HEIGHT) / HALF_SCREEN_HEIGHT,
                                                          0);
            Eigen::DiagonalMatrix<GLfloat, 3> scaleMat(spriteComponent.width / SCREEN_WIDTH,
                                                       spriteComponent.height / SCREEN_HEIGHT,
                                                       1);
            
            Eigen::AngleAxis<GLfloat> rotationMat(transformComponent.rotationAngle, Eigen::Vector3f::UnitZ());
            
            Eigen::Transform<GLfloat, 3, Eigen::Affine> transformMatrix = translationMat * scaleMat * rotationMat;
            
            switch (spriteComponent.sprite) {
                case SpriteType::SPACESHIP:
                    shipSprite.render(transformMatrix.matrix());
                    break;
                    
                case SpriteType::RED_LASER:
                    laserRedSprite.render(transformMatrix.matrix());
                    break;
                    
                case SpriteType::BLUE_LASER:
                    laserBlueSprite.render(transformMatrix.matrix());
                    break;
                    
                case SpriteType::GREEN_LASER:
                    laserGreenSprite.render(transformMatrix.matrix());
                    break;
                    
                case SpriteType::RED_POWERUP:
                    redPowerupSprite.render(transformMatrix.matrix());
                    break;
                    
                case SpriteType::GREEN_POWERUP:
                    greenPowerupSprite.render(transformMatrix.matrix());
                    break;
                    
                case SpriteType::BLUE_POWERUP:
                    bluePowerupSprite.render(transformMatrix.matrix());
                    break;
                    
                case SpriteType::ENEMY_SPACESHIP:
                    enemyShipSprite.render(transformMatrix.matrix());
                    break;
                    
                case SpriteType::EXPLOSION:
                    explosionSprite.render(transformMatrix.matrix(), spriteComponent.currentFrame);
                    break;
                
                default:
                    break;
            }
        });
    }
};

#endif /* SpriteRenderSystem_hpp */
