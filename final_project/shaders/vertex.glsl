attribute vec4 vertex_coord;
attribute vec3 vertex_normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat3 M_inv_transpose;

varying vec4 position_ws;
varying vec3 normal_ws;

void main() 
{
    position_ws = M * vertex_coord;
    normal_ws = normalize(M_inv_transpose * vertex_normal);

    gl_Position = P * V * M * vertex_coord;
}
