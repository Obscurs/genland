
uniform vec4 color;
uniform vec4 color2;
uniform float expand;
uniform vec2 center;
uniform float radius;
uniform float windowHeight;
uniform sampler2D texture;
void main(void)
{

    vec2 centerFromSfml = vec2(center.x, windowHeight - center.y);
    vec2 p = (gl_FragCoord.xy - centerFromSfml) / radius;
    float r = sqrt(dot(p, p));
    float res = float(r - expand)/float(1.0-expand);
    if(gl_Color == vec4(0,0,0,1)) gl_FragColor= color;
    else if (r < 1.0)
    {
        vec4 testColor = mix(color2, color, -res);
        gl_FragColor = mix(color, vec4(texture2D(texture, gl_TexCoord[0].xy)), res);
    }
    else
    {

        gl_FragColor = vec4(texture2D(texture, gl_TexCoord[0].xy));
        //gl_FragColor = vec4(0,0,0,1);
    }
}