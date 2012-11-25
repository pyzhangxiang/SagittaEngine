#version 120

// Interpolated values from the vertex shaders
varying vec2 UV0;
varying vec3 Position_worldspace;
varying vec3 vertexLightColor;
varying vec4 depthCoord;

uniform int sg_Sampler0_Enabled;	// bool, 0 == false, 1 == true
uniform sampler2D sg_Sampler0;
uniform sampler2D depthMap;
uniform vec3 sg_Light0_Position;	// in world space
uniform float sg_Light0_Intensity;
uniform vec4 sg_Enviroment_Ambient;

vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

float unpack_float(const in vec4 rgba_depth)
{
    const vec4 bit_shift = vec4(1.0/(256.0*256.0*256.0), 1.0/(256.0*256.0), 1.0/256.0, 1.0);
    float depth = dot(rgba_depth, bit_shift);
    return depth;
}

void main(){

	// Distance to the light
	float distance = length( sg_Light0_Position - Position_worldspace );

	// shadow
	vec4 ShadowCoordPostW = depthCoord / depthCoord.w;
	ShadowCoordPostW.x = ShadowCoordPostW.x * 0.5 + 0.5;
	ShadowCoordPostW.y = ShadowCoordPostW.y * 0.5 + 0.5;
	ShadowCoordPostW.z = ShadowCoordPostW.z * 0.5 + 0.5;
	
	float depth = 0.0;// = unpack_float(texture2D(depthMap, ShadowCoordPostW.xy));

	float min_depth = 0.1;
	float max_depth = 1000.0;
	float bias = min_depth / max_depth;
	float real_distance = ((distance - min_depth) / max_depth) - bias;

	float shadow = 1.0;
	for(int i=0; i<1; ++i)
	{
		depth = unpack_float(texture2D(depthMap, ShadowCoordPostW.xy/* + poissonDisk[i]/700.0*/));
		shadow = depth < real_distance ? shadow - 0.5 : shadow;
	}
	//float shadow = depth < real_distance ? 0.5 : 1.0;


	// Light emission properties
	// You probably want to put them as uniforms
	float lightPower = 10.0 * sg_Light0_Intensity;

	vec4 textureColor = sg_Sampler0_Enabled != 0 ? texture2D( sg_Sampler0, UV0) : vec4(1.0, 1.0, 1.0, 1.0);

	gl_FragColor = textureColor * 
		( sg_Enviroment_Ambient + 
			shadow * vec4(vertexLightColor, 1.0) * lightPower / (distance * distance) 
		);

}