attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 result_color;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main() 
{
	gl_Position = P * V * M * vPosition;
    result_color = vColor;
}

