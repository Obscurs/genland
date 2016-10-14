
uniform vec4 color;
uniform vec4 color2;
uniform float expand;
uniform vec2 center;
uniform float radius;
uniform float windowHeight;
uniform sampler2D texture;
uniform sampler2D texture2;
void main(void)
{
    vec4 tex_current =vec4(texture2D(texture, gl_TexCoord[0].xy));;
    vec4 tex_old = vec4(texture2D(texture2, gl_TexCoord[0].xy));
    vec2 centerFromSfml = vec2(center.x, windowHeight - center.y);
    vec2 p = (gl_FragCoord.xy - centerFromSfml) / radius;
    float r = sqrt(dot(p, p));
    float res = float((r) - expand)/float(1.0-expand);
    if(gl_Color == vec4(0,0,0,1)) gl_FragColor= color;
    else if (r >= 1.0)
    {
        vec4 testColor = mix(color, vec4(1,1,1,1), res);

        vec4 light_without_color = mix(vec4(1,1,1,1), tex_current, res);
        vec4 light_with_color = mix(color,light_without_color,1-res*0.1);
        //vec4 light_with_color;
        //if(light_without_color==vec4(1,1,1,1)){
        //    light_with_color = vec4(1,1,1,1);
        //}
        //light_with_color = mix(light_without_color,color,0.5);

        gl_FragColor = max(tex_old,light_with_color);

    }
    else
        {
            //vec4 light_without_color = mix(vec4(1,1,1,1), tex_current, res);
            vec4 light_with_color = mix(color,tex_current,0.9);
            gl_FragColor = max(tex_old,light_with_color);
            //gl_FragColor = vec4(0,0,0,1);
        }

}