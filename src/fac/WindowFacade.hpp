#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <unordered_map>
#include <com/ComponentsIncluder>
#include <tools/Color.hpp>

class GameEngine;


class WindowFacade {
public:
    virtual ~WindowFacade() = default;

    virtual const bool checkInput(const uint16_t key) const = 0;
    virtual std::array<float, 2> getMousePosition() const = 0;
    virtual std::array<float, 2> getWindowDimensions() const = 0;
    virtual void render(GameEngine &gameContext) const = 0;
    virtual void updateEntities(GameEngine &gameContext, std::vector<int> entitiesId) = 0;
    virtual void updateTextures(GameEngine &gameContext, std::vector<int> entitiesId) = 0;
    virtual void updateCamera(GameEngine& gameContext, int id) = 0;
    virtual void createEntity(GameEngine& gameContext, int id) = 0;
    virtual void createCamera(GameEngine &gameContext, int id) = 0;
    virtual void eraseEntity(int id) = 0;
    virtual void addColorToEntity(int id, Color color) = 0;
    virtual size_t countRenderNodes() const = 0;
    virtual bool existsNode(int id) const = 0;
    virtual bool existsImage(std::string path) const = 0;
    virtual bool existsCamera(int id) const = 0;
    virtual void addImage(std::string path) = 0;
    virtual bool existsTexture(std::string path) const = 0;
    virtual void addTexture(std::string path) = 0;
};
