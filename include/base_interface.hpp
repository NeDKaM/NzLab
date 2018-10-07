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

                void anchor(ex::anchor const & value) {
                    anchor_ = value;
                }
                ex::anchor anchor() const {
                    return anchor_;
                }
        };

    }

#endif /* HPP_BASE_INTERFACE_INCLUDED */
