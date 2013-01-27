
in vec3 pos_world;
in vec3 normal_camera;
in vec3 eyedirection_camera;
in vec3 lightdirection_camera;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightpos_world;

out vec3 color;


// reference: http://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
//float rand2(vec2 v)
//{
//    return fract(sin(dot(v.xy ,vec2(12.9898,78.233))) * 43758.5453);
//}

void main() 
{
    vec3 lightcolor   = vec3(1.0, 1.0, 1.0);
    vec3 surfacecolor = vec3(0.5, 0.2, 0.2);
    vec3 ambientcolor = vec3(0.2, 0.2, 0.2);
    float lightdistance = length(lightpos_world - pos_world);
    float lightpower  = 10.0f;

    vec3 normal = normalize(normal_camera);
    vec3 light  = normalize(lightdirection_camera);
    float costheta = max(0.0, clamp(dot(normal, light), 0, 1));
    
    //vec3 eye = normalize(eyedirection_camera);
    //vec3 reflect = reflect(-light, normal);
    //float cosalpha = clamp(dot(eye, reflect), 0, 1);

    color = ambientcolor + surfacecolor * lightcolor * lightpower * costheta / (lightdistance*lightdistance);
    //color = surfacecolor * costheta;
    //color = vec3(1.0, 0.0, 0.0);
    
    //float r = 255.0 * rand(gl_FragCoord.xy);
    //float g = 255.0 * rand(gl_FragCoord.yz);
    //float b = 255.0 * rand(gl_FragCoord.xz);
    //float gray = 0.3*r + 0.59*g + 0.11*b;
    //float gray = rand2(gl_FragCoord.xy);
    //gl_FragColor[0] = gray;
    //gl_FragColor[1] = gray;
    //gl_FragColor[2] = gray;
}
