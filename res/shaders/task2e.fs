varying vec2 v_uv;
uniform sampler2D u_texture;

void main()
{
    vec4 texColor = texture2D(u_texture, v_uv);

    vec2 center = vec2(0.5, 0.5);
    float dist = distance(v_uv, center);

    float vignette = smoothstep(0.8, 0.4, dist);

    vec3 result = texColor.rgb * vignette;

    gl_FragColor = vec4(result, 1.0);
}