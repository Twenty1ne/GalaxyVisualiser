#pragma once

#include "Renderer.h"

#include <cstddef>
#include <vector>
#include <glm/mat4x4.hpp>

class Scene{
public:
    std::size_t addGeometry(std::size_t geometryId, bool glow = false);

    void render(Renderer& renderer, float aspectRatio, const glm::mat4& view) const;

private:
    struct SceneGeometry{
        std::size_t geometryId;
        bool glow;
    };

    std::vector<SceneGeometry> m_geometryIds;
};
