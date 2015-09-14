//
//  LaserDetailsComponent.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 9/13/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef LaserDetailsComponent_h
#define LaserDetailsComponent_h

class LaserDetailsComponent {
public:
    bool firedByPlayer;
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity) {
        //noop
    }
};

#endif /* LaserDetailsComponent_h */
