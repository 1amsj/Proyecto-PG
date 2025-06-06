#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform float darkness;
uniform sampler2D texture_diffuse1;
uniform int isNight;
uniform int isMoon;

vec3 rgb2hsv(vec3 c) {
    vec4 K = vec4(0.0, -1.0/3.0, 2.0/3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}
vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    vec3 hsv = rgb2hsv(texColor.rgb);
    hsv.y = clamp(hsv.y * 1.5, 0.0, 1.0); 
    vec3 saturated = hsv2rgb(hsv);
    vec3 color = vec3(saturated);

    if(isNight == 1) {
        bool isWindow = all(lessThan(texColor.rgb, vec3(0.2, 0.2, 0.25)));
        if(isWindow) {
            color = vec3(1.0, 0.95, 0.6);
        } else {
            color *= 0.3;
        }
    }

    
    if(isMoon == 1) {
        color = min(texColor.rgb * 2.0, vec3(1.0)); 
    }

   if(isNight == 0) {
    color = mix(color, color * 0.3, darkness);
    }

    FragColor = vec4(color, texColor.a);
}