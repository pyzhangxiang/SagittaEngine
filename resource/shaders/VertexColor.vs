
attribute vec3 sg_Vertex;
attribute vec4 sg_Color;

uniform mat4 sg_ViewMatrix;
uniform mat4 sg_ProjectionMatrix;
uniform mat4 sg_ModelMatrix;
uniform mat4 sg_MVMatrix;
uniform mat4 sg_MVPMatrix;

varying vec4 vertexColor;

void main()
{	
	gl_Position = sg_ProjectionMatrix * sg_ViewMatrix * sg_ModelMatrix * (sg_Vertex, 1); //sg_MVPMatrix * vec4(sg_Vertex, 1);
	vertexColor = sg_Color;
}
