#ifndef HPP_EX_GRAPHICS_SLICED_SPRITE_INCLUDED
#define HPP_EX_GRAPHICS_SLICED_SPRITE_INCLUDED

    #include <Nazara/Graphics/Sprite.hpp>
    
    namespace ex::graphics {

        class sliced_sprite
            : public Nz::RefCounted
        {
            Nz::Vector2f size_;
            Nz::Rectf slice_;
            Nz::Rectf texcoords_;
            std::array<Nz::SpriteRef, 9> sprites_;

            public:
                using ref = Nz::ObjectRef<sliced_sprite>;
                using const_ref = Nz::ObjectRef<sliced_sprite const>;
                using library = Nz::ObjectLibrary<sliced_sprite>;

                inline sliced_sprite();
                sliced_sprite(sliced_sprite const &) = delete;
                sliced_sprite(sliced_sprite &&) = default;

                template <typename... Args>
                    static ref make(Args &&...);

                ~sliced_sprite() = default;

                sliced_sprite & operator =(sliced_sprite const &) = delete;
                sliced_sprite & operator =(sliced_sprite &&) = default;

                inline Nz::Vector2f size() const;
                inline void size(Nz::Vector2f const &);

                inline Nz::Rectf texture_coords() const;
                inline void texture_coords(Nz::Rectf const &);

                inline Nz::Rectf slice() const;
                inline void slice(Nz::Rectf const &);
                    
                inline void material();
                inline void material(Nz::MaterialRef, bool = true);
                inline void material(std::size_t, Nz::MaterialRef, bool = true);
                bool material(std::string const &, bool = true);
                bool material(std::size_t, std::string const &, bool = true);

                inline void texture(Nz::TextureRef, bool = true);
                inline void texture(std::size_t, Nz::TextureRef, bool = true);
                bool texture(std::string const &, bool = true);
                bool texture(std::size_t, std::string const &, bool = true);

                inline Nz::Color color() const;
                inline void color(Nz::Color const &);

                template <typename Functor>
                    void for_each(Functor) const;

            private:
                inline void recompute();

                friend library;
                static bool Initialize();
                static void Uninitialize();
                static library::LibraryMap s_library;
        };

    }

    #include <NzLab/Graphics/sliced_sprite.inl>

#endif
