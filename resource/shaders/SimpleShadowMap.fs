#version 120

// Interpolated values from the vertex shaders
varying vec2 UV0;
varying vec3 Position_worldspace;
varying vec3 Normal_cameraspace;
varying vec3 EyeDirection_cameraspace;
varying vec3 LightDirection_cameraspace;
varying vec3 vertexColor;
varying vec4 depthCoord;

// Values that stay constant for the whole mesh.
uniform vec4 sg_Material_Ambient;
uniform vec4 sg_Material_Diffuse;
uniform vec4 sg_Material_Specular;
uniform vec4 sg_Material_Emission;
uniform float sg_Material_Shininess;
uniform float sg_Material_SpecularAmount;
uniform float sg_Material_ReflectFraction;

uniform sampler2D sg_Sampler0;
uniform sampler2D depthMap;

uniform vec3 sg_Light0_Position;	// in world space
uniform vec4 sg_Light0_Diffuse;
uniform float sg_Light0_Intensity;

void main(){

	// Light emission properties
	// You probably want to put them as uniforms
	float LightPower = 10.0 * sg_Light0_Intensity;

	vec4 MaterialDiffuseColor =  sg_Material_Diffuse;// * texture2D( sg_Sampler0, UV0);

	// shadow
	//shadow2D( shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z)/ShadowCoord.w) ).r;
	//vec4 depthColor = shadow2D(depthMap, vec3(depthCoord.x, depthCoord.y, depthCoord.z/depthCoord.w));
	vec4 depthColor = texture2DProj(depthMap, vec3(depthCoord.x, depthCoord.y, depthCoord.z/depthCoord.w));
	//vec4 depthColor2 = texture2D(depthMap,UV0);
	vec4 lightDepth2 = vec4(vec3(pow(depthColor.z, 100)), 1.0);
	vec4 lightDepth2_2 = vec4(vec3(depthColor.z), 1.0);
	vec4 lightDepth = vec4(vec3((depthCoord.z / depthCoord.w)), 1.0);
	
 	float shadow = 1.0;
 	//if (depthCoord.w > 0.0)
 		shadow = depthColor.r < depthCoord.z / depthCoord.w ? 0.0 : 1.0 ;

	// Distance to the light
	float distance = length( sg_Light0_Position - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	gl_FragColor = //lightDepth2_2;//lightDepth2;//vec4(depthCoord.x, depthCoord.y, 0.0, 1.0);
	
		// Ambiant : simulates indirect lighting
		shadow * ( sg_Material_Ambient +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * sg_Light0_Diffuse * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		sg_Material_Specular * sg_Light0_Diffuse * LightPower * pow(cosAlpha,5) / (distance*distance) );
	

}