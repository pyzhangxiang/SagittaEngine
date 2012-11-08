
attribute vec3 sg_vertex;
attribute vec4 sg_color;

varying vec4 vertexColor;

void main()
{	
	gl_Position = gl_ModelViewProjectionMatrix * vec4(sg_vertex, 1);
	vertexColor = sg_color;
}
