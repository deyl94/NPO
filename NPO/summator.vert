uniform float k;

void main()
{
    vec4 a = gl_Vertex;
    gl_Position = gl_ModelViewProjectionMatrix * (a + vec4(gl_Normal * k, 0));
    gl_FrontColor = gl_Color;
}
