attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec4 vNormal;

varying vec4 color;
uniform mat4 MVP;
uniform vec3 heat;

void main()
{
	gl_Position = MVP * vPosition;
	color = vec4(heat.x, heat.y, heat.z, 1.0);
}