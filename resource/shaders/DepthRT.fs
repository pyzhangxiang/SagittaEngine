#version 120
varying vec4 Position_screenspace;

void main(){
	//vec4 depth = vec4(vec3(gl_FragCoord.z), 1.0);
	//depth = vec4(vec3(pow(depth.z, 2)), 1.0);
	vec4 depth = vec4(vec3(pow(gl_FragCoord.z, 100)), 1.0);
	gl_FragColor = vec4(vec3(gl_FragCoord.z), 1.0); //depth; //Position_screenspace; //gl_FragCoord.z / gl_FragCoord.w;
}