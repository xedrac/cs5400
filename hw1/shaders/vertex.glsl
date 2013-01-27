// Heavily referenced from the excellent tutorial at:
// http://www.opengl-tutorial.org
// Which really helped me understand the purpose of both the
// vertex shader and the fragment shader and how to communicate
// between them


// Dynamic inputs from C++, these will be different
// for every vertex that this shader processes
in vec3 vpos_model;
in vec3 vnormal_model;

// Static inputs that are constant for all verticies
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightpos_world;

// Outputs to fragment shader
out vec3 pos_world;
out vec3 normal_camera;
out vec3 eyedirection_camera;
out vec3 lightdirection_camera;

void main() 
{
    // Convert from model space to clip space
    gl_Position = MVP * vec4(vpos_model, 1);

    // Convert from model space to world space
    pos_world = (M * vec4(vpos_model, 1)).xyz;

    // vector from vertex to camera, in camera space
    vec3 vpos_camera = (V * M * vec4(vpos_model, 1)).xyz;
    eyedirection_camera = vec3(0, 0, 0) - vpos_camera;

    // vector from vertex to light in camera space
    vec3 lightpos_camera = (V * vec4(lightpos_world, 1)).xyz;
    lightdirection_camera = normalize(lightpos_camera + eyedirection_camera);

    // normal of the vertex in camera space
    normal_camera = normalize((V * M * vec4(vnormal_model, 0)).xyz);
}
