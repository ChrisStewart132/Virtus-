#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
	mat4 view;
	mat4 proj;
	vec3 lightPosition[4];
	vec3 lightColour[4];
	float lightIntensity[4];
	vec3 ambient;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;



layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragView;
layout(location = 4) out vec3 fragLight[4];
layout(location = 8) out vec3 fragLightColour[4];
layout(location = 12) out float fragLightIntensity[4];
layout(location = 16) out vec3 fragAmbient;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    vec4 worldPos=ubo.model*vec4(inPosition,1.0);
   
    fragColor = inColor;
    fragTexCoord = inTexCoord;
	fragNormal = mat3(ubo.view) * mat3(ubo.model) * inNormal;
	fragView = -(ubo.view * worldPos).xyz;
	for(int i = 0;i<4;i++){
		fragLight[i] = mat3(ubo.view) * (ubo.lightPosition[i] - vec3(worldPos));
		fragLightColour[i]=ubo.lightColour[i];
		fragLightIntensity[i]=ubo.lightIntensity[i];
	}
	fragAmbient=ubo.ambient;
}