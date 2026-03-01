varying vec2 v_uv;

void main()
{
    float tiles = 20.0;
    vec2 uv = floor(v_uv * tiles) / tiles;

    vec3 topLeft     = vec3(0.0, 1.0, 0.0); // green
    vec3 topRight    = vec3(1.0, 1.0, 0.0); // yellow
    vec3 bottomLeft  = vec3(0.0, 0.0, 0.0); // black
    vec3 bottomRight = vec3(1.0, 0.0, 0.0); // red

    vec3 top    = mix(topLeft, topRight, uv.x);
    vec3 bottom = mix(bottomLeft, bottomRight, uv.x);
    vec3 color  = mix(bottom, top, uv.y);

    gl_FragColor = vec4(color, 1.0);
}
