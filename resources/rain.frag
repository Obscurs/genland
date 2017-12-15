uniform float windowHeight;
uniform float windowWidth;
uniform sampler2D texture;
uniform float time;
const int LAYERS = 6;             // number of layers of drops
uniform float intensity;
uniform float SCALE;        // overall scale of the drops
uniform float LENGTH;        // length of the drops
uniform float LENGTH_SCALE;   // how much the drop length changes every layer

uniform float ALPHA;
uniform float SPEED;          // how fast the drops fall
uniform bool is_snow;
const vec3 DROP_COLOR = vec3(0.30, 0.5, 0.64);
const vec3 DROP_COLOR2 = vec3(0.8, 0.8, 0.9);
const float FADE = 0.6;           // how much the drops fade every layer

float rand(vec2 co)
{
	float a = 12.9898;
	float b = 78.233;
	float c = 43758.5453;
	float dt = dot(co.xy, vec2(a, b));
	float sn = mod(dt, 3.14);

	return fract(sin(sn) * c);
}

float rainFactor(vec2 uv, float scale, float dripLength, vec2 offset, float cutoff)
{
    vec2 pos = uv * vec2(scale, scale / dripLength) + offset;
    vec2 dripOffset = vec2(0.0, floor(rand(floor(pos * vec2(1.0, 0.0))) * (dripLength - 0.0001)) / dripLength);
    float f = rand(floor(pos + dripOffset));

    return step(cutoff, f);
}

vec4 over(vec4 a, vec4 b)
{
    return vec4(mix(b, a, a.a).rgb, max(a.a, b.a));
}
void main(void)
{
        //float factor3=gl_FragCoord.y/windowHeight;
        //float factor_end = min(0.0,factor3-factor2);
        vec3 BG_COLOR = vec3(texture2D(texture, gl_TexCoord[0].xy).xyz);
        //BG_COLOR = vec3(0.23, 0.38, 0.6);
        //gl_FragColor = color_terrain;
        //gl_FragColor = color_terrain;

    vec2 uv = gl_FragCoord.xy / vec2(windowWidth,windowHeight);
        float aspect = windowWidth/ windowHeight;
        uv.x *= aspect;

        vec4 finalColor = vec4(0.0);

        float dropLength = LENGTH;
        float alpha = 1.0;

        for (int i = 0; i < LAYERS; i++)
        {
            float f = rainFactor(uv, SCALE, dropLength, vec2(SCALE * float(i), time * SPEED), 1.0-intensity*0.1/2.0);
            vec4 color;
            if(is_snow) color = vec4(DROP_COLOR2, f* alpha-1.0+ALPHA);
            else color = vec4(DROP_COLOR, f* alpha-1.0+ALPHA);

            finalColor = over(finalColor, color);

            dropLength *= LENGTH_SCALE;
            alpha *= FADE;
        }

        finalColor = over(finalColor, vec4(BG_COLOR, 0.1));

    	gl_FragColor = finalColor;
}





