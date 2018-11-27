#include <NDK/Application.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <NDK/Systems/RenderSystem.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Components/DebugComponent.hpp>

#include <NzLab/container.hpp>
#include <NzLab/interface.hpp>
#include <NzLab/Graphics/traits.hpp>
#include <NzLab/Events/default_inputs.hpp>

int main() {
    Ndk::Application application;

    auto & window{ application.AddWindow<Nz::RenderWindow>(Nz::VideoMode{ 800, 600, 32 }, "NzLab") };

    auto & world{ application.AddWorld() };
        world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());

    auto view{ world.CreateEntity() };
        view->AddComponent<Ndk::NodeComponent>();
        auto & camera{ view->AddComponent<Ndk::CameraComponent>() };
            camera.SetTarget(&window);
            camera.SetProjectionType(Nz::ProjectionType_Orthogonal);

    auto slice{ [](ex::graphics::sliced_sprite::ref const & sprite) {
        sprite->slice({ 0.0157f, 0.197f, 0.961f, 0.772f });
    } };

    using SlicedSprite = ex::interface<ex::graphics::traits::sliced_image, ex::events::default_inputs>;
    using Container = ex::container<ex::base_interface<ex::events::default_inputs>, ex::events::default_inputs>;

    Container container;
        auto div1{ container->insert<SlicedSprite>(world) };
            div1->data("output-107.png");
            div1->anchor(*cont.get(), ex::anchor{ { 0.1f, 0.1f }, { 0.9f, 0.5f } });
            div1->style(slice);      
        auto div2{ container->insert<SlicedSprite>(world) };
            div2->data("output-107.png");
            div2->anchor(*cont.get(), ex::anchor::ease({ 0.1f, 0.9f }, { 0.5f, 0.9f }));

    while (application.Run()) {
        window.Display();
    }

    return 0;
}
