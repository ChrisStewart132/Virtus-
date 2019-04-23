#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler[10];

layout (push_constant) uniform textureInfo {
	int textureIndex;
} PushConstant;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 view;
layout(location = 4) in vec3 light; 


layout(location = 0) out vec4 outColor;

void main() {
    //outColor = vec4(fragColor*texture(texSampler[PushConstant.textureIndex], fragTexCoord).rgb,0.9);
	vec3 N = normalize(normal);
	vec3 L = normalize(light);
	vec3 V = normalize(view);
	vec3 R = reflect(-L,N);

	vec3 ambient = fragColor * 0.05;
	vec3 diffuse = max(dot(N,L),0.0) * fragColor;
	vec3 specular = pow(max(dot(R,V),0.0),16.0) * vec3(0.05); //*vec(intensity)
	vec3 texture = fragColor*texture(texSampler[PushConstant.textureIndex], fragTexCoord).rgb;
	

	outColor = vec4(ambient+diffuse+specular+texture,1.0);//+texture
}