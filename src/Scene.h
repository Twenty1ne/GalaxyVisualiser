#pragma once

#include "Renderer.h"

#include <cstddef>
#include <vector>
#include <glm/mat4x4.hpp>

class Scene{
public:
    std::size_t addGeometry(std::size_t geometryId);

    void render(Renderer& renderer, float aspectRatio, const glm::mat4& view) const;

private:
    std::vector<std::size_t> m_geometryIds;
};
