//
//  AABBComponent.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 9/1/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef AABBComponent_h
#define AABBComponent_h

class AABBComponent {
public:
    
    float width, height;
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity) {
        //noop
    }
};

#endif /* AABBComponent_h */
