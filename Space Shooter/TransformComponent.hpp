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
    float x, y, z;
    float dx, dy, dz;
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity){/*noop*/}
};

#endif /* TransformComponent_hpp */
