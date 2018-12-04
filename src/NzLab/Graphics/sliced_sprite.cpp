#include <NzLab/Graphics/sliced_sprite.hpp>

namespace ex::graphics {

    bool sliced_sprite::material(std::string const & name, bool resize) {
        auto mat{ Nz::MaterialLibrary::Query(name) };
        if (!mat) {
            mat = Nz::MaterialManager::Get(name);
            if (!mat) {
                NazaraError("Failed to get material \"" + name + "\"");
                return false;
            }
        }
        material(std::move(mat), resize);
        return true;
    }

    bool sliced_sprite::material(std::size_t skin, std::string const & name, bool resize) {
        auto mat{ Nz::MaterialLibrary::Query(name) };
        if (!mat) {
            mat = Nz::MaterialManager::Get(name);
            if (!mat) {
                NazaraError("Failed to get material \"" + name + "\"");
                return false;
            }
        }
        material(skin, std::move(mat), resize);
        return true;
    }

    bool sliced_sprite::texture(std::string const & name, bool resize) {
        auto tex{ Nz::TextureLibrary::Query(name) };
        if (!tex) {
            tex = Nz::TextureManager::Get(name);
            if (!tex) {
                NazaraError("Failed to get texture \"" + name + "\"");
                return false;
            }
        }
        texture(std::move(tex), resize);
        return true;
    }

    bool sliced_sprite::texture(std::size_t skin, std::string const & name, bool resize) {
        auto tex{ Nz::TextureLibrary::Query(name) };
        if (!tex) {
            tex = Nz::TextureManager::Get(name);
            if (!tex) {
                NazaraError("Failed to get texture \"" + name + "\"");
                return false;
            }
        }
        texture(skin, std::move(tex), resize);
        return true;
    }

    bool sliced_sprite::Initialize() {
        if (!sliced_sprite::library::Initialize()) {
            NazaraError("Failed to initialise [sliced_sprite] library");
            return false;
        }
        return true;
    }

    void sliced_sprite::Uninitialize() {
        sliced_sprite::library::Uninitialize();
    }

    sliced_sprite::library::LibraryMap sliced_sprite::s_library;

}
