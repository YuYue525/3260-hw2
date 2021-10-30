#version 330 core

in vec2 TexCoord;
in vec3 normalWorld;
in vec3 vertexPositionWorld;
in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;
uniform sampler2D Texture;

uniform vec3 eyePositionWorld;
uniform vec3 lightPos;

uniform vec3 env_ambientLight;
uniform vec3 env_lightDirection;
uniform float lightBrightness;

uniform float pointLightBrightness;

uniform vec3 point1_ambientLight;
uniform vec3 point1_lightPos;

uniform vec3 point2_ambientLight;
uniform vec3 point2_lightPos;

uniform vec3 point3_ambientLight;
uniform vec3 point3_lightPos;

uniform vec3 point4_ambientLight;
uniform vec3 point4_lightPos;

uniform vec3 point5_ambientLight;
uniform vec3 point5_lightPos;

uniform vec3 spot_ambientLight;
uniform vec3 spotPos;
uniform vec3 spotDir;
uniform float spotCutOff;
uniform float spotOuterCutOff;
uniform int spotOn;

out vec4 Color;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(normalWorld);
    vec3 lightDir = normalize(lightPos - vertexPositionWorld);
    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{

    float shadow = ShadowCalculation(FragPosLightSpace);

    
    //texture
    vec3 temp_texture = vec3(texture(Texture, TexCoord));
    //env_ambient
    vec3 env_ambient = temp_texture * env_ambientLight;
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
    
    vec4 env_color = lightBrightness * vec4(temp_texture * (env_ambient + (1-shadow) * (clamp(env_diffuseLight, 0, 1) + env_specularLight)), 1.0f);
    
    //point1_ambient
    vec3 v1 = point1_lightPos - vertexPositionWorld;
    float d1 = sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
    float fd1 = 1.0/(5*d1*d1);
    
    vec3 point1_ambient = temp_texture * point1_ambientLight;
    //env_diffuse
    vec3 point1_lightVectorWorld = normalize(point1_lightPos - vertexPositionWorld);
    float point1_brightness = dot(point1_lightVectorWorld, normalize(normalWorld));
    vec3 point1_diffuseLight = vec3(point1_brightness, point1_brightness, point1_brightness);
    //point_specular
    vec3 point1_reflectedLightVectorWorld = reflect(-point1_lightVectorWorld, normalWorld);
    float point1_s = clamp(dot(point1_reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
    point1_s = pow(point1_s, 50);
    vec3 point1_specularLight = vec3(point1_s, point1_s, point1_s);
    
    vec4 point1_color = fd1 * vec4(0.0f, 1.0f, 0.0f, 1.0f) * vec4(temp_texture * (point1_ambient + clamp(point1_diffuseLight, 0, 1) + point1_specularLight), 1.0f);
    
    //point2_ambient
    vec3 v2 = point2_lightPos - vertexPositionWorld;
    float d2 = sqrt(v2.x*v2.x + v2.y*v2.y + v2.z*v2.z);
    float fd2 = 1.0/(5*d2*d2);
    
    vec3 point2_ambient = temp_texture * point2_ambientLight;
    //env_diffuse
    vec3 point2_lightVectorWorld = normalize(point2_lightPos - vertexPositionWorld);
    float point2_brightness = dot(point2_lightVectorWorld, normalize(normalWorld));
    vec3 point2_diffuseLight = vec3(point2_brightness, point2_brightness, point2_brightness);
    //point_specular
    vec3 point2_reflectedLightVectorWorld = reflect(-point2_lightVectorWorld, normalWorld);
    float point2_s = clamp(dot(point2_reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
    point2_s = pow(point2_s, 50);
    vec3 point2_specularLight = vec3(point2_s, point2_s, point2_s);
    
    vec4 point2_color = fd2 * vec4(1.0f, 0.0f, 0.0f, 1.0f) * vec4(temp_texture * (point2_ambient + clamp(point2_diffuseLight, 0, 1) + point2_specularLight), 1.0f);
    
    //point3_ambient
    vec3 v3 = point3_lightPos - vertexPositionWorld;
    float d3 = sqrt(v3.x*v3.x + v3.y*v3.y + v3.z*v3.z);
    float fd3 = 1.0/(5*d3*d3);
    
    vec3 point3_ambient = temp_texture * point3_ambientLight;
    //env_diffuse
    vec3 point3_lightVectorWorld = normalize(point3_lightPos - vertexPositionWorld);
    float point3_brightness = dot(point3_lightVectorWorld, normalize(normalWorld));
    vec3 point3_diffuseLight = vec3(point3_brightness, point3_brightness, point3_brightness);
    //point_specular
    vec3 point3_reflectedLightVectorWorld = reflect(-point3_lightVectorWorld, normalWorld);
    float point3_s = clamp(dot(point3_reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
    point3_s = pow(point3_s, 50);
    vec3 point3_specularLight = vec3(point3_s, point3_s, point3_s);
    
    vec4 point3_color = fd3 * vec4(0.0f, 0.0f, 1.0f, 1.0f) * vec4(temp_texture * (point3_ambient + clamp(point3_diffuseLight, 0, 1) + point3_specularLight), 1.0f);
    
    //point4_ambient
    vec3 v4 = point4_lightPos - vertexPositionWorld;
    float d4 = sqrt(v4.x*v4.x + v4.y*v4.y + v4.z*v4.z);
    float fd4 = 1.0/(5*d4*d4);
    
    vec3 point4_ambient = temp_texture * point4_ambientLight;
    //env_diffuse
    vec3 point4_lightVectorWorld = normalize(point4_lightPos - vertexPositionWorld);
    float point4_brightness = dot(point4_lightVectorWorld, normalize(normalWorld));
    vec3 point4_diffuseLight = vec3(point4_brightness, point4_brightness, point4_brightness);
    //point_specular
    vec3 point4_reflectedLightVectorWorld = reflect(-point4_lightVectorWorld, normalWorld);
    float point4_s = clamp(dot(point4_reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
    point4_s = pow(point4_s, 50);
    vec3 point4_specularLight = vec3(point4_s, point4_s, point4_s);
    
    vec4 point4_color = fd4 * vec4(1.0f, 1.0f, 0.0f, 1.0f) * vec4(temp_texture * (point4_ambient + clamp(point4_diffuseLight, 0, 1) + point4_specularLight), 1.0f);
    
    //point5_ambient
    vec3 v5 = point5_lightPos - vertexPositionWorld;
    float d5 = sqrt(v5.x*v5.x + v5.y*v5.y + v5.z*v5.z);
    float fd5 = 1.0/(1+0.09*d5+2*d5*d5);
    
    vec3 point5_ambient = temp_texture * point5_ambientLight;
    //env_diffuse
    vec3 point5_lightVectorWorld = normalize(point5_lightPos - vertexPositionWorld);
    float point5_brightness = dot(point5_lightVectorWorld, normalize(normalWorld));
    vec3 point5_diffuseLight = vec3(point5_brightness, point5_brightness, point5_brightness);
    //point_specular
    vec3 point5_reflectedLightVectorWorld = reflect(-point5_lightVectorWorld, normalWorld);
    float point5_s = clamp(dot(point5_reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
    point5_s = pow(point5_s, 50);
    vec3 point5_specularLight = vec3(point5_s, point5_s, point5_s);
    
    vec4 point5_color = fd5 * vec4(1.0f, 1.0f, 1.0f, 1.0f) * vec4(temp_texture * (point5_ambient + clamp(point5_diffuseLight, 0, 1) + point5_specularLight), 1.0f);
    
    //spot
    vec3 lightDir = normalize(spotPos - vertexPositionWorld);
    vec4 spot_color;
    // Check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-spotDir));
    if(theta > spotCutOff)
    {
        vec3 v6 = spotPos - vertexPositionWorld;
        float d6 = sqrt(v6.x*v6.x + v6.y*v6.y + v6.z*v6.z);
        float fd6 = 1.0/(0.3*d6*d6);
        
        vec3 spot_ambient = temp_texture * spot_ambientLight;
        //env_diffuse
        vec3 spot_lightVectorWorld = lightDir;
        float spot_brightness = dot(spot_lightVectorWorld, normalize(normalWorld));
        vec3 spot_diffuseLight = vec3(spot_brightness, spot_brightness, spot_brightness);
        //point_specular
        vec3 spot_reflectedLightVectorWorld = reflect(-spot_lightVectorWorld, normalWorld);
        float spot_s = clamp(dot(spot_reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
        spot_s = pow(spot_s, 50);
        vec3 spot_specularLight = vec3(spot_s, spot_s, spot_s);
        
        float epsilon = (spotCutOff - spotOuterCutOff);
        float intensity = clamp((theta - spotOuterCutOff) / epsilon, 0.0, 1.0);
        
        spot_color = fd6 * vec4(1.0f, 1.0f, 0.0f, 1.0f) * vec4(temp_texture * (spot_ambient + (1-intensity) * (clamp(spot_diffuseLight, 0, 1) + spot_specularLight)), 1.0f);
    }
    else {
        spot_color = vec4(0.0f);
    }
    
    
    Color = env_color + pointLightBrightness * (point1_color + point2_color + point3_color + point4_color + point5_color) + spotOn * spot_color;
}
