#ifndef HPP_EX_CONCEPTS_GRAPHICS_INCLUDED
#define HPP_EX_CONCEPTS_GRAPHICS_INCLUDED

    #include <Nazara/Utility/AbstractTextDrawer.hpp>
    #include <Nazara/Graphics/TextSprite.hpp>
    #include <Nazara/Graphics/Sprite.hpp>

    namespace ex::graphics {
        
        struct text
        {
            using object_type = Nz::TextSprite;
            using value_type = Nz::AbstractTextDrawer;

            static void apply(Nz::TextSpriteRef const & sprite, Nz::AbstractTextDrawer const & value) {
                sprite->Update(value);
            }

            static void size(Nz::TextSpriteRef &, Nz::Vector2f const &) {
                // Nothing to do as a text sprite can't change its size
            }

            static Nz::Vector2f size(Nz::TextSpriteRef const & sprite) {
                return Nz::Vector2f{ sprite->GetBoundingVolume().obb.localBox.GetLengths() };
            }
        };

        struct image
        {
            using object_type = Nz::Sprite;
            using value_type = std::string;

            static void apply(Nz::SpriteRef const & sprite, std::string const & path) {
                sprite->SetTexture(Nz::TextureManager::Get(path));
            }

            static void size(Nz::SpriteRef & sprite, Nz::Vector2f const & value) {
                sprite->SetSize(value);
            }

            static Nz::Vector2f size(Nz::SpriteRef const & sprite) {
                return sprite->GetSize();
            }
        };

    }

#endif /* HPP_EX_CONCEPTS_GRAPHICS_INCLUDED */
