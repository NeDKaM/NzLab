#ifndef HPP_EX_CONCEPTS_GRAPHICS_INCLUDED
#define HPP_EX_CONCEPTS_GRAPHICS_INCLUDED

    #include <Nazara/Utility/AbstractTextDrawer.hpp>
    #include <Nazara/Graphics/TextSprite.hpp>
    #include <Nazara/Graphics/Sprite.hpp>

    namespace ex::graphics {
        
        struct text
        {
            using object_type = std::pair<Nz::TextSpriteRef const, Nz::SimpleTextDrawer>;
            using value_type = std::string;

            static std::unique_ptr<object_type> make() {
                return std::make_unique<object_type>(Nz::TextSprite::New(), Nz::SimpleTextDrawer{ });
            }

            static Nz::InstancedRenderableRef renderable(object_type const & obj) {
                return obj.first;
            }

            static void data(object_type & sprite, std::string const & value) {
                sprite.second.SetText(value);
                sprite.first->Update(sprite.second);
            }

            static void size(object_type &, Nz::Vector2f const &) {
                // Nothing to do as a text sprite can't change its size
            }

            static Nz::Vector2f size(object_type const & sprite) {
                return Nz::Vector2f{ sprite.first->GetBoundingVolume().obb.localBox.GetLengths() };
            }
        };

        struct image
        {
            using object_type = Nz::SpriteRef;
            using value_type = std::string;

            static std::unique_ptr<Nz::SpriteRef> make() {
                return std::make_unique<Nz::SpriteRef>(Nz::Sprite::New());
            }

            static Nz::InstancedRenderableRef renderable(Nz::SpriteRef const & obj) {
                return obj;
            }

            static void apply(Nz::SpriteRef const & sprite, std::string const & path) {
                sprite->SetTexture(Nz::TextureManager::Get(path));
            }

            static void size(Nz::SpriteRef const & sprite, Nz::Vector2f const & value) {
                sprite->SetSize(value);
            }

            static Nz::Vector2f size(Nz::SpriteRef const & sprite) {
                return sprite->GetSize();
            }
        };

    }

#endif /* HPP_EX_CONCEPTS_GRAPHICS_INCLUDED */
