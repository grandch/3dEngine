#version 150 core
#define M_PI 3.1415926535897932384626433832795

// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#implementation

struct LightManager
{
    vec4 pointLightsLocations[10];
    vec3 pointLightsColors[10];
    int nbPointLights;
    vec3 ambientColor;
    float ambientStrength;
};

struct Material
{
    vec3 diffuseColor;
    vec3 specularColor;
    float specularStrength;
    float shininess;

    sampler2D diffuseColorTexture;
    sampler2D specularColorTexture;
    sampler2D shininessTexture;
    sampler2D specularStrengthTexture;

    bool hasDiffuseColorTexture;
    bool hasSpecularColorTexture;
    bool hasShininessTexture;
    bool hasSpecularStrengthTexture;
};

in vec3 normal;
in vec3 fragPos;
in vec3 objectColor;
in vec2 out_uv;

uniform Material material;
uniform LightManager lightManager;

uniform mat4 view;

out vec4 out_Color;

float att(vec4 lightPos, float strength)
{
    float dist = distance(lightPos.xyz, fragPos);
    return strength / (1.0 + 0.022 * dist + 0.0019 * pow(dist, 2));
}

vec3 ambient()
{
    vec3 amb = lightManager.ambientColor * lightManager.ambientStrength;
    vec3 result = amb * objectColor;
    return result;
}

vec3 diffuse_brdf()
{
    if(material.hasDiffuseColorTexture)
    {
        return vec3(texture(material.diffuseColorTexture, out_uv)) / M_PI;
    }
    return material.diffuseColor / M_PI;
}

int heaviside(float x)
{
    if(x > 0)
        return 1;
    return 0;
}

float VGGX(float NdotL, float NdotV, float HdotL, float HdotV, float alpha)
{
    return (heaviside(HdotL) * heaviside(HdotV)) / ((abs(NdotL) + sqrt(alpha + (1 - alpha) * NdotL * NdotL)) * (abs(NdotV) + sqrt(alpha + (1 - alpha) * NdotV * NdotV)));
}

float DGGX(float NdotH, float alpha)
{
    return (alpha * heaviside(NdotH)) / (M_PI * pow((pow(NdotH, 2)*(alpha -1) + 1), 2));
}

vec3 specular_brdf(float NdotL, float NdotH, float NdotV, float HdotL, float HdotV, float alpha)
{
    float alphaSq = alpha * alpha;
    return vec3(DGGX(NdotH, alphaSq) * VGGX(NdotL, NdotV, HdotL, HdotV, alphaSq));
}

vec3 conductor_fresnel(vec3 f0, vec3 bsdf, vec3 lightPos)
{    
    vec3 l = normalize(-fragPos + lightPos);
    vec3 v = normalize(-fragPos);
    vec3 h = normalize(l + v);
    return bsdf * (f0 + (1 - f0) * pow(1 - abs(dot(v, h)), 5));
}

vec3 fresnel_mix(vec3 f0, vec3 base, vec3 layer, float HdotV)
{
    vec3 fr = f0 + (1 - f0) * pow(1 - abs(HdotV), 5);
    return mix(base, layer, fr);
}

void main()
{
    vec3 result = ambient();
    vec3 metal_brdf, dialectric_brdf;
    float roughness = material.shininess;
    float metallic = material.specularStrength;

    vec3 c_diff = mix(material.diffuseColor, vec3(0), metallic);
    vec3 f0 = mix(vec3(0.04), material.diffuseColor, metallic);
    float alpha = roughness * roughness;

    vec3 v = normalize(-fragPos);
    float NdotV = dot(normal, v);
    for(int i = 0; i < lightManager.nbPointLights; i++) 
    {
        vec3 l = normalize(-fragPos + vec3(view * lightManager.pointLightsLocations[i]));
        vec3 h = normalize(l + v);
        float NdotL = dot(normal, l);
        float NdotH = dot(normal, h);
        float HdotL = dot(h, l);
        float HdotV = dot(h, v);

        vec3 f_diffuse = NdotH * diffuse_brdf() * lightManager.pointLightsColors[i];
        vec3 f_specular = NdotH * specular_brdf(NdotL, NdotH, NdotV, HdotL, HdotV, alpha) * lightManager.pointLightsColors[i];
        result += fresnel_mix(f0, f_diffuse, f_specular, HdotV);
    }

    out_Color = vec4(result, 1.0);
}
