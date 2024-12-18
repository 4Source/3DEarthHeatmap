#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;
 
uniform vec3 center;
uniform float minDistance;
uniform float maxDistance;
// Imports the camera matrix from the main function
uniform mat4 camMatrix;
// Imports the model matrix from the main function
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the distance to the center from the current vertex
out float normalizedDistance;

void main()
{
    // calculates current position
	vec3 crntPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));

    // Calculate distance from the vertex to the center
    float distanceToCenter = length(aPos - center);
    // Normalize the distance to the range [0, 1]
    normalizedDistance = (distanceToCenter - minDistance) / (maxDistance - minDistance);

// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(crntPos, 1.0);
}