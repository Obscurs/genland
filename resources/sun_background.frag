uniform float windowHeight;
uniform vec4 color;
uniform vec4 color2;
uniform float factor;
uniform float factor2;
uniform sampler2D texture;
void main(void)
{
        //float factor3=gl_FragCoord.y/windowHeight;
        //float factor_end = min(0.0,factor3-factor2);
        vec4 color_terrain = vec4(texture2D(texture, gl_TexCoord[0].xy));
        vec4 color_end=mix(color,color2,factor2);
        gl_FragColor = mix(color_terrain,color_end,factor);

}
/*
vec4 color_sky = vec4(texture2D(texture2, gl_TexCoord[0].xy));
        vec4 color_terrain = vec4(texture2D(texture, gl_TexCoord[0].xy));

        vec4 color_end=mix(color,color2,factor2);
        gl_FragColor = gl_Color;
        if(color_sky == vec4(0,0,0,1)) gl_FragColor= vec4(0,0,0,1);

        else if(color_sky.z<0.85) gl_FragColor = mix(vec4(0,0,0,1),mix(color_terrain,color_end,factor),color_sky.z);
        else gl_FragColor = mix(color_terrain,color_end,factor);*/