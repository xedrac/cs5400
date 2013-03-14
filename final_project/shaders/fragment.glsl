
varying vec3 normal_camera;
varying vec3 eyedirection_camera;
varying vec4 lightdirection_camera[10];

uniform vec4 lightambient[10];
uniform vec4 lightdiffuse[10];
uniform vec4 lightspecular[10];
uniform int lightcount;

uniform float shininess;
uniform vec4 materialambient;
uniform vec4 materialdiffuse;
uniform vec4 materialspecular;



vec4 shade()
{
    vec4 N = vec4(normalize(normal_camera), 1.0);  
    vec4 E = vec4(normalize(eyedirection_camera), 1.0);

    vec4 outcolor = vec4(0.0, 0.0, 0.0, 1.0);

    for (int i=0; i<lightcount; i++) {
        vec4 L = normalize(-lightdirection_camera[i]);
        vec4 H = normalize(L + -E);

        float kd = max(dot(L, N), 0.0);
        float ks = pow(max(dot(N, H), 0.0), shininess);
        vec4 ambient  = lightambient[i] * materialambient;
        vec4 diffuse  = kd * (lightdiffuse[i] * materialdiffuse);
        vec4 specular = ks * (lightspecular[i] * materialspecular);

        if (dot(L, N) < 0.0) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }

        outcolor += ambient + diffuse + specular;
    }

    return outcolor;
}


void main() 
{
    gl_FragColor = shade();
}


