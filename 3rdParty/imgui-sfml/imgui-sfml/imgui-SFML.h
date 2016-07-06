#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <3rdParty/imgui-sfml/export.hpp>

namespace sf
{
    class Event;
    class RenderTarget;
    class RenderWindow;
    class Sprite;
    class Texture;
    class Window;
}

namespace ImGui
{
namespace SFML
{
    IMGUI_SFML_EXPORT void Init(sf::Window& window, sf::RenderTarget& target);
    IMGUI_SFML_EXPORT void Init(sf::RenderWindow& window); // for convenience
    IMGUI_SFML_EXPORT void ProcessEvent(const sf::Event& event);
    IMGUI_SFML_EXPORT void Update();
    IMGUI_SFML_EXPORT void Shutdown();

    IMGUI_SFML_EXPORT void SetRenderTarget(sf::RenderTarget& target);
    IMGUI_SFML_EXPORT void SetWindow(sf::Window& window);
}

// custom ImGui widgets for SFML stuff
    IMGUI_SFML_EXPORT void Image(const sf::Texture& texture);
    IMGUI_SFML_EXPORT void Image(const sf::Texture& texture, const sf::Vector2f& size);

    IMGUI_SFML_EXPORT void Image(const sf::Sprite& sprite);
    IMGUI_SFML_EXPORT void Image(const sf::Sprite& sprite, const sf::Vector2f& size);

    IMGUI_SFML_EXPORT bool ImageButton(const sf::Texture& texture, const int framePadding = -1,
                     const sf::Color& bgColor = sf::Color::Transparent,
                     const sf::Color& tintColor = sf::Color::White);
    IMGUI_SFML_EXPORT bool ImageButton(const sf::Texture& texture, const sf::Vector2f& size, const int framePadding = -1,
                     const sf::Color& bgColor = sf::Color::Transparent, const sf::Color& tintColor = sf::Color::White);

    IMGUI_SFML_EXPORT bool ImageButton(const sf::Sprite& sprite, const int framePadding = -1,
                     const sf::Color& bgColor = sf::Color::Transparent,
                     const sf::Color& tintColor = sf::Color::White);
    IMGUI_SFML_EXPORT bool ImageButton(const sf::Sprite& sprite, const sf::Vector2f& size, const int framePadding = -1,
                     const sf::Color& bgColor = sf::Color::Transparent,
                     const sf::Color& tintColor = sf::Color::White);
}
