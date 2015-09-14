//
//  SpriteRenderComponent.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/27/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef SpriteRenderComponent_hpp
#define SpriteRenderComponent_hpp

#include "TransformComponent.hpp"
#include "GlSprite.hpp"

class SpriteRenderComponent {
    
private:
    
public:
    SpriteType sprite;
    float width, height;
    double timeCounter;
    int currentFrame = 0;
    double frameDuration = 500;
    int numberOfFrames = 1;
    bool loop = true;
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity) {
    }
    
    void advanceAnimation(double dt) {
        timeCounter += dt;
        if (!loop && timeCounter > frameDuration * numberOfFrames) {
            currentFrame = numberOfFrames - 1;
        } else {
            currentFrame = ((int)(timeCounter / frameDuration)) % numberOfFrames;
        }
    }
};


#endif /* SpriteRenderComponent_hpp */
