//
//  ComponentTypes.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/27/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef ComponentTypes_hpp
#define ComponentTypes_hpp

#include "Entity.hpp"
#include "SpriteRenderComponent.hpp"
#include "TransformComponent.hpp"
#include "PlayerDetailsComponent.hpp"
#include "DiesWhenOffscreenComponent.hpp"
#include "AABBComponent.hpp"
#include "AutoMovementComponent.hpp"
#include "CollisionComponent.hpp"
#include "LaserUpgradeComponent.hpp"

template<>
int getTypeId<TransformComponent>() {
    return 0;
}

template <>
int getTypeId<SpriteRenderComponent>() {
    return 1;
}

template <>
int getTypeId<PlayerDetailsComponent>() {
    return 2;
}

template <>
int getTypeId<CollisionComponent>() {
    return 3;
}

template <>
int getTypeId<DiesWhenOffscreenComponent>() {
    return 4;
}

template<>
int getTypeId<AABBComponent>() {
    return 5;
}

template<>
int getTypeId<AutoMovementComponent>() {
    return 6;
}

template<>
int getTypeId<LaserUpgradeComponent>() {
    return 7;
}

#endif /* ComponentTypes_h */
