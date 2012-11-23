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

vec4 ShadowCoordPostW;

float chebyshevUpperBound( float distance)
{
	vec2 moments = texture2D(depthMap, ShadowCoordPostW.xy).rg;
	
	// Surface is fully lit. as the current fragment is before the light occluder
	if (distance <= moments.x)
		return 1.0 ;
	else
		return 0.5;
		
	// The fragment is either in shadow or penumbra. We now use chebyshev's upperBound to check
	// How likely this pixel is to be lit (p_max)
	float variance = moments.y - (moments.x*moments.x);
	variance = max(variance,0.00002);

	float d = distance - moments.x;
	float p_max = variance / (variance + d*d);

	return p_max;
}

void main(){

	// Light emission properties
	// You probably want to put them as uniforms
	float LightPower = 10.0 * sg_Light0_Intensity;

	vec4 MaterialDiffuseColor =  sg_Material_Diffuse;// * texture2D( sg_Sampler0, UV0);

	// shadow
	ShadowCoordPostW = depthCoord / depthCoord.w;
	ShadowCoordPostW.x = ShadowCoordPostW.x * 0.5 + 0.5;
	ShadowCoordPostW.y = ShadowCoordPostW.y * 0.5 + 0.5;
	ShadowCoordPostW.z = ShadowCoordPostW.z * 0.5 + 0.5;

	//ShadowCoordPostW.z = pow(ShadowCoordPostW.z, 50);
	
 	float shadow = chebyshevUpperBound(ShadowCoordPostW.z);
	
	vec4 map = vec4(vec3(texture2D(depthMap, ShadowCoordPostW.xy).r), 1.0);


	
	gl_FragColor = shadow * MaterialDiffuseColor;
	
	

}