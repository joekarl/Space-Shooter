//
//  TransformComponent.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/27/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef TransformComponent_hpp
#define TransformComponent_hpp

#include "Entity.hpp"

class TransformComponent {
public:
    float x = 0, y = 0, z = 0;
    float dx = 0, dy = 0, dz = 0;
    // only affects sprite rotation right now, doesn't affect collison bounds
    float rotationAngle = 0;
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity){/*noop*/}
};

#endif /* TransformComponent_hpp */
