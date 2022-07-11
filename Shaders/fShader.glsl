#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

varying vec3 vPosition;
varying vec3 vNormal;
varying vec2 vTexturePosition;

uniform vec3 uCameraPosition;
uniform vec3 uLightPosition;
uniform vec3 uLightColor;
uniform float uLightInnerCut;
uniform float uLightOuterCut;
uniform int uLightType;

uniform Material uMaterial;

vec4 directionalLight() {
    /* Background component */
    vec3 ambient = uLightColor * uMaterial.ambient;

    /* Scattered component */
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(uLightPosition);
    vec3 diffuse = uLightColor * max(dot(normal, lightDirection), 0.0) * uMaterial.diffuse;

    /* Reflected component */
    vec3 cameraDirection = normalize(uCameraPosition - vPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 specular = uLightColor * pow(max(dot(cameraDirection, reflectDirection), 0.0), uMaterial.shininess) * uMaterial.specular;

    return vec4(ambient + diffuse + specular, 1.0);
}

vec4 pointLight() {
    /* Background component */
    vec3 ambient = uLightColor * uMaterial.ambient;

    /* Scattered component */
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(uLightPosition - vPosition);
    vec3 diffuse = uLightColor * max(dot(normal, lightDirection), 0.0) * uMaterial.diffuse;

    /* Reflected component */
    vec3 cameraDirection = normalize(uCameraPosition - vPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 specular = uLightColor * pow(max(dot(cameraDirection, reflectDirection), 0.0), uMaterial.shininess) * uMaterial.specular;

    float distance = length(uLightPosition - vPosition);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * pow(distance, 2.0));

    return vec4(attenuation * (ambient + diffuse + specular), 1.0);
}

vec4 spotLight() {
    /* Background component */
    vec3 ambient = uLightColor * uMaterial.ambient;

    /* Scattered component */
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(uLightPosition - vPosition);
    vec3 diffuse = uLightColor * max(dot(normal, lightDirection), 0.0) * uMaterial.diffuse;

    /* Reflected component */
    vec3 cameraDirection = normalize(uCameraPosition - vPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 specular = uLightColor * pow(max(dot(cameraDirection, reflectDirection), 0.0), uMaterial.shininess) * uMaterial.specular;

    float angle = dot(lightDirection, normalize(uLightPosition));
    float intensity = clamp((angle - uLightOuterCut) / (uLightInnerCut - uLightOuterCut), 0.0, 1.0);

    float distance = length(uLightPosition - vPosition);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * pow(distance, 2.0));

    return vec4(attenuation * (ambient + intensity * (diffuse + specular)), 1.0);
}

void main() {
    /*switch (uLightType) {
        case 0:
            gl_FragColor = directionalLight();
            break;
        case 1:
            gl_FragColor = pointLight();
            break;
        case 2:
            gl_FragColor = spotLight();
            break;
        default:
            gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }*/
    gl_FragColor = directionalLight();
}
