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

    using InputsForwarder = ex::events::default_inputs_dispatch<ex::base_interface<ex::events::default_inputs>>;
    using ScreenContainer = ex::container<ex::base_interface<ex::events::default_inputs>, InputsForwarder>;

    using SlicedSprite = ex::interface<ex::graphics::traits::sliced_image, ex::events::default_inputs>;
    using Container = ex::container<ex::base_interface<ex::events::default_inputs>, ex::events::default_inputs>;

    using Dispatcher = ex::events::default_dispatcher<ex::events::default_inputs, ex::base_interface<ex::events::default_inputs>>;

    ScreenContainer screen_cont{ &screen_cont, window.GetEventHandler().CreateHandle() };
        screen_cont.size(Nz::Vector2f{ window.GetSize() });
     
    auto cont{ screen_cont.insert<Container>() };
    Dispatcher dispatcher{ *cont };
        cont->anchor(screen_cont, ex::anchor::ease(ex::anchor::stretch(), ex::anchor::stretch()));
        cont->events().mouse_moved.Connect<Dispatcher>(dispatcher, &Dispatcher::mouse_moved);
        cont->events().exit.Connect<Dispatcher>(dispatcher, &Dispatcher::exit);

    auto slice_gwen{ [](ex::graphics::sliced_sprite::ref const & sprite) {
        sprite->slice({ 0.0157f, 0.197f, 0.961f, 0.772f });
    } };

    auto slice_btn{ [](ex::graphics::sliced_sprite::ref const & sprite) {
        sprite->texture_coords({ 0.f, 0.8951f, 0.5f, 0.10801f });
        sprite->slice({ 0.04878f, 0.12903f, 0.90244f, 0.74193f });
    } };

    auto div1{ cont->insert<SlicedSprite>(world) };
        div1->data("output-107.png");
        div1->style(slice_gwen);
        div1->anchor(*cont.get(), ex::anchor{ { 0.1f, 0.1f }, { 0.9f, 0.5f } });
    auto div2{ cont->insert<SlicedSprite>(world) };
        div2->data("output-107.png");
        div2->anchor(*cont.get(), ex::anchor::ease({ 0.1f, 0.9f }, { 0.5f, 0.9f }));

    while (application.Run()) {
        window.Display();
    }

    return 0;
}
