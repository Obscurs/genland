/*uniform sampler2D texture;
//uniform float blur_radius;
uniform float min;
uniform float mult;
void main()
{

    //float xxx = min*mult;

    //vec2 offx = vec2(blur_radius, 0.0);
    //vec2 offy = vec2(0.0, blur_radius);
    */
/*
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy)               * 4.0 +
                 texture2D(texture, gl_TexCoord[0].xy - offx)        * 2.0 +
                 texture2D(texture, gl_TexCoord[0].xy + offx)        * 2.0 +
                 texture2D(texture, gl_TexCoord[0].xy - offy)        * 2.0 +
                 texture2D(texture, gl_TexCoord[0].xy + offy)        * 2.0 +
                 texture2D(texture, gl_TexCoord[0].xy - offx - offy) * 1.0 +
                 texture2D(texture, gl_TexCoord[0].xy - offx + offy) * 1.0 +
                 texture2D(texture, gl_TexCoord[0].xy + offx - offy) * 1.0 +
                 texture2D(texture, gl_TexCoord[0].xy + offx + offy) * 1.0;

    gl_FragColor =  gl_Color * (pixel / 16.0);
    */
    /*
    //vec4 pixel
    //gl_FragColor = vec4(texture2D(texture, gl_TexCoord[0].xy).xyz *(1-(gl_TexCoord[0].y)), 1);
    gl_FragColor = vec4(texture2D(texture, gl_TexCoord[0].xy));
    //gl_FragColor = vec4(vec3(1,1,1)*((gl_TexCoord[0].y+1)/2), 1);
    //gl_FragColor = vec4(vec3((uv.y-min)*mult),1.0);
}
*/

uniform sampler2D texture;
uniform sampler2D texture2;
void main(void)
{
    vec4 color1 = vec4(texture2D(texture, gl_TexCoord[0].xy));
    vec4 color2 = vec4(texture2D(texture2, gl_TexCoord[0].xy));
    vec4 color_resta = color1-color2;
    float error_max = 0.1;
    float factor=color_resta.x/error_max;

    if(color_resta.x ==0.0 || color_resta.y==0.0 ) gl_FragColor = vec4(0,0,0,1);
    else gl_FragColor = mix( vec4(0,0,0,1),color1, min(factor, 1));
    //else gl_FragColor = color1;

}