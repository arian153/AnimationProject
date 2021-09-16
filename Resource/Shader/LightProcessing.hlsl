struct DynamicLight
{
    float4 ambient_color;
    float4 diffuse_color;
    float4 specular_color;
    float4 ambient_range;

    float3 direction;
    float  range;

    float3 position;
    float  length;

    float3 attenuation;
    float  spot;

    int    type;
    float  intensity;
    float2 padding;
};

struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 reflect;
};

struct AmbientLight
{
    float4 ambient_color;
    float4 ambient_range;
};

struct DirectionalLight
{
    float4 ambient_color;
    float4 diffuse_color;
    float4 specular_color;
    float3 direction;
    float  intensity;
};

struct PointLight
{
    float4 ambient_color;
    float4 diffuse_color;
    float4 specular_color;

    float3 position;
    float  range;

    float3 attenuation;
    float  intensity;
};

struct SpotLight
{
    float4 ambient_color;
    float4 diffuse_color;
    float4 specular_color;

    float3 direction;
    float  range;

    float3 position;
    float  spot;

    float3 attenuation;
    float  intensity;
};

struct CapsuleLight
{
    float4 ambient_color;
    float4 diffuse_color;
    float4 specular_color;

    float3 direction;
    float  range;

    float3 position;
    float  length;

    float3 attenuation;
    float  intensity;
};

void CalculateAmbient(
    Material   mat, AmbientLight light, float3 normal,
    out float4 ambient)
{
    // Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Add ambient term.
    float up = normal.y * 0.5 + 0.5;
    ambient  = mat.ambient * (light.ambient_color + up * light.ambient_range);
}

void CalculateDirectional(
    Material   mat, DirectionalLight light,
    float3     normal, float3        to_eye,
    out float4 ambient, out float4   diffuse, out float4 specular)
{
    // Initialize outputs.
    ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // The light vector aims opposite the direction the light rays travel.
    float3 light_vec = -light.direction;

    // Add ambient term.
    ambient = mat.ambient * light.ambient_color;

    // Add diffuse and specular term, provided the surface is in 
    // the line of site of the light.

    float diffuse_factor = dot(light_vec, normal);

    // Flatten to avoid dynamic branching.
    [flatten]
    if (diffuse_factor > 0.0f)
    {
        float3 v           = reflect(-light_vec, normal);
        float  spec_factor = pow(max(dot(v, to_eye), 0.0f), mat.specular.w) * light.intensity;

        diffuse  = diffuse_factor * mat.diffuse * light.diffuse_color;
        specular = spec_factor * mat.specular * light.specular_color;
    }
}

void CalculatePoint(Material   mat, PointLight     light,
                    float3     pos, float3         normal, float3      to_eye,
                    out float4 ambient, out float4 diffuse, out float4 specular)
{
    // Initialize outputs.
    ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // The vector from the surface to the light.
    float3 light_vec = light.position - pos;

    // The distance from surface to light.
    float distance = length(light_vec);

    // Range test.
    if (distance > light.range)
        return;

    // Normalize the light vector.
    light_vec /= distance;

    // Ambient term.
    ambient = mat.ambient * light.ambient_color;

    // Add diffuse and specular term, provided the surface is in 
    // the line of site of the light.

    float diffuse_factor = dot(light_vec, normal);

    // Flatten to avoid dynamic branching.
    [flatten]
    if (diffuse_factor > 0.0f)
    {
        float3 v           = reflect(-light_vec, normal);
        float  spec_factor = pow(max(dot(v, to_eye), 0.0f), mat.specular.w);

        diffuse  = diffuse_factor * mat.diffuse * light.diffuse_color;
        specular = spec_factor * mat.specular * light.specular_color;
    }

    // Attenuate
    float att = 1.0f / dot(light.attenuation, float3(1.0f, distance, distance * distance));

    diffuse *= att;
    specular *= att;
}

