
// Input vertex data, different for all executions of this shader.
attribute vec3 sg_Vertex;	// in model space
attribute vec2 sg_UV0;
attribute vec3 sg_Normal;

// Output data ; will be interpolated for each fragment.
varying vec2 UV0;
varying vec3 Position_worldspace;
varying vec3 vertexLightColor;
varying vec4 depthCoord;

// Values that stay constant for the whole mesh.
uniform mat4 sg_MVPMatrix;
uniform mat4 sg_MVMatrix;
uniform mat4 sg_ViewMatrix;
uniform mat4 sg_ModelMatrix;
uniform mat4 depthBiasMVP;


// for per-vertex lighting
// material properties
uniform vec4 sg_Material_Ambient;
uniform vec4 sg_Material_Diffuse;
uniform vec4 sg_Material_Specular;
uniform vec4 sg_Material_Emission;
uniform float sg_Material_Shininess;
uniform float sg_Material_SpecularAmount;
uniform float sg_Material_ReflectFraction;


uniform vec3 sg_Light0_Position;	// in world space
uniform vec4 sg_Light0_Ambient;
uniform vec4 sg_Light0_Diffuse;
uniform vec4 sg_Light0_Specular;


void main(){

	// Output position of the vertex, in clip space : sg_MVPMatrix * position
	gl_Position =  sg_MVPMatrix * vec4(sg_Vertex, 1.0);
	depthCoord = depthBiasMVP * vec4(sg_Vertex, 1.0);
	Position_worldspace = (sg_ModelMatrix * vec4(sg_Vertex, 1.0)).xyz;

	// Normal of the the vertex, in camera space
	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	vec3 normal_viewspace = normalize(( sg_MVMatrix * vec4(sg_Normal, 0.0)).xyz);

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 eyedir_viewspace = ( sg_MVMatrix * vec4(sg_Vertex, 1.0)).xyz;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 lightpos_viewspace = ( sg_ViewMatrix * vec4(sg_Light0_Position, 1.0) ).xyz;
	vec3 lightdir_viewspace = normalize(lightpos_viewspace - eyedir_viewspace);

	vec3 r = reflect(-lightdir_viewspace, normal_viewspace);

	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( normal_viewspace, lightdir_viewspace ), 0.0, 1.0 );

	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot(-eyedir_viewspace, r ), 0.0, 1.0 );

	

	// Ambiant : simulates indirect lighting
	vec4 ambient = sg_Material_Ambient * sg_Light0_Ambient;

	// Diffuse : "color" of the object
	vec4 diffuse = sg_Material_Diffuse * sg_Light0_Diffuse * cosTheta;
			
	// Specular : reflective highlight, like a mirror
	vec4 specular = cosTheta > 0.0 ? sg_Material_Specular * sg_Light0_Specular
			 * pow(cosAlpha, sg_Material_Shininess) : vec4(0.0, 0.0, 0.0, 1.0);

	
	vertexLightColor = (ambient + diffuse + specular).xyz;
	
	
	// UV of the vertex. No special space for this one.
	UV0 = sg_UV0;
	
}

