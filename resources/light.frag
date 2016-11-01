
uniform vec4 color;
uniform float expand;
uniform vec2 center;
uniform float radius;
uniform float windowHeight;
uniform sampler2D texture;
uniform sampler2D texture2;
void main(void)
{
    vec4 tex_current =vec4(texture2D(texture, gl_TexCoord[0].xy));
    vec4 tex_old = vec4(texture2D(texture2, gl_TexCoord[0].xy));
    vec2 centerFromSfml = vec2(center.x, windowHeight - center.y);
    //vec2 p = (gl_FragCoord.xy - centerFromSfml) / radius;
    float dist = distance(gl_FragCoord.xy, centerFromSfml)-radius;
    float factor = dist/expand;
    //float r = sqrt(dot(p, p));
    //float res = float((r) - expand)/float(1.0-expand);
    if(gl_Color == vec4(0,0,0,0)) gl_FragColor= color;
    //else if(tex_old.w>0.0)gl_FragColor= vec4(0,1,0,1);
    else if (dist >= 0.0)
    {

        //vec4 light_without_color = mix(vec4(1,1,1,1), tex_current, res);
        //vec4 light_with_color = mix(color,light_without_color,1.0-res*0.1);
        vec4 light_with_color = mix(color,tex_current,0.8);
        vec4 light_with_color2 = mix( light_with_color,tex_old, min(factor,1.0));


        gl_FragColor = max(tex_old,light_with_color2);

    }
    else
        {
            vec4 light_with_color = mix(color,tex_current,0.8);
            gl_FragColor = max(tex_old,light_with_color);
            //gl_FragColor = max(tex_old,tex_current);
        }

}
