#version 330

in vec2 fragTexCoord;
uniform sampler2D texture0;
uniform vec2 resolution;
uniform float reflectionHeight;

out vec4 finalColor;

void main() {
    vec2 uv = fragTexCoord;
    vec4 originalColor = texture(texture0, uv);

    if (uv.y > reflectionHeight) {
        finalColor = originalColor; // No reflection above the floor
    } else {
        vec2 reflectionUV = vec2(uv.x, 2.0 * reflectionHeight - uv.y);
        vec4 reflectionColor = texture(texture0, reflectionUV);
        float reflectionStrength = 0.5; // Adjust the reflection strength here
        finalColor = mix(originalColor, reflectionColor, reflectionStrength);
    }
}
