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

template <typename... EntityManagerTypes>
class SpriteRenderSystem {
private:
    EntityManager<EntityManagerTypes...> *entityManagerRef;
    GLuint backgroundShaderProgramId;
    GLuint spriteShaderProgramId;
    
    int componentMask = 0;
    
public:
    void init(EntityManager<EntityManagerTypes...> *entityManagerRef,
              GLuint backgroundShaderProgramId,
              GLuint spriteShaderProgramId) {
        this->entityManagerRef = entityManagerRef;
        this->backgroundShaderProgramId = backgroundShaderProgramId;
        this->spriteShaderProgramId = spriteShaderProgramId;
        
        componentMask |= 1 << getTypeId<SpriteRenderComponent>();
        componentMask |= 1 << getTypeId<TransformComponent>();
    }
    
    void renderBackground() {
        // noop for now
    }
    
    void renderSprites() {
        glUseProgram(spriteShaderProgramId);
        entityManagerRef->findEntitiesWithTypeMask(componentMask, [](Entity<EntityManagerTypes...> &entity){
            auto &spriteComponent = entity.template getComponent<SpriteRenderComponent>();
            auto &transformComponent = entity.template getComponent<TransformComponent>();
            
            Eigen::Translation<GLfloat, 3> translationVec((transformComponent.x - HALF_SCREEN_WIDTH) / HALF_SCREEN_WIDTH,
                                                          (transformComponent.y - HALF_SCREEN_HEIGHT) / HALF_SCREEN_HEIGHT,
                                                          0);
            Eigen::DiagonalMatrix<GLfloat, 3> scaleVec(spriteComponent.scaleX, spriteComponent.scaleY, 1);
            
            Eigen::Transform<GLfloat, 3, Eigen::Affine> transformMatrix = translationVec * scaleVec;
            
            spriteComponent.sprite->render(transformMatrix.matrix());
        });
    }
};

#endif /* SpriteRenderSystem_hpp */
