varying vec2 v_uv;
uniform sampler2D u_texture;

void main()
{
    vec4 texColor = texture2D(u_texture, v_uv);

    // Convert original color to brightness (0 = black, 1 = white)
    float brightness = dot(texColor.rgb, vec3(0.299, 0.587, 0.114));

    // Map brightness to yellow shades (black → dark yellow → bright yellow)
    vec3 yellowShade = vec3(1.0, 1.0, 0.0) * brightness;

    gl_FragColor = vec4(yellowShade, 1.0);
}