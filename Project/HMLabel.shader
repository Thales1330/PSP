#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_mvpMatrix;

void main()
{
	gl_Position = u_mvpMatrix * position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_offset;

vec3 hsl2rgb(in vec3 c)
{
	vec3 rgb = clamp(abs(mod(c.x * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);

	return c.z + c.y * (rgb - 0.5) * (1.0 - abs(2.0 * c.z - 1.0));
}

void main()
{
	vec3 colorRGB = hsl2rgb(vec3(gl_FragCoord.z * 0.8, 1.0, 0.5));
	color = vec4(colorRGB, 1.0);
	if(gl_FragCoord.x - u_offset.x <= 2.0) color = vec4(0.0, 0.0, 0.0, 1.0);
	if(gl_FragCoord.x - u_offset.x >= 28.0) color = vec4(0.0, 0.0, 0.0, 1.0);
	if(gl_FragCoord.y + u_offset.y >= u_offset.w - 2.0) color = vec4(0.0, 0.0, 0.0, 1.0);
	if(gl_FragCoord.y + u_offset.y <= u_offset.w - 298.0) color = vec4(0.0, 0.0, 0.0, 1.0);
};