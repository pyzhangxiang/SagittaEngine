
// Input vertex data, different for all executions of this shader.
attribute vec3 sg_Vertex;	// in model space
//attribute vec2 sg_UV0;
attribute vec3 sg_Normal;

// Output data ; will be interpolated for each fragment.
varying vec2 UV;
varying vec3 Position_worldspace;
varying vec3 Normal_cameraspace;
varying vec3 EyeDirection_cameraspace;
varying vec3 LightDirection_cameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 sg_MVPMatrix;
uniform mat4 sg_ViewMatrix;
uniform mat4 sg_ModelMatrix;
// in world space
uniform vec3 sg_Light0_Position;	

void main(){

	// Output position of the vertex, in clip space : sg_MVPMatrix * position
	gl_Position =  sg_MVPMatrix * vec4(sg_Vertex,1);
	
	// Position of the vertex, in worldspace : sg_ModelMatrix * position
	Position_worldspace = (sg_ModelMatrix * vec4(sg_Vertex,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( sg_ViewMatrix * sg_ModelMatrix * vec4(sg_Vertex,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( sg_ViewMatrix * vec4(sg_Light0_Position, 1.0) ).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	Normal_cameraspace = ( sg_ViewMatrix * sg_ModelMatrix * vec4(sg_Normal,0)).xyz; 
	
	// UV of the vertex. No special space for this one.
	//UV = sg_UV0;
}

