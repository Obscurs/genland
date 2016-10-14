
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
    float res = float((r/4.0) - expand)/float(1.0-expand);
    if(gl_Color == vec4(0,0,0,1)) gl_FragColor= color;
    else
    {
        vec4 testColor = mix(color, color2, res);
        gl_FragColor = max(tex_old,mix(testColor, tex_current, res));

    }

}