#version 450

in vec3 vecPos;
in vec2 Tex;

uniform sampler2D textura;

layout(location = 0) out vec4 FragColor;



void main()
{
	FragColor = vec4(texture(textura,Tex.xy).bgr,1.0);
	//FragColor = vec4(Tex.xy, 1.0,1.0);
	//FragColor = vec4(1.0, 1.0, 1.0,1.0);
}
