varying vec2 v_uv;

void main()
{
    float wave = sin(v_uv.x * 6.0) * 0.45 + 0.5;
    
    // Gradients
    vec3 colorTop = vec3(0.0, 1.0 - v_uv.y, 0.0);
    vec3 colorBottom = vec3(0.0, v_uv.y, 0.0);
    
    // Mask: 1 where v_uv.y > wave
    float mask = step(wave, v_uv.y);
    
    // Mix colors
    gl_FragColor = vec4(mix(colorBottom, colorTop, mask), 1.0);
}
