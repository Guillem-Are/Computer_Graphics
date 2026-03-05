varying vec2 v_uv;
uniform sampler2D u_texture;
uniform float u_time;

void main()
{
    // Center coords
    vec2 center = vec2(0.5, 0.5);

    // Translate UV to center
    vec2 uv = v_uv - center;

    // Rotation angle over time
    float angle = u_time;  // radians
    float cosA = cos(angle);
    float sinA = sin(angle);

    // Rotate UV
    uv = vec2(
        uv.x * cosA - uv.y * sinA,
        uv.x * sinA + uv.y * cosA
    );

    // Translate back
    uv += center;

    // Sample texture
    vec4 texColor = texture2D(u_texture, uv);

    gl_FragColor = texColor;
}