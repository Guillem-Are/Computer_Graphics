// raster.vs
attribute vec3 gl_Vertex;
attribute vec3 gl_Normal;
attribute vec2 gl_MultiTexCoord0;

uniform mat4 u_model;
uniform mat4 u_viewprojection;

varying vec2 v_uv;
varying vec3 v_world_normal;
varying vec3 v_world_position;

void main()
{
    // Pass UVs to fragment shader
    v_uv = gl_MultiTexCoord0.xy;

    // Transform to world space
    vec3 world_position = (u_model * vec4(gl_Vertex, 1.0)).xyz;
    vec3 world_normal = (u_model * vec4(gl_Normal, 0.0)).xyz;

    v_world_position = world_position;
    v_world_normal = world_normal;

    // Project to screen
    gl_Position = u_viewprojection * vec4(world_position, 1.0);
}