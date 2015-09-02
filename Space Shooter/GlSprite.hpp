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

class GlSprite {
private:
    GLuint textureId;
    GLuint glVertexBufferId;
    GLuint ss_aVecPosition;
    GLuint ss_aTexCoord;
    GLuint ss_uTexture;
    GLuint ss_uTransform;
    
public:
    void init(GLuint shaderProgram, std::string spriteImagePath);
    void render(const Eigen::Matrix<GLfloat, 4, 4> &transformMatrix);
};

#endif /* GlSprite_hpp */
