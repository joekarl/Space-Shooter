//
//  GlSprite.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/25/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef GlSprite_hpp
#define GlSprite_hpp

#include <string>
#include "GlUtils.hpp"
#include <Eigen/Core>
#include <vector>

class GlSprite {
private:
    GLuint textureId;
    GLuint glVertexBufferId;
    GLuint ss_aVecPosition;
    GLuint ss_aTexCoord;
    GLuint ss_uTexture;
    GLuint ss_uTransform;
    
    std::vector<GLuint> frameTextureCoordinates;
    
public:
    void init(GLuint shaderProgram, std::string spriteImagePath);
    void init(GLuint shaderProgram, std::string spriteImagePath, float numberOfFrames);
    void render(const Eigen::Matrix<GLfloat, 4, 4> &transformMatrix);
    void render(const Eigen::Matrix<GLfloat, 4, 4> &transformMatrix, int frameNumber);
};

#endif /* GlSprite_hpp */
