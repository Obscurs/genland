

uniform sampler2D texture;
uniform sampler2D texture2;
void main(void)
{


        vec4 color_terrain = vec4(texture2D(texture, gl_TexCoord[0].xy));
        vec4 color_sky = vec4(texture2D(texture2, gl_TexCoord[0].xy));
        //color_sky.w = color_terrain.w;
        if(color_terrain.w <1.0 ){
            vec3 color_aux = mix(color_sky.xyz,color_terrain.xyz,color_terrain.w);
            gl_FragColor = vec4(color_aux.xyz,1.0);
        }
        else gl_FragColor = color_terrain;
        //gl_FragColor = color_sky;
        //gl_FragColor = color_terrain;

}
/*
vec4 color_sky = vec4(texture2D(texture2, gl_TexCoord[0].xy));
        vec4 color_terrain = vec4(texture2D(texture, gl_TexCoord[0].xy));

        vec4 color_end=mix(color,color2,factor2);
        gl_FragColor = gl_Color;
        if(color_sky == vec4(0,0,0,1)) gl_FragColor= vec4(0,0,0,1);

        else if(color_sky.z<0.85) gl_FragColor = mix(vec4(0,0,0,1),mix(color_terrain,color_end,factor),color_sky.z);
        else gl_FragColor = mix(color_terrain,color_end,factor);*/