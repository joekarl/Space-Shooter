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
public:
    SpriteType sprite;
    float scaleX, scaleY;
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity) {
        //noop
    }
};


#endif /* SpriteRenderComponent_hpp */
