//
//  GameManager.cpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/21/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#include "GameManager.hpp"

#include <stdlib.h>
#include <boost/variant.hpp>

#include "ComponentTypes.hpp"
#include "GlobalDefines.hpp"
#include "GlUtils.hpp"
#include "Shaders.hpp"
#include "GlSprite.hpp"
#include "EntityManager.hpp"
#include "SpriteRenderSystem.hpp"
#include "PlayerAISystem.hpp"
#include "AutoMovementSystem.hpp"
#include "DiesWhenOffscreenSystem.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define COMPONENT_TYPES TransformComponent, SpriteRenderComponent, PlayerDetailsComponent, DiesWhenOffscreenComponent, AABBComponent, AutoMovementComponent

GlSprite shipSprite;
GlSprite laserSprite;

EntityManager<COMPONENT_TYPES> entityManager;
SpriteRenderSystem<COMPONENT_TYPES> renderSystem;
PlayerAISystem<COMPONENT_TYPES> playerAISystem;
AutoMovementSystem<COMPONENT_TYPES> autoMovementSystem;
DiesWhenOffscreenSystem<COMPONENT_TYPES> diesWhenOffscreenSystem;

double gameTime = 0;
bool initialized = false;

void GameManager::init(std::string resourcesPath) {
    
    // init shaders
    GLuint backgroundShaderProgramId = compileShaders("background shaders", BACKGROUND_VERTEX_SHADER, BACKGROUND_FRAGMENT_SHADER);
    GLuint spriteShaderProgramId = compileShaders("sprite shaders", SPRITE_VERTEX_SHADER, SPRITE_FRAGMENT_SHADER);
    
    if (backgroundShaderProgramId == -1 || spriteShaderProgramId == -1) {
        // we're boned :/
        printf("OH NOES, SHADERS FAILED TO COMPILE...\n");
        exit(1);
    }

    shipSprite.init(spriteShaderProgramId, resourcesPath + "spaceship.png");
    laserSprite.init(spriteShaderProgramId, resourcesPath + "laser.png");
    
    renderSystem.init(&entityManager, backgroundShaderProgramId, spriteShaderProgramId);
    playerAISystem.init(&entityManager, &laserSprite);
    autoMovementSystem.init(&entityManager);
    diesWhenOffscreenSystem.init(&entityManager);
    
    auto &shipEntity = entityManager.createEntity();
    TransformComponent shipTransformComponent;
    shipTransformComponent.x = 90;
    shipTransformComponent.y = 240;
    AABBComponent playerBounds;
    playerBounds.width = 64;
    playerBounds.height = 64;
    SpriteRenderComponent shipSpriteComponent;
    shipSpriteComponent.sprite = &shipSprite;
    shipSpriteComponent.scaleX = playerBounds.width / SCREEN_WIDTH;
    shipSpriteComponent.scaleY = playerBounds.height / SCREEN_HEIGHT;
    PlayerDetailsComponent playerComponent;
    shipEntity.addComponent(playerBounds);
    shipEntity.addComponent(shipTransformComponent);
    shipEntity.addComponent(shipSpriteComponent);
    shipEntity.addComponent(playerComponent);
    
    // setup defaults
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    initialized = true;
}

void GameManager::update(double dt, GameInput &input) {
    
// debug keystate
//    for (int i = 0; i < 255; ++i) {
//        if (input.keys[i] == KS_DOWN) {
//            printf("key %d\n", i);
//        }
//    }
    
    gameTime += dt;
    
    // update player position
    playerAISystem.updatePosition(dt, gameTime, input);
    
    // update dumb entities
    autoMovementSystem.moveEntities(dt);
    
    // update entities that should die when the go offscreen
    diesWhenOffscreenSystem.checkOffscreenPositions();
    
    entityManager.cleanEntities();
}

void GameManager::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (!initialized) {
        return;
    }
    
    renderSystem.renderSprites();

}