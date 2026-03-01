varying vec2 v_uv;

void main()
{
    float tileX = floor(v_uv.x * 10.0);
    float tileY = floor(v_uv.y * 10.0);
    float checker = mod(tileX + tileY, 2.0);
    gl_FragColor = vec4(vec3(checker), 1.0);
}
