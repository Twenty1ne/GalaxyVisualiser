#include "Scene.h"

std::size_t Scene::addGeometry(std::size_t geometryId){
    m_geometryIds.push_back(geometryId);
    return geometryId;
}

void Scene::render(Renderer& renderer, float aspectRatio, const glm::mat4& view) const{
    for(std::size_t geometryId : m_geometryIds){
        renderer.render(geometryId, aspectRatio, view);
    }
}
