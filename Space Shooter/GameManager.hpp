//
//  GameManager.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/21/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef GameManager_hpp
#define GameManager_hpp

#include <string>
#include "GameTypes.hpp"

class GameManager {
private:
    
public:
    void init(std::string resourcesPath);
    void update(double dt, GameInput &input);
    void render();
};

#endif /* GameManager_hpp */
