#ifndef HPP_BASE_INTERFACE_INCLUDED
#define HPP_BASE_INTERFACE_INCLUDED

    #include <Nazara/Utility/Node.hpp>
    #include <Nazara/Core/HandledObject.hpp>

    #include <anchor.hpp>
    #include <object.hpp>

    namespace ex {

        class base_interface
            : public Nz::Node
            , public object<base_interface>
        {
            ex::anchor anchor_;

            public:
                virtual ~base_interface() = default;

                virtual Nz::Vector2f size() const = 0;
                virtual void size(Nz::Vector2f const & value) = 0;

                virtual void show(bool value) = 0;

                virtual void scissor(Nz::Recti const &) = 0;

                void anchor(base_interface const &, ex::anchor const &);
                ex::anchor anchor() const;
        };

    }

    #include "base_interface.inl"

#endif /* HPP_BASE_INTERFACE_INCLUDED */
