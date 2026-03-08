varying vec3 v_color;  // interpolated color from vertex shader

void main()
{
    gl_FragColor = vec4(v_color, 1.0);
}
