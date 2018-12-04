#ifndef HPP_EX_EVENTS_DEFAULT_INPUTS_INCLUDED
#define HPP_EX_EVENTS_DEFAULT_INPUTS_INCLUDED

    #include <Nazara/Core/Signal.hpp>
    #include <Nazara/Platform/Event.hpp>

    #include <NzLab/container.hpp>

    namespace ex::events {
            
        struct default_inputs
        {
            Nz::Signal<int /*x*/, int /*y*/, int /*dx*/, int /*dy*/> mouse_moved;
            Nz::Signal<> enter;
            Nz::Signal<> exit;

            Nz::Signal<Nz::Mouse::Button, int /*x*/, int /*y*/> button_pressed;
            Nz::Signal<Nz::Mouse::Button, int /*x*/, int /*y*/> button_released;

            Nz::Signal<Nz::WindowEvent::KeyEvent const &> key_pressed;
            Nz::Signal<Nz::WindowEvent::KeyEvent const &> key_released;
            Nz::Signal<char32_t, bool /*repeated*/> text_entered;

            Nz::Signal<> focus_gained;
            Nz::Signal<> focus_lost;
        };

        template <typename Events, typename ElementType>
        struct default_dispatcher
        {
            explicit default_dispatcher(ex::container<ElementType, Events> *);

            void keyboard_owner(ElementType *);

            void mouse_moved(int, int, int, int);
            void enter();
            void exit();
            void button_pressed(Nz::Mouse::Button, int, int);
            void button_released(Nz::Mouse::Button, int, int);
            void key_pressed(Nz::WindowEvent::KeyEvent const &);
            void key_released(Nz::WindowEvent::KeyEvent const &);
            void text_entered(char32_t, bool);
            void focus_gained();
            void focus_lost();

            private:
                ex::container<ElementType, Events> * container_;
                ex::handle<ElementType> hovered_;
                ex::handle<ElementType> kb_owner_;
        };

        template <typename ElementType>
        class default_inputs_dispatch
            : public default_dispatcher<default_inputs_dispatch<ElementType>, ElementType>
        {
            Nz::EventHandlerHandle h_events_;

            public:
                explicit default_inputs_dispatch(ex::container<ElementType, default_inputs_dispatch> *, Nz::EventHandlerHandle);

            protected:
                template <typename... Args>
                    using slot_type = typename Nz::Signal<Nz::EventHandler const *, Args...>::ConnectionGuard;

                slot_type<Nz::WindowEvent::MouseMoveEvent const &> slot_mouse_moved;
                slot_type<> slot_mouse_exit;
                slot_type<Nz::WindowEvent::MouseButtonEvent const &> slot_button_pressed;
                slot_type<Nz::WindowEvent::MouseButtonEvent const &> slot_button_released;
                slot_type<Nz::WindowEvent::KeyEvent const &> slot_key_pressed;
                slot_type<Nz::WindowEvent::KeyEvent const &> slot_key_released;
                slot_type<Nz::WindowEvent::TextEvent const &> slot_text_entered;

            private:
                void on_mouse_moved(Nz::EventHandler const *, Nz::WindowEvent::MouseMoveEvent const &);
                void on_mouse_exit(Nz::EventHandler const *);
                void on_button_pressed(Nz::EventHandler const *, Nz::WindowEvent::MouseButtonEvent const &);
                void on_button_released(Nz::EventHandler const *, Nz::WindowEvent::MouseButtonEvent const &);
                void on_key_pressed(Nz::EventHandler const *, Nz::WindowEvent::KeyEvent const &);
                void on_key_released(Nz::EventHandler const *, Nz::WindowEvent::KeyEvent const &);
                void on_text_entered(Nz::EventHandler const *, Nz::WindowEvent::TextEvent const &);
        };

    }

    #include <NzLab/Events/default_inputs.inl>

#endif