void CalculateSpot(Material   mat, SpotLight      light,
                   float3     pos, float3         normal, float3      to_eye,
                   out float4 ambient, out float4 diffuse, out float4 specular)
{
    // Initialize outputs.
    ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // The vector from the surface to the light.
    float3 light_vec = light.position - pos;

    // The distance from surface to light.
    float distance = length(light_vec);

    // Range test.
    if (distance > light.range)
        return;

    // Normalize the light vector.
    light_vec /= distance;

    // Ambient term.
    ambient = ambient = mat.ambient * light.ambient_color;

    // Add diffuse and specular term, provided the surface is in 
    // the line of site of the light.

    float diffuse_factor = dot(light_vec, normal);

    // Flatten to avoid dynamic branching.
    [flatten]
    if (diffuse_factor > 0.0f)
    {
        float3 v           = reflect(-light_vec, normal);
        float  spec_factor = pow(max(dot(v, to_eye), 0.0f), mat.specular.w) * light.intensity;

        diffuse  = diffuse_factor * mat.diffuse * light.diffuse_color;
        specular = spec_factor * mat.specular * light.specular_color;
    }

    // Scale by spotlight factor and attenuate.
    float spot = pow(max(dot(-light_vec, light.direction), 0.0f), light.spot);

    // Scale by spotlight factor and attenuate.
    float att = spot / dot(light.attenuation, float3(1.0f, distance, distance * distance));

    ambient *= spot;
    diffuse *= att;
    specular *= att;
}

void CalculateCapsule(Material   mat, CapsuleLight   light,
                      float3     pos, float3         normal, float3      to_eye,
                      out float4 ambient, out float4 diffuse, out float4 specular)
{
    // Initialize outputs.
    ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 to_capsule_start = pos - light.position;
    float  distance_on_line = dot(to_capsule_start, light.direction) / light.length;
    distance_on_line        = saturate(distance_on_line) * light.length;
    float3 point_on_line    = light.position + light.direction * distance_on_line;

    // The vector from the surface to the light.
    float3 light_vec = point_on_line - pos;

    // The distance from surface to light.
    float distance = length(light_vec);

    // Range test.
    if (distance > light.range)
        return;

    // Normalize the light vector.
    light_vec /= distance;

    // Ambient term.
    ambient = mat.ambient * light.ambient_color;

    // Add diffuse and specular term
    float diffuse_factor = dot(light_vec, normal);

    // Flatten to avoid dynamic branching.
    [flatten]
    if (diffuse_factor > 0.0f)
    {
        float3 v               = reflect(-light_vec, normal);
        float  specular_factor = pow(max(dot(v, to_eye), 0.0f), mat.specular.w) * light.intensity;

        diffuse  = diffuse_factor * mat.diffuse * light.diffuse_color;
        specular = specular_factor * mat.specular * light.specular_color;
    }

    // Attenuate
    float att = 1.0f / dot(light.attenuation, float3(1.0f, distance, distance * distance));
    diffuse *= att;
    specular *= att;
}

void CalculateAmbient(
    Material   mat, DynamicLight light, float3 normal,
    out float4 ambient)
{
    // Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Add ambient term.
    float up = normal.y * 0.5 + 0.5;
    ambient  = mat.ambient * (light.ambient_color + up * light.ambient_range);
}

void CalculateDirectional(
    Material   mat, DynamicLight   light,
    float3     normal, float3      to_eye,
    out float4 ambient, out float4 diffuse, out float4 specular)
{
    // Initialize outputs.
    ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // The light vector aims opposite the direction the light rays travel.
    float3 light_vec = -light.direction;

    // Add ambient term.
    ambient = mat.ambient * light.ambient_color;

    // Add diffuse and specular term, provided the surface is in 
    // the line of site of the light.

    float diffuse_factor = dot(light_vec, normal);

    // Flatten to avoid dynamic branching.
    [flatten]
    if (diffuse_factor > 0.0f)
    {
        float3 v           = reflect(-light_vec, normal);
        float  spec_factor = pow(max(dot(v, to_eye), 0.0f), mat.specular.w) * light.intensity;

        diffuse  = diffuse_factor * mat.diffuse * light.diffuse_color;
        specular = spec_factor * mat.specular * light.specular_color;
    }
}

