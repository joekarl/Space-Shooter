//
//  PlayerDetailsComponent.h
//  Space Shooter
//
//  Created by Karl Kirch on 8/30/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef PlayerDetailsComponent_h
#define PlayerDetailsComponent_h

#include "GameTypes.hpp"

#define DEFAULT_LASER RED

class PlayerDetailsComponent {
public:
    double lastFireTime;
    LaserUpgradeType laserType = LaserUpgradeType::RED;
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity) {
        //noop
    }
};

#endif /* PlayerDetailsComponent_h */