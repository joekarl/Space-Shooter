//
//  IsPlayerComponent.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/28/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef IsPlayerComponent_h
#define IsPlayerComponent_h

class IsPlayerComponent {
public:
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity) {
        //noop
    }
};

#endif /* IsPlayerComponent_h */
