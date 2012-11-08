
attribute vec3 sg_Vertex;
attribute vec4 sg_Color;

varying vec4 vertexColor;

void main()
{	
	gl_Position = gl_ModelViewProjectionMatrix * vec4(sg_Vertex, 1);
	vertexColor = sg_Color;
}
