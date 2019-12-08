#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler[6];

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
layout(location = 17) in vec3 ambient;//doesn't seem to be working, set to 0.2f manually


layout(location = 0) out vec4 outColor;

void main() {
    //outColor = vec4(fragColor*texture(texSampler[PushConstant.textureIndex], fragTexCoord).rgb,0.9);

	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);
	int i = 0;

	//for(int i = 0;i<4;i++){		
		vec3 N = normalize(normal);
		vec3 L = normalize(light[i]);			
		totalDiffuse = totalDiffuse + (max(dot(N,L),0.0) * fragColor * lightColour[i]);

		float specularStrength = 0.2;//diameter/size of reflected light
		vec3 V = normalize(view);//viewDir
		vec3 R = reflect(-L,N);	//reflectDir
		float spec=pow(max(dot(R,V),0.0),32);
		totalSpecular =totalSpecular + (specularStrength * spec * lightColour[i]);	
	//}	
	totalDiffuse=max(totalDiffuse,0.02);
	//totalSpecular = max(totalSpecular,0.05);

	float ambientLightIntensity = 0.1f;
	vec3 ambientLight = vec3(ambientLightIntensity,ambientLightIntensity,ambientLightIntensity);	
	vec3 sunLightIntensity = vec3(0.1,0.1,0.1);
	vec3 sunLightDirection = normalize(vec3(3.0,4.0,-2.0));
	vec3 sunLight = sunLightIntensity * max(dot(normal,sunLightDirection),0.0);

	vec3 texture = fragColor*texture(texSampler[PushConstant.textureIndex], fragTexCoord).rgb;

	outColor = vec4(texture*(ambientLight+sunLight+totalDiffuse+totalSpecular),1.0);//+texture
}