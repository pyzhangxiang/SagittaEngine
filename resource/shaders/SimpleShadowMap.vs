
// Input vertex data, different for all executions of this shader.
attribute vec3 sg_Vertex;	// in model space
attribute vec2 sg_UV0;
attribute vec3 sg_Normal;

// Output data ; will be interpolated for each fragment.
varying vec2 UV0;
varying vec3 Position_worldspace;
//varying vec3 vertexLightColor;
varying vec4 depthCoord;

varying vec3 normal_viewspace;
varying vec3 eyedir_viewspace;
varying vec3 lightdir_viewspace;

// Values that stay constant for the whole mesh.
uniform mat4 sg_MVPMatrix;
uniform mat4 sg_MVMatrix;
uniform mat4 sg_ViewMatrix;
uniform mat4 sg_ModelMatrix;
uniform mat4 sg_NormalMVMatrix;
uniform mat4 depthMVP;


uniform vec3 sg_Light0_Position;	// in world space


void main(){

	// Output position of the vertex, in clip space : sg_MVPMatrix * position
	gl_Position =  sg_MVPMatrix * vec4(sg_Vertex, 1.0);
	depthCoord = depthMVP * vec4(sg_Vertex, 1.0);
	Position_worldspace = (sg_ModelMatrix * vec4(sg_Vertex, 1.0)).xyz;

	// Normal of the the vertex, in camera space
	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	normal_viewspace = ( sg_NormalMVMatrix * vec4(sg_Normal, 0.0)).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	eyedir_viewspace = ( sg_MVMatrix * vec4(sg_Vertex, 1.0)).xyz;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 lightpos_viewspace = ( sg_ViewMatrix * vec4(sg_Light0_Position, 1.0) ).xyz;
	lightdir_viewspace = lightpos_viewspace - eyedir_viewspace;

	
	//vertexLightColor = (ambient + diffuse + specular).xyz;
	
	
	// UV of the vertex. No special space for this one.
	UV0 = sg_UV0;
	
}

