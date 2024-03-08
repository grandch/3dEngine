#version 150 core
#define M_PI 3.1415926535897932384626433832795


out vec4 out_Color;

in vec2 TexCoords;

struct LightManager
{
    vec4 pointLightsLocations[10];
    vec3 pointLightsColors[10];
    int nbPointLights;
    vec3 ambientColor;
    float ambientStrength;
    mat4 view;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform LightManager lightManager;

float att(vec4 lightPos, vec3 fragPos, float strength)
{
    float dist = distance(lightPos.xyz, fragPos);
    return strength / (1.0 + 0.022 * dist + 0.0019 * pow(dist, 2));
}

vec3 ambient()
{
    vec3 amb = lightManager.ambientColor * lightManager.ambientStrength;
    return amb;
}

float diffuse_brdf()
{
    return 1 / M_PI;
}

int heaviside(float x)
{
    if(x > 0)
        return 1;
    return 0;
}

float GSchlickGGX(float NdotV, float k)
{
    return NdotV / ((NdotV) * (1-k) + k);
}

float GSmith(float NdotV, float NdotL, float alpha)
{
    float k = pow(alpha+1, 2)/8; // direct lighting
    float ggx1 = GSchlickGGX(NdotV, k);
    float ggx2 = GSchlickGGX(NdotL, k);
    return ggx1 * ggx2;
}

float VGGX(float NdotL, float NdotV, float HdotL, float HdotV, float alpha)
{
    return (heaviside(HdotL) * heaviside(HdotV)) / ((abs(NdotL) + sqrt(alpha + (1 - alpha) * NdotL * NdotL)) * (abs(NdotV) + sqrt(alpha + (1 - alpha) * NdotV * NdotV)));
}

float DGGX(float NdotH, float alpha)
{
    return (alpha * NdotH) / (M_PI * pow((pow(NdotH, 2)*(alpha -1) + 1), 2));
}

vec3 specular_brdf(float NdotL, float NdotH, float NdotV, float HdotL, float HdotV, float alpha)
{
    return vec3(DGGX(NdotH, alpha*alpha) * GSmith(NdotV, NdotL, alpha));
}

vec3 conductor_fresnel(vec3 f0, vec3 bsdf, float HdotV)
{
    return bsdf * (f0 + (1 - f0) * pow(1 - abs(HdotV), 5));
}

vec3 fresnel_mix(vec3 f0, vec3 base, vec3 layer, float HdotV)
{
    vec3 fr = f0 + (1 - f0) * pow(1 - abs(HdotV), 5);
    return mix(base, layer, fr);
}

void main()
{
    // retrieve data from G-buffer
    vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float specular = texture(gAlbedoSpec, TexCoords).a;

    float roughness = texture(gNormal, TexCoords).a;
    float metallic = specular;

    float alpha = roughness;

    vec3 v = normalize(-fragPos);
    float NdotV = max(dot(normal, v), 0.);

    vec3 result = ambient() * albedo;

    for(int i = 0; i < lightManager.nbPointLights; i++)
    {
        vec3 l = normalize(-fragPos + vec3(lightManager.view * lightManager.pointLightsLocations[i]));
        vec3 h = normalize(l + v);
        float NdotL = max(dot(normal, l), 0.);
        float NdotH = max(dot(normal, h), 0.);
        float HdotL = max(dot(h, l), 0.);
        float HdotV = max(dot(h, v), 0.);

        vec3 f_diffuse = NdotH * diffuse_brdf() * lightManager.pointLightsColors[i] * albedo;
        vec3 f_specular = NdotH * specular_brdf(NdotL, NdotH, NdotV, HdotL, HdotV, alpha) * lightManager.pointLightsColors[i];
        vec3 dielectric = fresnel_mix(vec3(1.5), f_diffuse, f_specular, HdotV);
        vec3 metal = conductor_fresnel(albedo, f_specular, HdotV);

        result += mix(dielectric, metal, metallic);
    }

    out_Color = vec4(result, 1.0);
}