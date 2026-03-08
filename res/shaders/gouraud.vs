uniform mat4 u_model;
uniform mat4 u_viewprojection;

uniform vec3 u_camera_position;
uniform vec3 u_ambient_light;
uniform vec3 u_light_position;
uniform vec3 u_light_color;

uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;
uniform float u_shininess;

varying vec3 v_color;

void main()
{
    vec3 world_pos = (u_model * gl_Vertex).xyz;

    vec3 N = normalize(vec3(
        dot(gl_Normal, vec3(u_model[0].x, u_model[1].x, u_model[2].x)),
        dot(gl_Normal, vec3(u_model[0].y, u_model[1].y, u_model[2].y)),
        dot(gl_Normal, vec3(u_model[0].z, u_model[1].z, u_model[2].z))
    ));

    vec3 L = normalize(u_light_position - world_pos);
    vec3 V = normalize(u_camera_position - world_pos);
    vec3 R = reflect(-L, N);

    vec3 ambient  = u_Ka * u_ambient_light;
    vec3 diffuse  = u_Kd * max(dot(L, N), 0.0) * u_light_color;
    vec3 specular = u_Ks * pow(max(dot(R, V), 0.0), u_shininess) * u_light_color;

    v_color = ambient + diffuse + specular;

    gl_Position = u_viewprojection * u_model * gl_Vertex;
}
