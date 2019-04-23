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
layout(location = 4) in vec3 light[4]; 
layout(location = 8) in vec3 lightColour[4]; 
layout(location = 12) in vec3 lightIntensity[4]; 
layout(location = 16) in vec3 ambient; 


layout(location = 0) out vec4 outColor;

void main() {
    //outColor = vec4(fragColor*texture(texSampler[PushConstant.textureIndex], fragTexCoord).rgb,0.9);

	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);

	for(int i = 0;i<4;i++){
		vec3 N = normalize(normal);
		vec3 L = normalize(light[i]);
		vec3 V = normalize(view);
		vec3 R = reflect(-L,N);	
		totalDiffuse = totalDiffuse + (max(dot(N,L),0.0) * fragColor * lightColour[i]);
		totalSpecular =totalSpecular + (pow(max(dot(R,V),0.0),16.0) * lightIntensity[i] * lightColour[i]);	
	}	
	//totalDiffuse=max(totalDiffuse,0.02);

	vec3 ambientColour = fragColor * ambient;
	vec3 texture = fragColor*texture(texSampler[PushConstant.textureIndex], fragTexCoord).rgb;
	outColor = vec4(ambientColour+totalDiffuse+totalSpecular+texture,1.0);//+texture
}