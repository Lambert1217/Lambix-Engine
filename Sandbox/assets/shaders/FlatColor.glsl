#type vertex
#version 330 core
			
layout(location = 0) in vec3 aPos;

uniform mat4 aViewProjection;
uniform mat4 aTransform;

void main()
{
    gl_Position = aViewProjection * aTransform * vec4(aPos, 1.0f);
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
    FragColor = u_Color;
}