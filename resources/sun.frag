
uniform sampler2D texture;
void main(void)
{
    vec4 tex_current =vec4(texture2D(texture, gl_TexCoord[0].xy));
        if(gl_Color == vec4(0,0,1,1) || gl_Color == vec4(1,0,0,1)) gl_FragColor= gl_Color;
        else gl_FragColor = tex_current;


}