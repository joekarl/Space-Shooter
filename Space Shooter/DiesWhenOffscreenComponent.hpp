//
//  DiesWhenOffscreenComponent.h
//  Space Shooter
//
//  Created by Karl Kirch on 8/28/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef DiesWhenOffscreenComponent_h
#define DiesWhenOffscreenComponent_h

class DiesWhenOffscreenComponent {
public:
    
    bool cameOnscreen = false;
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity) {
        //noop
    }
};

#endif /* DiesWhenOffscreenComponent_h */
