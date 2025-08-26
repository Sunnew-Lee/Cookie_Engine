#version 450 core

/*-------------------------------------------------------
File Name: line_shader.vert
Purpose: 
	Vertex shader for lines.
Language: GLSL
Platform: VS 22 / version 16.9.0 / Windows
Project: Cookie_Engine
Author: Sunwoo Lee / sunwoo.lee
Creation date: 08.26.2025
--------------------------------------------------------*/

layout (location=0) in vec3 pos;

uniform mat4 projection;
uniform mat4 view;

void main() {
  gl_Position = projection * view  * vec4(pos, 1.0);
}
