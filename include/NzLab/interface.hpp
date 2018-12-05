#ifndef HPP_EX_INTERFACE_INCLUDED
#define HPP_EX_INTERFACE_INCLUDED

    #include <NDK/EntityOwner.hpp>

    #include <NzLab/base_interface.hpp>

    namespace ex {

        //struct gfx_traits
        //{
        //    class object_type;
        //    class value_type;
        //
        //    static std::unique_ptr<object_type> make();
        //    static void attach(object_type const &, Ndk::GraphicsComponent &);
        //    static void data(object_type &, value_type const &);
        //    static void size(object_type &, Nz::Vector2f const &);
        //    static Nz::Vector2f size(object_type const &);
        //};

        template <typename Gfx, typename Events>
        class interface 
            : public base_interface<Events>
        {
            using gfx_type = typename Gfx::object_type;

            Ndk::EntityOwner entity_;
            std::unique_ptr<gfx_type> gfx_;

            public:
                interface() = delete;
                template <typename... EventsArgs>
                    interface(Ndk::World &, EventsArgs &&...);
                interface(interface const &) = delete;
                interface(interface &&) = default;
                ~interface() = default;

                interface & operator =(interface const &) = delete;
                interface & operator =(interface &&) = default;

                void data(typename Gfx::value_type const &);

                Nz::Vector2f size() const override;

                void size(Nz::Vector2f const &) override;

                void show(bool) override;

                template <typename Style>
                    void style(Style);

                void scissor(Nz::Recti const &) override;
        };

    }

    #include <NzLab/interface.inl>

#endif /* HPP_EX_INTERFACE_INCLUDED */

