#version 450 core

/*-------------------------------------------------------
File Name: model_shader.frag
Purpose: 
	Fragment shader for models.
Language: GLSL
Platform: VS 22 / version 16.9.0 / Windows
Project: Cookie_Engine
Author: Sunwoo Lee / sunwoo.lee
Creation date: 08.26.2025
--------------------------------------------------------*/


uniform vec3 light_pos;
uniform vec3 light_color = vec3(1.0, 1.0, 1.0);
uniform vec3 viewPos = vec3(0.0, 0.0, 10.0);

uniform vec4 color;
uniform sampler2D tex;

in vec2 UV;
in vec3 NRM;

in vec3 frag_pos;
out vec4 outColor;

void main(void)
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light_color;

    // diffuse
    vec3 norm = normalize(NRM);
    vec3 lightDir = normalize(light_pos - frag_pos);
    float diff = max(dot(norm,lightDir),0.);//brightness
    vec3 diffuse = diff * light_color;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - frag_pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light_color;  


    if(color.a > 0.f)
    {
        outColor = vec4( ( ambient + diffuse + specular  ) * color.xyz, 1.0);    //+ specular
    }
    else
    {
        outColor = vec4(color.xyz, 1.0);
    }
    
    //outColor = vec4( normalize(NRM), 1.0 );
}