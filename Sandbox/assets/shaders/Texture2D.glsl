#type vertex
#version 330 core
			
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

uniform mat4 aViewProjection;
uniform mat4 aTransform;

void main()
{
    vTexCoord = aTexCoord;
    gl_Position = aViewProjection * aTransform * vec4(aPos, 1.0f);
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 FragColor;

in vec2 vTexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    FragColor = texture(u_Texture, vTexCoord) * u_Color;
}