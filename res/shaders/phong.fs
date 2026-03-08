uniform vec3 u_camera_position;
uniform vec3 u_ambient_light;
uniform vec3 u_light_position;
uniform vec3 u_light_color;

uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;
uniform float u_shininess;

uniform sampler2D u_color_texture;
uniform sampler2D u_normal_texture;
uniform int u_use_color_texture;
uniform int u_use_specular_texture;
uniform int u_use_normal_texture;

uniform mat4 u_model;

varying vec3 v_world_pos;
varying vec3 v_normal;
varying vec2 v_uv;

void main()
{
    // --- Normal ---
    vec3 N = normalize(v_normal);
    if (u_use_normal_texture == 1) {
        vec3 tex_normal = texture2D(u_normal_texture, v_uv).xyz;
        tex_normal = tex_normal * 2.0 - 1.0;
        vec3 world_normal = vec3(
            dot(tex_normal, vec3(u_model[0].x, u_model[1].x, u_model[2].x)),
            dot(tex_normal, vec3(u_model[0].y, u_model[1].y, u_model[2].y)),
            dot(tex_normal, vec3(u_model[0].z, u_model[1].z, u_model[2].z))
        );
        N = normalize(mix(v_normal, world_normal, 0.5));
    }

    // --- Material coefficients ---
    vec3 Ka = u_Ka;
    vec3 Kd = u_Kd;
    vec3 Ks = u_Ks;

    if (u_use_color_texture == 1) {
        vec4 tex_color = texture2D(u_color_texture, v_uv);
        Ka = tex_color.rgb;
        Kd = tex_color.rgb;
    }

    if (u_use_specular_texture == 1) {
        vec4 tex_color = texture2D(u_color_texture, v_uv);
        Ks = vec3(tex_color.a);
    }

    // --- Lighting ---
    vec3 L = normalize(u_light_position - v_world_pos);
    vec3 V = normalize(u_camera_position - v_world_pos);
    vec3 R = reflect(-L, N);

    vec3 ambient  = Ka * u_ambient_light;
    vec3 diffuse  = Kd * max(dot(L, N), 0.0) * u_light_color;
    vec3 specular = Ks * pow(max(dot(R, V), 0.0), u_shininess) * u_light_color;

    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}
