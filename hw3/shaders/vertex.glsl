// Inputs from C++, these will be different
// for every vertex that this shader processes
attribute vec3 vertex;
attribute vec3 vertexnormal;

// Static inputs that are constant for all verticies
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec4 lightpos_world[10];
uniform int lightcount;

// Outputs to fragment shader
varying vec3 normal_camera;
varying vec3 eyedirection_camera;
varying vec4 lightdirection_camera[10];


void main() 
{
    // Calculate the Model-View-Projection matrix
    mat4 MVP = P * V * M;

    // Convert from model space to clip space
    gl_Position = MVP * vec4(vertex, 1);

    // Convert from model space to world space
    //pos_world = (M * vec4(vertex, 1)).xyz;

    // vector from vertex to camera, in camera space
    vec4 vpos_camera = V * M * vec4(vertex, 1.0);
    eyedirection_camera = normalize((vec4(0, 0, 0, 1) - vpos_camera).xyz);

    // vectors from vertex to lights in camera space
    for (int i=0; i<lightcount; i++) {
        vec4 lightpos_camera = V * lightpos_world[i];
        lightdirection_camera[i] = normalize(lightpos_camera - vpos_camera); 
    }

    // normal of the vertex in camera space
    normal_camera = normalize((V * M * vec4(vertexnormal, 0)).xyz);
}
