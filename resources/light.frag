
uniform vec4 color;
uniform float expand;
uniform vec2 center;
uniform float radius;
uniform float windowHeight;
uniform sampler2D texture;
uniform sampler2D textureOld;
uniform sampler2D textureSun;
void main(void)
{
    vec4 tex_current =vec4(texture2D(texture, gl_TexCoord[0].xy));
    vec4 tex_old = vec4(texture2D(textureOld, gl_TexCoord[0].xy));
    vec4 tex_sun = vec4(texture2D(textureSun, gl_TexCoord[0].xy));
    vec2 centerFromSfml = vec2(center.x, windowHeight - center.y);
    float dist = distance(gl_FragCoord.xy, centerFromSfml)-radius;
    float factor = dist/expand;
    if(tex_sun.z == 1.0 || tex_old.w < 1.0) gl_FragColor = tex_old;
    else if(gl_Color == vec4(0,0,0,0)) gl_FragColor = color;
    else{
        vec4 colorEnd;
        if (dist >= 0.0)
        {
            vec4 light_with_color = mix(color,tex_current,0.8);
            vec4 light_with_color2 = mix( light_with_color,tex_old, min(factor,1.0));
            colorEnd = max(tex_old,light_with_color2);

        }
        else
        {
            vec4 light_with_color = mix(color,tex_current,0.8);
            colorEnd = max(tex_old,light_with_color);

        }



        gl_FragColor = colorEnd;

    }


}
