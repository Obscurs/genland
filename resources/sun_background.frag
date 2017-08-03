uniform vec4 color;
uniform vec4 color2;
uniform float factor;
uniform float factor2;
uniform sampler2D texture;
void main(void)
{
        vec4 color_terrain = vec4(texture2D(texture, gl_TexCoord[0].xy));
        vec4 color_end=mix(color,color2,factor2);
        gl_FragColor = mix(color_terrain,color_end,factor);

}
