#version 450 core

/*-------------------------------------------------------
File Name: model_shader.vert
Purpose: 
	Vertex shader for models.
Language: GLSL
Platform: VS 22 / version 17.14.11 / Windows
Project: Cookie_Engine
Author: Sunwoo Lee / sunwoo.lee
Creation date: 08.26.2025
--------------------------------------------------------*/

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 mapping;

out vec2 UV;
out vec3 NRM;
out vec3 frag_pos;

void main(void) 
{
    gl_Position = projection * view * model * mapping * vec4(pos, 1.0);
    frag_pos = vec3(model * mapping * vec4(pos, 1.0));

    UV = uv;
    NRM = mat3(transpose(inverse(model))) * nrm;
}