#version 450 core

in vec3 vertexColor;

out vec4 FragColor;

void main(){
    vec2 pointPosition = gl_PointCoord * 2.0 - 1.0;

    float distanceFromCentre = length(pointPosition);

    if(distanceFromCentre > 1.0) discard;

    float z = sqrt(1.0 - distanceFromCentre * distanceFromCentre);

    vec3 normal = normalize(vec3(pointPosition.x, pointPosition.y, z));

    vec3 lightDirection = normalize(vec3(-0.4, 0.5, 1.0));

    float brightness = max(dot(normal, lightDirection), 0.0);

    float ambient = 0.25;

    vec3 finalColor = vertexColor * (ambient + brightness * 0.75);

    FragColor = vec4(finalColor, 1.0);
}
