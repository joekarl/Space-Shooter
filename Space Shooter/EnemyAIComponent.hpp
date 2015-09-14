//
//  EnemyAIComponent.h
//  Space Shooter
//
//  Created by Karl Kirch on 9/13/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef EnemyAIComponent_h
#define EnemyAIComponent_h



class EnemyAIComponent {
public:
    
    enum AIType {
        DUMB,
        FLEE,
        FOLLOW_FIRE
    };
    
    size_t playerId;
    AIType aiType = AIType::DUMB;
    
    template <typename... EntityComponentTypes>
    void init(Entity<EntityComponentTypes...> &entity) {
        //noop
    }
};

#endif /* EnemyAIComponent_h */
