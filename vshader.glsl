#version 130

in vec4 vPosition;
in vec4 vColor;
out vec4 color;

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;

void main()
{
	color = vColor;
	gl_Position = projection_matrix * model_view_matrix * vPosition / vPosition.w;
}