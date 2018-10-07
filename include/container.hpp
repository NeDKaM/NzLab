#ifndef HPP_CONTAINER_INCLUDED
#define HPP_CONTAINER_INCLUDED
    
    #include <list>

    #include <base_interface.hpp>
    #include <anchor.hpp>

    namespace ex {

        class container
            : public base_interface
        {
            using owner_type = owner<base_interface>;

            std::list<owner_type> elements_;
            Nz::Vector2f size_;

            public:
                container() = default;
                container(container const &) = delete;
                container(container &&) = default;
                ~container() = default;

                container & operator =(container const &) = delete;
                container & operator =(container &&) = default;

                template <typename Interface, typename = std::enable_if_t<std::is_base_of_v<base_interface, Interface>>>
                    handle<Interface> insert(owner<Interface> && owner);
                template <typename Interface, typename... Args, typename = std::enable_if_t<std::is_base_of_v<base_interface, Interface>>>
                    handle<Interface> insert(Args &&... args);

                template <typename Interface, typename = std::enable_if_t<std::is_base_of_v<base_interface, Interface>>>
                    owner<Interface> release(Interface * element);

                void collocate() const;

                void size(Nz::Vector2f const & value) override;

                Nz::Vector2f size() const override;

                std::size_t count() const;

                void show(bool value) override;

                void scissor(bool value);

            private:
                template <typename Interface>
                    handle<Interface> insert(Interface * element);

                void scissor(Nz::Recti const &) override;
        };

    }

    #include "container.inl"

#endif /* HPP_CONTAINER_INCLUDED */
