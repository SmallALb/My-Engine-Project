#version 460 core
#extension GL_ARB_bindless_texture : require

out vec4 FragColor;

in vec4 color;
in vec3 normal;
in vec2 uv;
in vec3 worldPosition;

#define MAXSIZE 32
//单向光
struct DirectionLight {
    vec3 direction;
    vec3 color;
    float intensity;
    float specularIntensity;
};
//点光
struct PointLight {
    vec3 position;
    vec3 color;
    float kc;
    float k1;
    float k2;
    float specularIntensity;
    int flag;
};
//聚光
struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float kc;
    float k1;
    float k2;
    float cutOff;        // 内锥角cos值
    float outerCutOff;   // 外锥角cos值
    float specularIntensity;
    int flag;
};
//是否开启环境光照
uniform bool dirLightOn;
uniform DirectionLight dirLight;

uniform PointLight pointLights[MAXSIZE];
uniform int numPointLights;

uniform SpotLight spotLights[MAXSIZE];
uniform int numSpotLights;

uniform vec3 ambientcolor;
uniform sampler2D sampler;
uniform vec3 cameraPosition;
uniform float shiness;

// 计算漫反射
vec3 calculateDiffuse(vec3 normal, vec3 lightDir, vec3 lightColor, vec3 objectColor) {
    float diffuse = clamp(dot(-lightDir, normal), 0.0, 1.0);
    return diffuse * lightColor * objectColor;
}

// 计算高光
vec3 calculateSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 lightColor, float specularIntensity) {
    vec3 reflectlight = normalize(reflect(-lightDir, normal));
    float specular = max(dot(reflectlight, viewDir), 0.0);
    specular = pow(specular, shiness);
    float doresult = dot(lightDir, normal);
    float flag = step(0.0, doresult);
    return lightColor * specular * flag * specularIntensity;
}

// 计算聚光灯
vec3 calculateSpotLight(vec3 normal, SpotLight light, vec3 viewDir) {
    if (light.flag != 1) return vec3(1.0, 0.0, 0.0);
    vec3 objectColor = texture(sampler, uv).xyz;
    vec3 lightDir = normalize(light.direction); // 聚光灯的照射方向
    //世界坐标到灯光的一条反向量
    vec3 fragToLight = normalize(worldPosition - light.position);
    //计算夹角
    float theta = dot(fragToLight, -lightDir);
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    float distance = length(worldPosition - light.position);
    float attenuation = 1.0 / (light.kc + light.k1 * distance + light.k2 * (distance * distance));
    
    // 计算漫反射和镜面反射
    vec3 diffuse = calculateDiffuse(normal, fragToLight, light.color, objectColor);
    vec3 specular = calculateSpecular(normal, fragToLight, viewDir, light.color, light.specularIntensity);
    
    return (diffuse + specular) * intensity * attenuation * light.intensity;
}

// 平行光   
vec3 calculateDirectionLight(vec3 normal, DirectionLight light, vec3 viewDir) {
    vec3 objectColor = texture(sampler, uv).xyz;
    vec3 lightDir = normalize(-light.direction);
    vec3 diffcuseColor = calculateDiffuse(normal, lightDir, light.color, objectColor);
    vec3 specularcolor = calculateSpecular(normal, -lightDir, viewDir, light.color, light.specularIntensity);
    return diffcuseColor + specularcolor;
}

// 点光源
vec3 calculatePointLight(vec3 normal, PointLight light, vec3 viewDir) {
    if (light.flag != 1) return vec3(0.0);
    vec3 objectColor = texture(sampler, uv).xyz;
    vec3 Direction = normalize(worldPosition - light.position);
    float distance = length(worldPosition - light.position);
    float attenuation = 1.0 / (light.kc + light.k1 * distance + light.k2 * (distance * distance));
    vec3 diffcuseColor = calculateDiffuse(normal, Direction, light.color, objectColor);
    vec3 specularcolor = calculateSpecular(normal, -Direction, viewDir, light.color, light.specularIntensity);
    return (diffcuseColor + specularcolor) * attenuation;
}

void main() {
    vec3 result = vec3(0.0, 0.0, 0.0);
    vec3 viewDir = normalize(cameraPosition - worldPosition);
    vec3 normalN = normalize(normal);
    vec4 objectColor = texture(sampler, uv);

    // if (objectColor.a < 0.01) {
    //     objectColor = vec4(1.0, 0.0, 1.0, 1.0); // 紫色标记无效采样
    // }

    // if (objectColor.a == 0.0) {
    //     objectColor = vec3(1.0, 0.0, 1.0);
    // }

    if (dirLightOn) {
        result += calculateDirectionLight(normalN, dirLight, viewDir);
    }
    for (int i = 0; i < numSpotLights; i++) {
        result += calculateSpotLight(normalN, spotLights[i], viewDir);
    }
    for (int i = 0; i < numPointLights; i++) {
        result += calculatePointLight(normalN, pointLights[i], viewDir);
    }
    result += objectColor.xyz * ambientcolor * 0.5; // 环境光系数
    
    FragColor = vec4(result, objectColor.w);
}