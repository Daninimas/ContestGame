#pragma once

#include <fac/WindowFacade.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>


class SFMLEngine : public WindowFacade {
public:
    SFMLEngine(int width, int height, bool fullscreen);
    ~SFMLEngine();

    const bool checkInput(const uint16_t key) const override;
    std::array<float, 2> getMousePosition() const override;
    std::array<float, 2> getWindowDimensions() const override;
    void render(GameEngine& gameContext) const override;
    void updateEntities(GameEngine& gameContext, std::vector<int> entitiesId) override;
    void updateTextures(GameEngine& gameContext, std::vector<int> entitiesId) override;
    void createEntity(GameEngine& gameContext, int id) override;
    void eraseEntity(int id) override;
    void addColorToEntity(int id, std::array<float, 3> color) override;
    size_t countRenderNodes() const override;
    bool existsNode(int id) const override;
    bool existsImage(std::string path) const override;
    void addImage(std::string path) override;
    bool existsTexture(std::string path) const override;
    void addTexture(std::string path) override;

private:
    void renderColliders(GameEngine& gameContext) const;
    void drawBoundingTree(BoundingBoxNode boundingNode, SituationComponent& sit) const;

    void updateNode(GameEngine& gameContext, int id);

    std::unique_ptr<sf::RenderWindow> device;
    std::unordered_map<int, sf::Sprite> nodeMap;
    std::unordered_map<std::string, sf::Image> imageMap;
    std::unordered_map<std::string, sf::Texture> textureMap;

    bool fullscreen = true;
    bool renderCollidables = true;
};