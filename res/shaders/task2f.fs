varying vec2 v_uv;
uniform sampler2D u_texture;

void main()
{
    vec2 offset = vec2(1.0 / 128.0, 1.0 / 128.0);

    vec3 blur = vec3(0.0);

    blur += texture2D(u_texture, v_uv + offset * vec2(-1,-1)).rgb;
    blur += texture2D(u_texture, v_uv + offset * vec2( 0,-1)).rgb;
    blur += texture2D(u_texture, v_uv + offset * vec2( 1,-1)).rgb;

    blur += texture2D(u_texture, v_uv + offset * vec2(-1, 0)).rgb;
    blur += texture2D(u_texture, v_uv).rgb;
    blur += texture2D(u_texture, v_uv + offset * vec2( 1, 0)).rgb;

    blur += texture2D(u_texture, v_uv + offset * vec2(-1, 1)).rgb;
    blur += texture2D(u_texture, v_uv + offset * vec2( 0, 1)).rgb;
    blur += texture2D(u_texture, v_uv + offset * vec2( 1, 1)).rgb;

    blur /= 9.0;

    gl_FragColor = vec4(blur, 1.0);
}