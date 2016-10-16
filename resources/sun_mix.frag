
uniform vec4 color;
uniform float factor;
uniform sampler2D texture;
uniform sampler2D texture2;
void main(void)
{

        vec4 color_sky = vec4(texture2D(texture2, gl_TexCoord[0].xy));
        vec4 color_terrain = vec4(texture2D(texture, gl_TexCoord[0].xy));
        if(color_sky == vec4(0,0,0,1)) gl_FragColor= vec4(0,0,0,1);
        else if(color_sky.z<0.85) gl_FragColor = mix(vec4(0,0,0,1),mix(color_terrain,color,factor),color_sky.z);
        else gl_FragColor = mix(color_terrain,color,factor);


}
