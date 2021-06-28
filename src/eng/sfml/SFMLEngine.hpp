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
    void createText(GameEngine& gameContext, int id) override;
    void createCamera(GameEngine& gameContext, int id) override;
    void eraseEntity(int id) override;
    void eraseAllEntities() override;
    size_t countRenderNodes() const override;
    bool existsNode(int id) const override;
    bool existsText(int id) const override;
    bool existsHUDNode(int id) const override;
    bool existsHUDText(int id) const override;
    bool existsImage(std::string path) const override;
    bool existsCamera(int id) const override;
    void addImage(std::string path) override;
    bool existsTexture(std::string path) const override;
    void addTexture(std::string path) override;
    void setColorToEntity(const int id, const Color color) override;
    void setBackgroundLayers(std::vector<BackgroundLayer>& layers, float backgroundSize) override;
    void updateBackgroundLayers(std::vector<BackgroundLayer>& layers, float backgroundSize) override;

    // For updating the FPS 
    void updateFPSTextNode(uint8_t FPS) override;

private:
    bool existsWorldNode(int id) const;

    void drawScene(GameEngine& gameContext) const;
    void renderColliders(GameEngine& gameContext) const;
    void renderLaser(GameEngine& gameContext) const;
    void renderAllSensors(GameEngine& gameContext) const;
    void drawBoundingTree(BoundingBoxNode boundingNode, SituationComponent& sit) const;
    void drawHudElements(GameEngine& gameContext) const;

    void updateNode(GameEngine& gameContext, sf::Sprite& node, int id);
    void updateTexture(GameEngine& gameContext, sf::Sprite& node, int id);
    void updateText(GameEngine& gameContext, sf::Text& textNode, int id);
    void updateCamera(GameEngine& gameContext, int id);

    void setKeyToControl(GameEngine& gameContext, sf::Event& event) const;
    void setJoystickButtonToControl(GameEngine& gameContext, sf::Event& event) const;


    std::unique_ptr<sf::RenderWindow> device;
    std::unordered_map<int, sf::Sprite> nodeMap;
    std::unordered_map<int, sf::Sprite> nodeWorldMap; // sprites of the world (WALL)
    std::unordered_map<int, sf::Text> textMap;
    std::unordered_map<int, sf::Sprite> HUDNodeMap; // nodes that render on hud 
    std::unordered_map<int, sf::Text> HUDTextMap;
    std::unordered_map<std::string, sf::Image> imageMap;
    std::unordered_map<std::string, sf::Texture> textureMap;
    std::vector<sf::Sprite> backgroundLayers;

    std::unordered_map<int, sf::View> cameraMap;

    sf::Font font;

    // For the FPS display
    sf::Text FPSTextNode {};
    bool renderFPS { false };

    bool fullscreen = true;
    bool renderCollidables = true;
    bool renderSensors = true;
};