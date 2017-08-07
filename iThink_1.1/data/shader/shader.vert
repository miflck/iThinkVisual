

void main() {

 /*   float windowWidth = 1024.0;
    float windowHeight = 768.0;
    
    float r = gl_FragCoord.x / windowWidth;
    float g = gl_FragCoord.y / windowHeight;
    float b = 1.0;
    float a = 1.0;
    outputColor = vec4(r, g, b, a);
    */
    
    gl_Position   = gl_ModelViewProjectionMatrix * gl_Vertex;
	float size    = gl_Normal.x;
    gl_PointSize  = size;
    gl_FrontColor = gl_Color;

}
