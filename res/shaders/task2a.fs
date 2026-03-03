varying vec2 v_uv;
uniform sampler2D u_texture;

void main()
{
    vec4 texColor = texture2D(u_texture, v_uv);   // 🔥 MISSING LINE

    float gray = dot(texColor.rgb, vec3(0.299, 0.587, 0.114));
    gl_FragColor = vec4(vec3(gray), 1.0);
}
