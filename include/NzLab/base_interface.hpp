#ifndef HPP_BASE_INTERFACE_INCLUDED
#define HPP_BASE_INTERFACE_INCLUDED

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

        class base_interface
            : public Nz::Node
            , public object<base_interface>
        {
            ex::anchor anchor_;
            ex::padding padding_;
            Nz::Recti scissor_;
            Nz::Signal<Nz::Node const *>::ConnectionGuard nodeinvalidated_;

            public:
                base_interface();
                virtual ~base_interface() = default;

                virtual Nz::Vector2f size() const = 0;
                virtual void size(Nz::Vector2f const &) = 0;

                virtual void show(bool value) = 0;

                virtual void scissor(Nz::Recti const &);
                Nz::Recti scissor() const;

                void anchor(Nz::Vector3f const &, Nz::Vector2f const &, ex::anchor const &);
                void anchor(base_interface const &, ex::anchor const &);
                ex::anchor anchor() const;

                void padding(ex::padding const &);
                ex::padding padding() const;
        };

    }

    #include "base_interface.inl"

#endif /* HPP_BASE_INTERFACE_INCLUDED */
