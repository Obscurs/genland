uniform sampler2D texture;
//uniform float blur_radius;
uniform float min;
uniform float mult;
void main()
{

    //float xxx = min*mult;

    //vec2 offx = vec2(blur_radius, 0.0);
    //vec2 offy = vec2(0.0, blur_radius);
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
    //vec4 pixel
    gl_FragColor = vec4(texture2D(texture, gl_TexCoord[0].xy).xyz *(1-(gl_TexCoord[0].y-min)*mult), 1);
    //gl_FragColor = vec4(vec3(1,1,1)*((gl_TexCoord[0].y+1)/2), 1);
    //gl_FragColor = vec4(vec3((uv.y-min)*mult),1.0);
}