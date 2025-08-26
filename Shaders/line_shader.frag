#version 450 core

/*-------------------------------------------------------
File Name: line_shader.frag
Purpose: 
	Fragment shader for lines.
Language: GLSL
Platform: VS 22 / version 16.9.0 / Windows
Project: Cookie_Engine
Author: Sunwoo Lee / sunwoo.lee
Creation date: 08.26.2025
--------------------------------------------------------*/

uniform vec4 color;

out vec4 outColor;

void main () {
  outColor = vec4(color.xyz, 1.0);
}