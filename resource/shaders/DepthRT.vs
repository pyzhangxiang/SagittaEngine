#version 120

// Input vertex data, different for all executions of this shader.
attribute vec3 sg_Vertex;

varying vec4 Position_screenspace;
//varying vec4 Position_worldspace;

// Values that stay constant for the whole mesh.
uniform mat4 sg_MVPMatrix;
//uniform mat4 sg_MVMatrix;

void main(){
	gl_Position = sg_MVPMatrix * vec4(sg_Vertex,1);
	Position_screenspace = gl_Position;
    Position_screenspace.x = Position_screenspace.x * 0.5 + 0.5;
    Position_screenspace.y = Position_screenspace.y * 0.5 + 0.5;
    Position_screenspace.z = Position_screenspace.z * 0.5 + 0.5;
    
 //   Position_worldspace = sg_Vertex;
}

