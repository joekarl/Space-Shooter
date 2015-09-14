//
//  DebugCollisionRenderSystem.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 9/13/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef DebugCollisionRenderSystem_h
#define DebugCollisionRenderSystem_h

#include "EntityManager.hpp"
#include <Eigen/Geometry>
#include "GlUtils.hpp"
#include <string>

template <typename... EntityManagerTypes>
class DebugCollisionRenderSystem {
private:
    EntityManager<EntityManagerTypes...> *entityManagerRef;
    GLuint spriteShaderProgramId;
    
    GlSprite boundsSprite;
    
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
        
        boundsSprite.init(spriteShaderProgramId, resourcesPath + "DebugBounds.png");
        
        componentMask |= 1 << getTypeId<TransformComponent>();
        componentMask |= 1 << getTypeId<AABBComponent>();
    }
    
    void renderSprites() {
        glUseProgram(spriteShaderProgramId);
        entityManagerRef->visitEntitiesWithTypeMask(componentMask, [&](Entity<EntityManagerTypes...> &entity){
            auto &aabbComponent = entity.template getComponent<AABBComponent>();
            auto &transformComponent = entity.template getComponent<TransformComponent>();
            
            Eigen::Translation<GLfloat, 3> translationMat((transformComponent.x - HALF_SCREEN_WIDTH) / HALF_SCREEN_WIDTH,
                                                          (transformComponent.y - HALF_SCREEN_HEIGHT) / HALF_SCREEN_HEIGHT,
                                                          0);
            Eigen::DiagonalMatrix<GLfloat, 3> scaleMat(aabbComponent.width / SCREEN_WIDTH,
                                                       aabbComponent.height / SCREEN_HEIGHT,
                                                       1);
            
            Eigen::Transform<GLfloat, 3, Eigen::Affine> transformMatrix = translationMat * scaleMat;
            
            boundsSprite.render(transformMatrix.matrix());
        });
    }
};


#endif /* DebugCollisionRenderSystem_h */
