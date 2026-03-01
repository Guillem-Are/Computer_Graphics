varying vec2 v_uv;

void main()
{
    float h = step(0.9, sin(v_uv.x * 40.0) * 0.5 + 0.5);
    float v = step(0.9, sin(v_uv.y * 40.0) * 0.5 + 0.5);
    vec3 color = vec3(h, 0.0, v);
    gl_FragColor = vec4(color, 1.0);
}
