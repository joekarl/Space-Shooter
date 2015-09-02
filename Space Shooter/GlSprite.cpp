//
//  GlSprite.cpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/25/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#include "GlSprite.hpp"

#include "stb_image.h"

void GlSprite::init(GLuint shaderProgram, std::string spriteImagePath) {
    // get attributes/uniforms
    ss_aVecPosition = glGetAttribLocation(shaderProgram, "aVecPosition");
    ss_aTexCoord = glGetAttribLocation(shaderProgram, "aTexCoord");
    glEnableVertexAttribArray(ss_aVecPosition);
    glEnableVertexAttribArray(ss_aTexCoord);
    ss_uTexture = glGetUniformLocation(shaderProgram, "uTexture");
    ss_uTransform = glGetUniformLocation(shaderProgram, "uTransform");
    
    // create sprite array buffer
    glGenBuffers(1, &glVertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, glVertexBufferId);
    static const GLfloat spriteBufferData[] = {
        //x,    y,   u,   v
        -1.0, -1.0, 0.0, 0.0,
         1.0,  1.0, 1.0, 1.0,
        -1.0,  1.0, 0.0, 1.0,
        -1.0, -1.0, 0.0, 0.0,
         1.0, -1.0, 1.0, 0.0,
         1.0,  1.0, 1.0, 1.0
    };
    glBufferData(GL_ARRAY_BUFFER, GL_FLOAT, spriteBufferData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    
    // create background texture
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    int spriteWidth, spriteHeight, spriteComponents;
    auto *spriteImageData = stbi_load(spriteImagePath.c_str(), &spriteWidth, &spriteHeight, &spriteComponents, STBI_rgb_alpha);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spriteWidth, spriteHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteImageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    stbi_image_free(spriteImageData);
    glBindTexture(GL_TEXTURE_2D, NULL);
}

void GlSprite::render(const Eigen::Matrix<GLfloat, 4, 4> &transformMatrix) {
    glBindBuffer(GL_ARRAY_BUFFER, glVertexBufferId);
    glVertexAttribPointer(ss_aVecPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
    glVertexAttribPointer(ss_aTexCoord, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1ui(ss_uTexture, 0);
    glUniformMatrix4fv(ss_uTransform, 1, GL_FALSE, transformMatrix.data());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glBindTexture(GL_TEXTURE_2D, NULL);
}