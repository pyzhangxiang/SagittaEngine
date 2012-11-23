
// Input vertex data, different for all executions of this shader.
attribute vec3 sg_Vertex;	// in model space
attribute vec2 sg_UV0;
attribute vec3 sg_Normal;

// Output data ; will be interpolated for each fragment.
varying vec2 UV0;
varying vec3 Position_worldspace;
varying vec3 Normal_cameraspace;
varying vec3 EyeDirection_cameraspace;
varying vec3 LightDirection_cameraspace;
varying vec3 vertexColor;
varying vec4 depthCoord;

// Values that stay constant for the whole mesh.
uniform mat4 sg_MVPMatrix;
uniform mat4 sg_ViewMatrix;
uniform mat4 sg_ModelMatrix;
uniform mat4 depthBiasMVP;

// in world space
uniform vec3 sg_Light0_Position;	

void main(){

	// Output position of the vertex, in clip space : sg_MVPMatrix * position
	gl_Position =  sg_MVPMatrix * vec4(sg_Vertex,1);

	depthCoord = depthBiasMVP * vec4(sg_Vertex,1);
	
}

