#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "SceneNode.hpp"
#include "SceneObject.hpp"

namespace Me {
    class Scene {
    public:
        Scene(const char *scene_name) :
        SceneGraph(new BaseSceneNode(scene_name)) {
        }

        ~Scene() = default;

    public:
        std::shared_ptr<BaseSceneNode> SceneGraph;

        std::unordered_multimap<std::string, std::shared_ptr<SceneCameraNode>> CameraNodes;
        std::unordered_multimap<std::string, std::shared_ptr<SceneLightNode>> LightNodes;
        std::unordered_multimap<std::string, std::shared_ptr<SceneGeometryNode>> GeometryNodes;

        std::unordered_map<std::string, std::shared_ptr<SceneObjectCamera>> Cameras;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectLight>> Lights;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectMaterial>> Materials;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectGeometry>> Geometries;

        const std::shared_ptr<SceneObjectCamera> GetCamera(std::string key) const;

        const std::shared_ptr<SceneCameraNode> GetFirstCameraNode() const;

        const std::shared_ptr<SceneCameraNode> GetNextCameraNode() const;

        const std::shared_ptr<SceneObjectLight> GetLight(std::string key) const;

        const std::shared_ptr<SceneLightNode> GetFirstLightNode() const;

        const std::shared_ptr<SceneLightNode> GetNextLightNode() const;

        const std::shared_ptr<SceneObjectMaterial> GetMaterial(std::string key) const;

        const std::shared_ptr<SceneObjectMaterial> GetFirstMaterial() const;

        const std::shared_ptr<SceneObjectMaterial> GetNextMaterial() const;

        const std::shared_ptr<SceneObjectGeometry> GetGeometry(std::string key) const;

        const std::shared_ptr<SceneGeometryNode> GetFirstGeometryNode() const;

        const std::shared_ptr<SceneGeometryNode> GetNextGeometryNode() const;

    };
}

