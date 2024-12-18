#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

in float normalizedDistance; // From vertex shader
uniform sampler1D gradiant0;

void main() {
    // Use normalizedDistance to determine color or other properties
    FragColor = texture(gradiant0, normalizedDistance); // Grayscale based on normalized distance
}