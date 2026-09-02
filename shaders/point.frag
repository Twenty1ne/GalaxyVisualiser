#version 450 core

in vec3 vertexColor;

uniform bool glow;

out vec4 FragColor;

void main()
{
    vec2 pointPosition = gl_PointCoord * 2.0 - 1.0;

    float distanceFromCentre = length(pointPosition);

    if (distanceFromCentre > 1.0)
    {
        discard;
    }

    if (glow)
    {
        float coreRadius = 0.2;

        if (distanceFromCentre < coreRadius)
        {
            float z =
                sqrt(
                    1.0 -
                    distanceFromCentre * distanceFromCentre
                );

            vec3 normal =
                normalize(
                    vec3(
                        pointPosition.x,
                        pointPosition.y,
                        z
                    )
                );

            vec3 lightDirection =
                normalize(
                    vec3(-0.4, 0.5, 1.0)
                );

            float brightness =
                max(
                    dot(normal, lightDirection),
                    0.0
                );

            float ambient = 0.25;

            vec3 finalColor =
                vertexColor *
                (ambient + brightness * 0.75);

            FragColor =
                vec4(finalColor, 1.0);
        }
        else
        {
            float glowStrength =
                1.0 -
                smoothstep(
                    coreRadius,
                    1.0,
                    distanceFromCentre
                );

            glowStrength *= 0.5;

            FragColor =
                vec4(
                    vertexColor,
                    glowStrength
                );
        }

        return;
    }

    float z =
        sqrt(
            1.0 -
            distanceFromCentre * distanceFromCentre
        );

    vec3 normal =
        normalize(
            vec3(
                pointPosition.x,
                pointPosition.y,
                z
            )
        );

    vec3 lightDirection =
        normalize(
            vec3(-0.4, 0.5, 1.0)
        );

    float brightness =
        max(
            dot(normal, lightDirection),
            0.0
        );

    float ambient = 0.25;

    vec3 finalColor =
        vertexColor *
        (ambient + brightness * 0.75);

    FragColor =
        vec4(finalColor, 1.0);
}
