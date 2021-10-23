#version 330 core

in vec2 TexCoord;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

out vec4 Color;

uniform sampler2D Texture;

uniform vec3 env_ambientLight;
uniform vec3 env_lightDirection;
uniform float lightBrightness;

uniform vec3 point1_ambientLight;
uniform vec3 point1_lightPos;

uniform vec3 point2_ambientLight;
uniform vec3 point2_lightPos;

uniform vec3 point3_ambientLight;
uniform vec3 point3_lightPos;

uniform vec3 point4_ambientLight;
uniform vec3 point4_lightPos;

uniform vec3 eyePositionWorld;

void main()
{
    //texture
    vec3 temp_texture = vec3(texture(Texture, TexCoord));
    //env_ambient
    vec3 env_ambientLight = temp_texture * env_ambientLight;
    //env_diffuse
    vec3 env_lightVectorWorld = normalize(env_lightDirection);
    float env_brightness = dot(env_lightVectorWorld, normalize(normalWorld));
    vec3 env_diffuseLight = vec3(env_brightness, env_brightness, env_brightness);
    //env_specular
    vec3 env_reflectedLightVectorWorld = reflect(-env_lightVectorWorld, normalWorld);
    vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
    float env_s = clamp(dot(env_reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
    env_s = pow(env_s, 50);
    vec3 env_specularLight = vec3(env_s, env_s, env_s);
    
    vec4 env_color = lightBrightness * vec4(temp_texture * (env_ambientLight + clamp(env_diffuseLight, 0, 1) + env_specularLight), 1.0f);
    
    //point1_ambient
    vec3 v1 = point1_lightPos - vertexPositionWorld;
    float d1 = sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
    float fd1 = 1.0/(5*d1*d1);
    
    vec3 point1_ambientLight = temp_texture * point1_ambientLight;
    //env_diffuse
    vec3 point1_lightVectorWorld = normalize(point1_lightPos - vertexPositionWorld);
    float point1_brightness = dot(point1_lightVectorWorld, normalize(normalWorld));
    vec3 point1_diffuseLight = vec3(point1_brightness, point1_brightness, point1_brightness);
    //point_specular
    vec3 point1_reflectedLightVectorWorld = reflect(-point1_lightVectorWorld, normalWorld);
    float point1_s = clamp(dot(point1_reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
    point1_s = pow(point1_s, 50);
    vec3 point1_specularLight = vec3(point1_s, point1_s, point1_s);
    
    vec4 point1_color = fd1 * vec4(0.0f, 1.0f, 0.0f, 1.0f) * vec4(temp_texture * (point1_ambientLight + clamp(point1_diffuseLight, 0, 1) + point1_specularLight), 1.0f);
    
    //point2_ambient
    vec3 v2 = point2_lightPos - vertexPositionWorld;
    float d2 = sqrt(v2.x*v2.x + v2.y*v2.y + v2.z*v2.z);
    float fd2 = 1.0/(5*d2*d2);
    
    vec3 point2_ambientLight = temp_texture * point2_ambientLight;
    //env_diffuse
    vec3 point2_lightVectorWorld = normalize(point2_lightPos - vertexPositionWorld);
    float point2_brightness = dot(point2_lightVectorWorld, normalize(normalWorld));
    vec3 point2_diffuseLight = vec3(point2_brightness, point2_brightness, point2_brightness);
    //point_specular
    vec3 point2_reflectedLightVectorWorld = reflect(-point2_lightVectorWorld, normalWorld);
    float point2_s = clamp(dot(point2_reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
    point2_s = pow(point2_s, 50);
    vec3 point2_specularLight = vec3(point2_s, point2_s, point2_s);
    
    vec4 point2_color = fd2 * vec4(1.0f, 0.0f, 0.0f, 1.0f) * vec4(temp_texture * (point2_ambientLight + clamp(point2_diffuseLight, 0, 1) + point2_specularLight), 1.0f);
    
    //point3_ambient
    vec3 v3 = point3_lightPos - vertexPositionWorld;
    float d3 = sqrt(v3.x*v3.x + v3.y*v3.y + v3.z*v3.z);
    float fd3 = 1.0/(5*d3*d3);
    
    vec3 point3_ambientLight = temp_texture * point3_ambientLight;
    //env_diffuse
    vec3 point3_lightVectorWorld = normalize(point3_lightPos - vertexPositionWorld);
    float point3_brightness = dot(point3_lightVectorWorld, normalize(normalWorld));
    vec3 point3_diffuseLight = vec3(point3_brightness, point3_brightness, point3_brightness);
    //point_specular
    vec3 point3_reflectedLightVectorWorld = reflect(-point3_lightVectorWorld, normalWorld);
    float point3_s = clamp(dot(point3_reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
    point3_s = pow(point3_s, 50);
    vec3 point3_specularLight = vec3(point3_s, point3_s, point3_s);
    
    vec4 point3_color = fd3 * vec4(0.0f, 0.0f, 1.0f, 1.0f) * vec4(temp_texture * (point3_ambientLight + clamp(point3_diffuseLight, 0, 1) + point3_specularLight), 1.0f);
    
    //point4_ambient
    vec3 v4 = point4_lightPos - vertexPositionWorld;
    float d4 = sqrt(v4.x*v4.x + v4.y*v4.y + v4.z*v4.z);
    float fd4 = 1.0/(5*d4*d4);
    
    vec3 point4_ambientLight = temp_texture * point4_ambientLight;
    //env_diffuse
    vec3 point4_lightVectorWorld = normalize(point4_lightPos - vertexPositionWorld);
    float point4_brightness = dot(point4_lightVectorWorld, normalize(normalWorld));
    vec3 point4_diffuseLight = vec3(point4_brightness, point4_brightness, point4_brightness);
    //point_specular
    vec3 point4_reflectedLightVectorWorld = reflect(-point4_lightVectorWorld, normalWorld);
    float point4_s = clamp(dot(point4_reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
    point4_s = pow(point4_s, 50);
    vec3 point4_specularLight = vec3(point4_s, point4_s, point4_s);
    
    vec4 point4_color = fd4 * vec4(1.0f, 1.0f, 0.0f, 1.0f) * vec4(temp_texture * (point4_ambientLight + clamp(point4_diffuseLight, 0, 1) + point4_specularLight), 1.0f);
    
    Color = env_color + point1_color + point2_color + point3_color + point4_color;
}
