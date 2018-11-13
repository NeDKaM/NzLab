#ifndef HPP_EX_EVENTS_DEFAULT_INPUTS_INCLUDED
#define HPP_EX_EVENTS_DEFAULT_INPUTS_INCLUDED

    #include <Nazara/Core/Signal.hpp>
    #include <Nazara/Platform/Event.hpp>

    #include <NzLab/container.hpp>

    namespace ex {

        namespace events {
            
            struct default_inputs
            {
                Nz::Signal<int /*x*/, int /*y*/, int /*dx*/, int /*dy*/> mouse_moved;
                Nz::Signal<> enter;
                Nz::Signal<> exit;
            };

            template <typename ElementType>
            class default_inputs_forward
                : public ElementType::events_type
            {
                ex::container<ElementType, default_inputs_forward> * container_;
                ElementType * hovered_;

            public:
                default_inputs_forward(ex::container<ElementType, default_inputs_forward> * cont, Nz::EventHandler & events)
                    : container_{ cont }
                    , hovered_{ nullptr } {
                    slot_mouse_moved.Connect(events.OnMouseMoved, this, &default_inputs_forward::on_mouse_moved);
                }

            protected:
                template <typename... Args>
                using slot_type = typename Nz::Signal<Nz::EventHandler const *, Args...>::ConnectionGuard;

                slot_type<Nz::WindowEvent::MouseMoveEvent const &> slot_mouse_moved;

            private:
                void on_mouse_moved(Nz::EventHandler const *, Nz::WindowEvent::MouseMoveEvent const & ev) {
                    // mouse move handling, as it is done in Nazara
                    ElementType * element{ nullptr };
                    float best_area{ std::numeric_limits<float>::infinity() };
                    container_->for_each([&](ElementType * elm) {
                        auto position{ Nz::Vector3i{ elm->GetPosition() } };
                        auto size{ elm->size() };
                        Nz::Recti elm_area{ position.x, position.y, static_cast<int>(size.x), static_cast<int>(size.y) };
                        if (elm_area.Contains(ev.x, ev.y)) {
                            float area{ size.x * size.y };
                            if (area < best_area) {
                                best_area = area;
                                element = elm;
                            }
                        }
                    });
                    if (element) {
                        if (element != hovered_) {
                            if (hovered_) {
                                hovered_->events().exit();
                            }
                            hovered_ = element;
                            hovered_->events().enter();
                        }
                        auto x{ static_cast<int>(std::round(ev.x - element->GetPosition().x)) };
                        auto y{ static_cast<int>(std::round(ev.y - element->GetPosition().y)) };
                        hovered_->events().mouse_moved(x, y, ev.deltaX, ev.deltaY);
                    }
                    else if (hovered_) {
                        hovered_->events().exit();
                        hovered_ = nullptr;
                    }
                }
            };

        }

    }

#endif
