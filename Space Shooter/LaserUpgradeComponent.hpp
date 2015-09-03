//
//  LaserUpgradeComponent.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 9/2/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef LaserUpgradeComponent_h
#define LaserUpgradeComponent_h

enum LaserUpgradeType {
    RED,
    BLUE,
    GREEN
};

class LaserUpgradeComponent {
public:
    LaserUpgradeType upgradeType;
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity) {
        //noop
    }
};

#endif /* LaserUpgradeComponent_h */
