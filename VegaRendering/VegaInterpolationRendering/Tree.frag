//#version 410 core
//out vec4 FragColor;
//
//in vec2 TexCoords;
//
//uniform sampler2D texture_diffuse1;
//
//void main()
//{    
//    //FragColor = texture(texture_diffuse1, TexCoords);
//   FragColor = texture(texture_diffuse1, TexCoords);
//}

#version 430 core
out vec4 FragColor;

in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0f);
}