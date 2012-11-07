
attribute vec3 sg_vertex;
attribute vec4 sg_color;

uniform float scale;

varying vec4 vertexColor;

void main()
{	
	vec3 aa = sg_vertex;
	aa.x = aa.x * scale;
    aa.y = aa.y * scale;
	aa.z = aa.z * scale;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(aa, 1);
	
	vertexColor = sg_color;

}
