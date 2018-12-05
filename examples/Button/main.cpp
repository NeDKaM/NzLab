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

class Button
    : public ex::interface<ex::graphics::traits::image, ex::events::default_inputs>
{
    using base = ex::interface<ex::graphics::traits::image, ex::events::default_inputs>;

    ex::interface<ex::graphics::traits::text, ex::events::default_inputs> text_;

    public:
        Button(Ndk::World & world)
            : ex::interface<ex::graphics::traits::image, ex::events::default_inputs> { world }
            , text_{ world } {
            text_.SetParent(this);

            auto defaultstate{ [](Nz::SpriteRef const & spr) {
                spr->SetColor({ 96, 96, 96 });
            } };

            style(defaultstate);

            events().enter.Connect([&]() {
                style([](Nz::SpriteRef const & spr) {
                    spr->SetColor({ 127, 127, 127 });
                });
            });

            events().exit.Connect([&]() {
                style(defaultstate);
            });
        }

        void SetText(std::string const & str) {
            text_.data(str);
            size(text_.size());
        }

        void size(Nz::Vector2f const & value) override {
            base::size(value);
            text_.SetPosition(base::size() / 2.f - text_.size() / 2.f);
        }
};

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
    using Canvas = ex::container<ex::base_interface<ex::events::default_inputs>, InputsForwarder>;

    Canvas canvas{ &canvas, window.GetEventHandler().CreateHandle() };
        canvas.size(Nz::Vector2f{ window.GetSize() });

    auto btn{ canvas.insert<Button>(world) };
        btn->SetText("Exit");
        btn->anchor(canvas, ex::anchor::ease({ .4f, .6f }, { .4f, .6f }));
        btn->events().button_released.Connect([&](Nz::Mouse::Button, int, int) {
            application.Quit();
        });

    while (application.Run()) {
        window.Display();
    }

    return 0;
}
