#version 120

// Input vertex data, different for all executions of this shader.
attribute vec3 sg_Vertex;

varying vec4 Position_screenspace;

// Values that stay constant for the whole mesh.
uniform mat4 sg_MVPMatrix;

void main(){
	gl_Position = sg_MVPMatrix * vec4(sg_Vertex,1);
	Position_screenspace = gl_Position;
}

