#ifndef HPP_EX_INTERFACE_INCLUDED
#define HPP_EX_INTERFACE_INCLUDED

    #include <NDK/EntityOwner.hpp>

    #include <NzLab/base_interface.hpp>

    namespace ex {

        //struct gfx_concept
        //{
        //    class object_type;
        //    class value_type;

        //    static void apply(Nz::ObjectRef<object_type> const &, value_type const &);
        //    static void size(Nz::ObjectRef<object_type> const &, Nz::Vector2f const &);
        //    static Nz::Vector2f size(Nz::ObjectRef<object_type> const &);
        //};

        template <typename Gfx>
        class interface 
            : public base_interface
        {
            using gfx_type = std::enable_if_t<std::is_base_of_v<Nz::InstancedRenderable, typename Gfx::object_type>, typename Gfx::object_type>;

            Ndk::EntityOwner entity_;
            Nz::ObjectRef<gfx_type> gfx_;

            public:
                interface() = delete;
                interface(Ndk::World & world);
                interface(Ndk::World & world, typename Gfx::value_type const &);

                interface(interface const &) = delete;
                interface(interface &&) = default;
                ~interface() = default;

                interface & operator =(interface const &) = delete;
                interface & operator =(interface &&) = default;

                void data(typename Gfx::value_type const & value);

                Nz::Vector2f size() const override;

                void size(Nz::Vector2f const & value);

                void show(bool value) override;

            private:
                void scissor(Nz::Recti const &) override;
        };

    }

    #include "interface.inl"

#endif /* HPP_EX_INTERFACE_INCLUDED */