void CalculatePoint(Material   mat, DynamicLight   light,
                    float3     pos, float3         normal, float3      to_eye,
                    out float4 ambient, out float4 diffuse, out float4 specular)
{
    // Initialize outputs.
    ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // The vector from the surface to the light.
    float3 light_vec = light.position - pos;

    // The distance from surface to light.
    float distance = length(light_vec);

    // Range test.
    if (distance > light.range)
        return;

    // Normalize the light vector.
    light_vec /= distance;

    // Ambient term.
    ambient = mat.ambient * light.ambient_color;

    // Add diffuse and specular term, provided the surface is in 
    // the line of site of the light.

    float diffuse_factor = dot(light_vec, normal);

    // Flatten to avoid dynamic branching.
    [flatten]
    if (diffuse_factor > 0.0f)
    {
        float3 h = normalize(light_vec + to_eye);
        float  spec_factor = pow(max(dot(h, normal), 0.0f), mat.specular.w) * light.intensity;

        diffuse  = diffuse_factor * mat.diffuse * light.diffuse_color;
        specular = spec_factor * mat.specular * light.specular_color;
    }

    // Attenuate
    float att = 1.0f / dot(light.attenuation, float3(1.0f, distance, distance * distance));

    diffuse *= att;
    specular *= att;
}

void CalculateSpot(Material   mat, DynamicLight   light,
                   float3     pos, float3         normal, float3      to_eye,
                   out float4 ambient, out float4 diffuse, out float4 specular)
{
    // Initialize outputs.
    ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // The vector from the surface to the light.
    float3 light_vec = light.position - pos;

    // The distance from surface to light.
    float distance = length(light_vec);

    // Range test.
    if (distance > light.range)
        return;

    // Normalize the light vector.
    light_vec /= distance;

    // Ambient term.
    ambient = ambient = mat.ambient * light.ambient_color;

    // Add diffuse and specular term, provided the surface is in 
    // the line of site of the light.

    float diffuse_factor = dot(light_vec, normal);

    // Flatten to avoid dynamic branching.
    [flatten]
    if (diffuse_factor > 0.0f)
    {
        float3 h = normalize(light_vec + to_eye);
        float  spec_factor = pow(max(dot(h, normal), 0.0f), mat.specular.w) * light.intensity;

        diffuse  = diffuse_factor * mat.diffuse * light.diffuse_color;
        specular = spec_factor * mat.specular * light.specular_color;
    }

    // Scale by spotlight factor and attenuate.
    float spot = pow(max(dot(-light_vec, light.direction), 0.0f), light.spot);

    // Scale by spotlight factor and attenuate.
    float att = spot / dot(light.attenuation, float3(1.0f, distance, distance * distance));

    ambient *= spot;
    diffuse *= att;
    specular *= att;
}

void CalculateCapsule(Material   mat, DynamicLight   light,
                      float3     pos, float3         normal, float3      to_eye,
                      out float4 ambient, out float4 diffuse, out float4 specular)
{
    // Initialize outputs.
    ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 to_capsule_start = pos - light.position;
    float  distance_on_line = dot(to_capsule_start, light.direction) / light.length;
    distance_on_line        = saturate(distance_on_line) * light.length;
    float3 point_on_line    = light.position + light.direction * distance_on_line;

    // The vector from the surface to the light.
    float3 light_vec = point_on_line - pos;

    // The distance from surface to light.
    float distance = length(light_vec);

    // Range test.
    if (distance > light.range)
        return;

    // Normalize the light vector.
    light_vec /= distance;

    // Ambient term.
    ambient = mat.ambient * light.ambient_color;

    // Add diffuse and specular term
    float diffuse_factor = dot(light_vec, normal);

    // Flatten to avoid dynamic branching.
    [flatten]
    if (diffuse_factor > 0.0f)
    {
        float3 h = normalize(light_vec + to_eye);
        float  spec_factor = pow(max(dot(h, normal), 0.0f), mat.specular.w) * light.intensity;

        diffuse  = diffuse_factor * mat.diffuse * light.diffuse_color;
        specular = spec_factor * mat.specular * light.specular_color;
    }

    // Attenuate
    float att = 1.0f / dot(light.attenuation, float3(1.0f, distance, distance * distance));
    diffuse *= att;
    specular *= att;
}
