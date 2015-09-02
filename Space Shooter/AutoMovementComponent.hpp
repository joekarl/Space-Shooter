//
//  AutoMovementComponent.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 9/1/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef AutoMovementComponent_h
#define AutoMovementComponent_h

class AutoMovementComponent {
public:
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity) {
        //noop
    }
};

#endif /* AutoMovementComponent_h */
