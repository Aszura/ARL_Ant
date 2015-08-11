#version 330  

uniform sampler2D colorMap;

in vec2 uv;
in vec4 color;

out vec4 FragColor;

void main(void)
{
	vec4 texel = texture2D(colorMap,uv);
	vec4 c;
	c.r = pow(color.r,1/2.2);
	c.g = pow(color.g,1/2.2);
	c.b = pow(color.b,1/2.2);
	c.a = pow(color.a,1/2.2);
	if(texel.a < 1.0){
		discard;
	}
	FragColor = texel * c;
}