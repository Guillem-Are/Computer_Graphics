varying vec2 v_uv;
uniform float u_aspect_ratio;

void main()
{
    vec2 uv = vec2((v_uv.x - 0.5) * u_aspect_ratio + 0.5, v_uv.y);
    float d = distance(uv, vec2(0.5, 0.5));
    float brightness = d * 1.5;
    gl_FragColor = vec4(vec3(brightness), 1.0);
}
