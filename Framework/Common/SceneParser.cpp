#include "SceneParser.hpp"

using namespace Me;
using namespace std;

const std::shared_ptr<SceneObjectCamera> Scene::GetCamera(std::string key) const {
    auto i = Cameras.find(key);
    if (i == Cameras.end())
        return nullptr;
    else
        return i->second;
}

const std::shared_ptr<SceneObjectCamera> Scene::GetFirstCamera() const {
    return Cameras.empty() ? nullptr : Cameras.cbegin()->second;
}

const std::shared_ptr<SceneObjectCamera> Scene::GetNextCamera() const {
    static thread_local auto _it = Cameras.cbegin();
    if (_it == Cameras.cend()) return nullptr;
    return (++_it == Cameras.cend()) ? nullptr : _it->second;
}

const std::shared_ptr<SceneObjectLight> Scene::GetLight(std::string key) const {
    auto i = Lights.find(key);
    if (i == Lights.end())
        return nullptr;
    else
        return i->second;
}

const std::shared_ptr<SceneObjectLight> Scene::GetFirstLight() const {
    return Lights.empty() ? nullptr : Lights.cbegin()->second;
}

const std::shared_ptr<SceneObjectLight> Scene::GetNextLight() const {
    static thread_local auto _it = Lights.cbegin();
    if (_it == Lights.cend()) return nullptr;
    return (++_it == Lights.cend()) ? nullptr : _it->second;
}

const std::shared_ptr<SceneObjectMaterial> Scene::GetMaterial(std::string key) const {
    auto i = Materials.find(key);
    if (i == Materials.end())
        return nullptr;
    else
        return i->second;
}

const std::shared_ptr<SceneObjectMaterial> Scene::GetFirstMaterial() const {
    return Materials.empty() ? nullptr : Materials.cbegin()->second;
}

const std::shared_ptr<SceneObjectMaterial> Scene::GetNextMaterial() const {
    static thread_local auto _it = Materials.cbegin();
    if (_it == Materials.cend()) return nullptr;
    return (++_it == Materials.cend()) ? nullptr : _it->second;
}

const std::shared_ptr<SceneObjectGeometry> Scene::GetGeometry(std::string key) const {
    auto i = Geometries.find(key);
    if (i == Geometries.end())
        return nullptr;
    else
        return i->second;
}

const std::shared_ptr<SceneObjectGeometry> Scene::GetFirstGeometry() const {
    return Geometries.empty() ? nullptr : Geometries.cbegin()->second;
}

const std::shared_ptr<SceneObjectGeometry> Scene::GetNextGeometry() const {
    // todo why
    static thread_local auto _it = Geometries.cbegin();
    if (_it == Geometries.cend()) return nullptr;
    return (++_it == Geometries.cend()) ? nullptr : _it->second;
}
