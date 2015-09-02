//
//  Shaders.cpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/23/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#include "Shaders.hpp"

// Shaders for background warp drawing
const std::string BACKGROUND_VERTEX_SHADER = R"(
attribute vec2 aPosition;
attribute vec2 aTexCoord;
varying vec2 vTexCoord;

void main()
{
    gl_Position = vec4(aPosition, 0.0, 1.0);
    vTexCoord = aTexCoord;
}
)";


const std::string BACKGROUND_FRAGMENT_SHADER = R"(
varying vec2 vTexCoord;
uniform sampler2D uTexture;
uniform vec2 uBackgroundOffset;
uniform vec2 uScreenResolution;

/*
 struct warpPoint {
 float centerX, centerY;
 float radius;
 float warpRadians;
 }
 */
uniform vec4 uWarpPoint0;
uniform vec4 uWarpPoint1;
uniform vec4 uWarpPoint2;
uniform vec4 uWarpPoint3;

void warpAroundPoint(in vec4 warpPoint,
                     inout vec2 uv,
                     in float resolutionRatio) {
    
    // if our warp point is nonsense ignore it...
    if (warpPoint.x < -50.0 || warpPoint.y < -50.0) {
        return;
    }
    
    // move our radius to screen coordinates
    float radius = warpPoint.z / resolutionRatio;
    
    // move center to screen coordinates and apply background offset
    vec2 center = (warpPoint.xy + uBackgroundOffset) * resolutionRatio;
    
    // do warp calculations
    vec2 screenSpaceUV = resolutionRatio * uv;
    vec2 screenSpaceDistance = screenSpaceUV - center;
    float len = length(screenSpaceDistance);
    
    vec2 temp = screenSpaceUV;
    // if within the radius of our warp point calculate the warp UV
    if (len <= radius) {
        float beta = atan(screenSpaceDistance.y, screenSpaceDistance.x) + radians(warpPoint.w) * (radius - len) / radius;
        temp = center + len * vec2(cos(beta), sin(beta));
    }
    
    // convert back to
    uv = temp / resolutionRatio;
}

void main(void) {
    
    vec2 uv = gl_FragCoord.xy / uScreenResolution;
    uv += uBackgroundOffset;
    float resolutionRatio = uScreenResolution.x / uScreenResolution.y;
    warpAroundPoint(uWarpPoint3, uv, resolutionRatio);
    warpAroundPoint(uWarpPoint2, uv, resolutionRatio);
    warpAroundPoint(uWarpPoint1, uv, resolutionRatio);
    warpAroundPoint(uWarpPoint0, uv, resolutionRatio);
    
    vec4 rColor = texture2D(uTexture, uv + vec2(-3.0 / uScreenResolution.x, 0));
    vec4 gColor = texture2D(uTexture, uv);
    vec4 bColor = texture2D(uTexture, uv + vec2(3.0 / uScreenResolution.x, 0));
    
    gl_FragColor = vec4(rColor.r, gColor.g, bColor.b, 1.0);
    //gl_FragColor = texture2D(uTexture, uv);
}
)";

// shaders for sprite drawing
const std::string SPRITE_VERTEX_SHADER = R"(
attribute vec2 aVecPosition;
attribute vec2 aTexCoord;
varying vec2 vTexCoord;
uniform mat4 uTransform;

void main()
{
    gl_Position = uTransform * vec4(aVecPosition, 0.0, 1.0);
    vTexCoord = aTexCoord;
}
)";


const std::string SPRITE_FRAGMENT_SHADER = R"(
varying vec2 vTexCoord;
uniform sampler2D uTexture;

void main(void) {
    gl_FragColor = texture2D(uTexture, vTexCoord);
}
)";

