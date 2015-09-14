//
//  DiesAfterTimePeriodComponent.h
//  Space Shooter
//
//  Created by Karl Kirch on 9/13/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef DiesAfterTimePeriodComponent_h
#define DiesAfterTimePeriodComponent_h

class DiesAfterTimePeriodComponent {
public:
    
    double deathTimeout;
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity) {
        //noop
    }
};

#endif /* DiesAfterTimePeriodComponent_h */
