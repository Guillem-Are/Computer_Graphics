varying vec2 v_uv;
uniform sampler2D u_texture;
uniform float u_time;

void main()
{
    vec2 uv = v_uv;

    // Distort UV with sine waves
    uv.x += 0.05 * sin(10.0 * uv.y + u_time);
    uv.y += 0.05 * cos(10.0 * uv.x + u_time);

    // Clamp UV
    uv = clamp(uv, 0.0, 1.0);

    // Sample texture
    vec4 texColor = texture2D(u_texture, uv);

    gl_FragColor = texColor;
}