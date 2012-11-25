#version 120
varying vec4 Position_screenspace;
varying vec4 Position_viewspace;

vec4 pack_float(const in float depth)
{
    const vec4 bit_shift = vec4(256.0*256.0*256.0, 256.0*256.0, 256.0, 1.0);
    const vec4 bit_mask  = vec4(0.0, 1.0/256.0, 1.0/256.0, 1.0/256.0);
    vec4 res = fract(depth * bit_shift);
    res -= res.xxyz * bit_mask;
    return res;
}

void main(){

	vec4 depthCoord = Position_screenspace / Position_screenspace.w;
	depthCoord.x  = depthCoord.x * 0.5 + 0.5;
	depthCoord.y  = depthCoord.y * 0.5 + 0.5;
	depthCoord.z  = depthCoord.z * 0.5 + 0.5;
	float depth = depthCoord.z;			//Don't forget to move away from unit cube ([-1,1]) to [0,1] coordinate system
	
	float min_depth = 0.1;
	float max_depth = 1000.0;
	depth = (length(Position_viewspace) - min_depth) / max_depth;
	gl_FragColor = pack_float(depth);
	//gl_FragColor = vec4( depth, depth, depth, 1.0 );
	//gl_FragDepth = depth;
}