#version 460 core
in vec3 worldPosition;
in vec3 normal;
in vec2 uv;

out vec4 FragColor;

// 方向光参数
uniform vec3 lightDir;      // 方向光方向（世界空间，已归一化，指向物体）
uniform vec3 lightColor;    // 方向光颜色
uniform float lightIntensity; // 方向光强度

// 材质参数
uniform vec3 viewPos;       // 摄像机位置
uniform float shininess;    // 高光系数
uniform float specularStrength; // 高光强度

uniform sampler2D uTexture;

void main() {
    // 归一化法线
    vec3 norm = normalize(normal);

    // 方向光方向（已归一化，指向物体）
    vec3 dir = normalize(-lightDir);

    vec3 texColor = texture(uTexture, uv).rgb;

    // 漫反射
    float diff = max(dot(norm, dir), 0.0);
    vec3 diffuse = diff * lightColor * lightIntensity * texColor;

    // 采样纹理颜色
    

    // 视线方向
    vec3 viewDir = normalize(viewPos - worldPosition);

    // 反射方向
    vec3 reflectDir = reflect(-dir, norm);

    // 高光
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor * lightIntensity;

    // 环境光（可选，简单常量）
    vec3 ambient = 0.4 * lightColor;

    // 合成最终颜色
    vec3 result = (ambient + diffuse + specular) * texColor;
    //if (all(equal(diffuse, vec3(0.0)))) result = vec3(1.0, 1.0, 1.0);
    FragColor = vec4(result, 1.0);
}