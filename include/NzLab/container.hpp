#ifndef HPP_EX_CONTAINER_INCLUDED
#define HPP_EX_CONTAINER_INCLUDED
    
    #include <list>

    #include <NzLab/base_interface.hpp>
    #include <NzLab/anchor.hpp>

    namespace ex {

        template <typename ElementType, typename Events>
        class container
            : public base_interface<Events>
        {
            using owner_type = owner<ElementType>;

            mutable std::list<owner_type> elements_;
            Nz::Vector2f contentsize_;

            public:
                template <typename... EventsArgs> container(EventsArgs &&...);
                container(container const &) = delete;
                container(container &&) = default;
                ~container() = default;

                container & operator =(container const &) = delete;
                container & operator =(container &&) = default;

                template <typename Interface> handle<Interface> insert(owner<Interface> &&);
                template <typename Interface, typename... Args> handle<Interface> insert(Args &&...);

                template <typename Interface> owner<Interface> release(Interface &);

                void size(Nz::Vector2f const &) override;

                Nz::Vector2f size() const override;

                std::size_t count() const;

                void show(bool) override;

                void scissor(bool);

                template <typename Functor> void for_each(Functor);
                template <typename Functor> void for_each(Functor) const;

            private:
                template <typename Interface> handle<Interface> insert(Interface *);

                void scissor(Nz::Recti const &) override;
        };

    }

    #include <NzLab/container.inl>

#endif /* HPP_EX_CONTAINER_INCLUDED */
