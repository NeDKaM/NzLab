namespace ex {

    namespace events {

        // dispatcher

        template <typename Events, typename ElementType>
        dispatcher<Events, ElementType>::dispatcher(ex::container<ElementType, Events> * cont)
            : container_{ cont }
            , hovered_{ nullptr }
            , kb_owner_{ nullptr } {
        }

        /*
            Note: events are managed as they are in Nazara
        */

        template <typename Events, typename ElementType>
        void dispatcher<Events, ElementType>::keyboard_owner(ElementType * elm) {
            if (elm != kb_owner_) {
                if (kb_owner_) {
                    kb_owner_->events().focus_lost();
                }
                kb_owner_ = elm;
                if (kb_owner_) {
                    kb_owner_->events().focus_gained();
                }
            }
        }

        template <typename Events, typename ElementType>
        void dispatcher<Events, ElementType>::mouse_moved(int x, int y, int dx, int dy) {
            ElementType * element{ nullptr };
            float best_area{ std::numeric_limits<float>::infinity() };
            container_->for_each([&](ElementType * elm) {
                auto position{ Nz::Vector3i{ elm->GetPosition() } };
                auto size{ elm->size() };
                Nz::Recti elm_area{ position.x, position.y, static_cast<int>(size.x), static_cast<int>(size.y) };
                if (elm_area.Contains(x, y)) {
                    float area{ size.x * size.y };
                    if (area < best_area) {
                        best_area = area;
                        element = elm;
                    }
                }
            });
            if (element) {
                if (element != hovered_.get()) {
                    if (hovered_) {
                        hovered_->events().exit();
                    }
                    hovered_.reset(element);
                    hovered_->events().enter();
                }
                auto lx{ static_cast<int>(std::round(x - element->GetPosition().x)) };
                auto ly{ static_cast<int>(std::round(y - element->GetPosition().y)) };
                hovered_->events().mouse_moved(lx, ly, dx, dy);
            }
            else if (hovered_) {
                hovered_->events().exit();
                hovered_.reset();
            }
        }

        template <typename Events, typename ElementType>
        void dispatcher<Events, ElementType>::enter() {

        }

        template <typename Events, typename ElementType>
        void dispatcher<Events, ElementType>::exit() {
            if (hovered_) {
                hovered_->events().exit();
                hovered_.reset();
            }
        }

        template <typename Events, typename ElementType>
        void dispatcher<Events, ElementType>::button_pressed(Nz::Mouse::Button btn, int x, int y) {
            if (hovered_) {
                auto lx{ static_cast<int>(std::round(x - hovered_->GetPosition().x)) };
                auto ly{ static_cast<int>(std::round(y - hovered_->GetPosition().y)) };
                hovered_->events().button_pressed(btn, lx, ly);
            }
        }

        template <typename Events, typename ElementType>
        void dispatcher<Events, ElementType>::button_released(Nz::Mouse::Button btn, int x, int y) {
            if (hovered_) {
                auto lx{ static_cast<int>(std::round(x - hovered_->GetPosition().x)) };
                auto ly{ static_cast<int>(std::round(y - hovered_->GetPosition().y)) };
                hovered_->events().button_released(btn, lx, ly);
            }
        }

        template <typename Events, typename ElementType>
        void dispatcher<Events, ElementType>::key_pressed(Nz::WindowEvent::KeyEvent const & ev) {
            if (kb_owner_) {
                kb_owner_->events().key_pressed(ev);
            }
        }

        template <typename Events, typename ElementType>
        void dispatcher<Events, ElementType>::key_released(Nz::WindowEvent::KeyEvent const & ev) {
            if (kb_owner_) {
                kb_owner_->events().key_released(ev);
            }
        }

        template <typename Events, typename ElementType>
        void dispatcher<Events, ElementType>::text_entered(char32_t ch, bool repeat) {
            if (kb_owner_) {
                kb_owner_->events().text_entered(ch, repeat);
            }
        }

        template <typename Events, typename ElementType>
        void dispatcher<Events, ElementType>::focus_gained() {

        }

        template <typename Events, typename ElementType>
        void dispatcher<Events, ElementType>::focus_lost() {

        }

        // default_inputs_dispatch

        template <typename ElementType>
        default_inputs_dispatch<ElementType>::default_inputs_dispatch(ex::container<ElementType, default_inputs_dispatch> * cont, Nz::EventHandlerHandle events)
            : dispatcher(cont)
            , h_events_{ std::move(events) } {
            slot_mouse_moved.Connect(h_events_->OnMouseMoved, this, &default_inputs_dispatch::on_mouse_moved);
            slot_mouse_exit.Connect(h_events_->OnMouseLeft, this, &default_inputs_dispatch::on_mouse_exit);
            slot_button_pressed.Connect(h_events_->OnMouseButtonPressed, this, &default_inputs_dispatch::on_button_pressed);
            slot_button_released.Connect(h_events_->OnMouseButtonReleased, this, &default_inputs_dispatch::on_button_released);
            slot_key_pressed.Connect(h_events_->OnKeyPressed, this, &default_inputs_dispatch::on_key_pressed);
            slot_key_released.Connect(h_events_->OnKeyReleased, this, &default_inputs_dispatch::on_key_released);
            slot_text_entered.Connect(h_events_->OnTextEntered, this, &default_inputs_dispatch::on_text_entered);
        }

        template <typename ElementType>
        void default_inputs_dispatch<ElementType>::on_mouse_moved(Nz::EventHandler const *, Nz::WindowEvent::MouseMoveEvent const & ev) {
            mouse_moved(ev.x, ev.y, ev.deltaX, ev.deltaY);
        }

        template <typename ElementType>
        void default_inputs_dispatch<ElementType>::on_mouse_exit(Nz::EventHandler const *) {
            exit();
        }

        template <typename ElementType>
        void default_inputs_dispatch<ElementType>::on_button_pressed(Nz::EventHandler const *, Nz::WindowEvent::MouseButtonEvent const & ev) {
            button_pressed(ev.button, ev.x, ev.y);
        }

        template <typename ElementType>
        void default_inputs_dispatch<ElementType>::on_button_released(Nz::EventHandler const *, Nz::WindowEvent::MouseButtonEvent const & ev) {
            button_released(ev.button, ev.x, ev.y);
        }

        template <typename ElementType>
        void default_inputs_dispatch<ElementType>::on_key_pressed(Nz::EventHandler const *, Nz::WindowEvent::KeyEvent const & ev) {
            key_pressed(ev);
        }

        template <typename ElementType>
        void default_inputs_dispatch<ElementType>::on_key_released(Nz::EventHandler const *, Nz::WindowEvent::KeyEvent const & ev) {
            key_released(ev);
        }

        template <typename ElementType>
        void default_inputs_dispatch<ElementType>::on_text_entered(Nz::EventHandler const *, Nz::WindowEvent::TextEvent const & ev) {
            text_entered(ev.character, ev.repeated);
        }

    }

}

