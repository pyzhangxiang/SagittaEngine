#version 120
varying vec4 Position_screenspace;

void main(){

	vec4 depthCoord = Position_screenspace / Position_screenspace.w;
	depthCoord.x  = depthCoord.x * 0.5 + 0.5;
	depthCoord.y  = depthCoord.y * 0.5 + 0.5;
	depthCoord.z  = depthCoord.z * 0.5 + 0.5;
	float depth = depthCoord.z;			//Don't forget to move away from unit cube ([-1,1]) to [0,1] coordinate system

	float moment1 = depth;
	float moment2 = depth * depth;

	// Adjusting moments (this is sort of bias per pixel) using derivative
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	moment2 += 0.25*(dx*dx+dy*dy) ;
	
    gl_FragColor = vec4( moment1,moment1, moment1, 1.0 );
	//gl_FragDepth = depth;
}