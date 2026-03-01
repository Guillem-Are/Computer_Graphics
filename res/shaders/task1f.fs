varying vec2 v_uv;

void main()
{
    float wave = sin(v_uv.x * 6.0) * 0.45 + 0.5;
    float dist = abs(v_uv.y - wave);
    float intensity = 1.0 - smoothstep(0.0, 0.3, dist);
    float background = 0.15; // dark green background
    vec3 color = vec3(0.0, max(intensity, background), 0.0);
    gl_FragColor = vec4(color, 1.0);
}
