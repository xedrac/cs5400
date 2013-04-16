varying vec4  position_ws;           // position of vertex/fragment in world space
varying vec3  normal_ws;             // normal of vertex/fragment in world space
varying vec2  UV;                    // UV coordinate for texturing

uniform int   lightswitch;
uniform int   textureswitch;

uniform vec4  camerapos_ws;          // camera position in world space
uniform vec4  lightpos_ws[5];        // light position(s) in world space
uniform vec4  lightdiffuse[5];       // light diffuse component(s)
uniform vec4  lightspecular[5];      // light spectular component(s)
uniform vec3  lightattenuation[5];   // light attenuation (constant, linear, quadratic)
uniform float lightspotcutoff[5];    // light spotlight cutoff
uniform float lightspotexponent[5];  // light spotlight exponent
uniform vec3  lightspotdirection[5]; // light spotlight direction
uniform int   lightcount;            // number of lights (max 5)
uniform float shininess;
uniform vec4  materialambient;
uniform vec4  materialdiffuse;
uniform vec4  materialspecular;
uniform vec3  sceneambient;          // ambient light in the scene
uniform mat4  V_inv;
uniform sampler2D texturesampler;



vec4 shade()
{
    vec3 normaldirection = normalize(normal_ws);
    vec3 viewdirection   = normalize((V_inv * camerapos_ws - position_ws).xyz);
    vec3 fragcolor       = vec3(0.0, 0.0, 0.0);

    for (int i=0; i<lightcount; i++) {
        vec3 lightdirection;
        float attenuation;

        // directional light?
        if (lightpos_ws[i].w == 0.0) {
            attenuation = 1.0;
            lightdirection = normalize(vec3(lightpos_ws[i]));

        // point light or other type
        } else {
            vec3 positiontolight = vec3(lightpos_ws[i] - position_ws); 
            float distance = length(positiontolight);
            lightdirection = normalize(positiontolight);

            float constattn  = lightattenuation[i].x;
            float linearattn = lightattenuation[i].y;
            float quadattn   = lightattenuation[i].z;
            attenuation = 1.0 / (constattn + linearattn*distance + quadattn*distance*distance);

            // spotlight?
            if (lightspotcutoff[i] <= 90.0) {
                float clampedcos = max(0.0, dot(-lightdirection, lightspotdirection[i]));

                // Outside the spotlight cone?
                if (clampedcos < cos(radians(lightspotcutoff[i]))) {
                    attenuation = 0.0;
                } else {
                    attenuation = attenuation * pow(clampedcos, lightspotexponent[i]);
                }
            }
        }
    
        vec3 ambient  = sceneambient * vec3(materialambient);
        vec3 diffuse  = attenuation * vec3(lightdiffuse[i]) * vec3(materialdiffuse) * max(0.0, dot(normaldirection, lightdirection));
        vec3 specular;

        // Make sure light source isn't on the wrong side
        if (dot(normaldirection, lightdirection) < 0.0) { 
            specular = vec3(0.0, 0.0, 0.0);
        } else {
            vec3 reflectdirection = reflect(-lightdirection, normaldirection);
            specular = attenuation * vec3(lightspecular[i]) * vec3(materialspecular) * pow(max(0.0, dot(reflectdirection, viewdirection)), shininess);
        }
        
        fragcolor += ambient + diffuse + specular;
    }

    return vec4(fragcolor, 1.0);
}

void main() 
{
    vec4 lightedcolor  = vec4(0,0,0,0);
    vec4 texturedcolor = vec4(0,0,0,0);

    if (lightswitch == 1)
        lightedcolor = shade();
    if (textureswitch == 1)
        texturedcolor = texture2D(texturesampler, UV).rgba;

    gl_FragColor = lightedcolor + texturedcolor;
}


