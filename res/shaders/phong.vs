uniform mat4 u_model;
uniform mat4 u_viewprojection;

varying vec3 v_world_pos;
varying vec3 v_normal;
varying vec2 v_uv;

void main()
{
    v_world_pos = (u_model * gl_Vertex).xyz;

    v_normal = vec3(
        dot(gl_Normal, vec3(u_model[0].x, u_model[1].x, u_model[2].x)),
        dot(gl_Normal, vec3(u_model[0].y, u_model[1].y, u_model[2].y)),
        dot(gl_Normal, vec3(u_model[0].z, u_model[1].z, u_model[2].z))
    );

    v_uv = gl_MultiTexCoord0.xy;

    gl_Position = u_viewprojection * u_model * gl_Vertex;
}
