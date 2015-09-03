//
//  GameTypes.h
//  Space Shooter
//
//  Created by Karl Kirch on 8/21/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef GameTypes_h
#define GameTypes_h

#define VK_UP    126
#define VK_DOWN  125
#define VK_LEFT  123
#define VK_RIGHT 124
#define VK_SPACE 49

enum KEY_STATE {
    KS_UP,
    KS_DOWN
};

struct GameInput {
    KEY_STATE keys[255];
};

enum SpriteType {
    SPACESHIP,
    LASER,
    RED_POWERUP,
    GREEN_POWERUP,
    BLUE_POWERUP
};

#endif /* GameTypes_h */
