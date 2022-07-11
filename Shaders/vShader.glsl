#version 330 core

attribute vec3 aPosition;
attribute vec3 aNormal;

varying vec3 vPosition;
varying vec3 vNormal;
varying vec2 vTexturePosition;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    vPosition = vec3(uModel * vec4(aPosition, 1.0));
    vNormal = vec3(uModel * vec4(aNormal, 0.0));
    vTexturePosition = 0.1 * aPosition.xz;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
