varying vec4 fragmentColor;
void main(){	
	gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
	
	fragmentColor = gl_Color;

}
