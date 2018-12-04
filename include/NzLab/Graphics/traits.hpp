#ifndef HPP_EX_CONCEPTS_GRAPHICS_INCLUDED
#define HPP_EX_CONCEPTS_GRAPHICS_INCLUDED

    #include <Nazara/Utility/SimpleTextDrawer.hpp>
    #include <Nazara/Graphics/TextSprite.hpp>
    #include <Nazara/Graphics/Sprite.hpp>

    #include <NzLab/Graphics/sliced_sprite.hpp>

    namespace ex::graphics::traits {
        
        struct text
        {
            using object_type = std::pair<Nz::TextSpriteRef const, Nz::SimpleTextDrawer>;
            using value_type = std::string;

            static std::unique_ptr<object_type> make() {
                return std::make_unique<object_type>(Nz::TextSprite::New(), Nz::SimpleTextDrawer{ });
            }

            static void attach(object_type const & obj, Ndk::GraphicsComponent & comp) {
                comp.Attach(obj.first);
            }

            static void data(object_type & text, std::string const & value) {
                text.second.SetText(value);
                text.first->Update(text.second);
            }

            static void size(object_type & text, Nz::Vector2f const & value) {
                // scale ?
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

            static void attach(object_type const & obj, Ndk::GraphicsComponent & comp) {
                comp.Attach(obj);
            }

            static void data(Nz::SpriteRef const & sprite, std::string const & path) {
                sprite->SetTexture(Nz::TextureManager::Get(path));
            }

            static void size(Nz::SpriteRef const & sprite, Nz::Vector2f const & value) {
                sprite->SetSize(value);
            }

            static Nz::Vector2f size(Nz::SpriteRef const & sprite) {
                return sprite->GetSize();
            }
        };

        struct sliced_image
        {
            using object_type = ex::graphics::sliced_sprite::ref;
            using value_type = std::string;

            static std::unique_ptr<object_type> make() {
                return std::make_unique<object_type>(ex::graphics::sliced_sprite::make());
            }

            static void attach(object_type const & obj, Ndk::GraphicsComponent & comp) {
                obj->for_each([&](Nz::SpriteRef const & sprite) {
                    comp.Attach(sprite);
                });
            }

            static void data(object_type const & sprite, std::string const & path) {
                sprite->texture(Nz::TextureManager::Get(path));
            }

            static void size(object_type const & sprite, Nz::Vector2f const & value) {
                sprite->size(value);
            }

            static Nz::Vector2f size(object_type const & sprite) {
                return sprite->size();
            }
        };

    }

#endif /* HPP_EX_CONCEPTS_GRAPHICS_INCLUDED */
