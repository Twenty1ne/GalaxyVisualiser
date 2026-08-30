#include "Scene.h"

std::size_t Scene::addGeometry(std::size_t geometryId, bool glow){
    m_geometryIds.push_back({geometryId, glow});
    return geometryId;
}

void Scene::render(Renderer& renderer, float aspectRatio, const glm::mat4& view) const{
    for(const SceneGeometry& geometry : m_geometryIds){
        renderer.render(geometry.geometryId, aspectRatio, view, geometry.glow);
    }
}
