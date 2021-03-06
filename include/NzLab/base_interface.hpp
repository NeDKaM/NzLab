#ifndef HPP_EX_BASE_INTERFACE_INCLUDED
#define HPP_EX_BASE_INTERFACE_INCLUDED

    #include <Nazara/Utility/Node.hpp>

    #include <NzLab/anchor.hpp>
    #include <NzLab/object.hpp>

    namespace ex {

        struct padding
        {
            float left, top, right, bottom;

            static padding none() {
                return { 0.f, 0.f, 0.f, 0.f };
            };
        };
    
        template <typename Events>
        class base_interface
            : public Nz::Node
            , public object<base_interface<Events>>
        {
            ex::anchor anchor_;
            ex::padding padding_;
            Nz::Recti scissor_;
            Events events_;

            public:
                using events_type = Events;

                template <typename... EventsArgs> base_interface(EventsArgs &&... args);
                virtual ~base_interface() = default;

                virtual Nz::Vector2f size() const = 0;
                virtual void size(Nz::Vector2f const &) = 0;

                virtual void show(bool) = 0;

                virtual void scissor(Nz::Recti const &);
                Nz::Recti scissor() const;

                void anchor(Nz::Vector3f const &, Nz::Vector2f const &, ex::anchor const &);
                template <typename Interface> void anchor(Interface const &, ex::anchor const &);
                ex::anchor anchor() const;

                void padding(ex::padding const &);
                ex::padding padding() const;

                Events & events();

            private:
                void InvalidateNode() override;
        };

    }

    #include "base_interface.inl"

#endif /* HPP_EX_BASE_INTERFACE_INCLUDED */
