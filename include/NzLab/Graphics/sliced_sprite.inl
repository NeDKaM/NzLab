namespace ex::graphics {

    inline sliced_sprite::sliced_sprite()
        : size_{ 64.f, 64.f }
        , slice_{ 1.f, 1.f }
        , texcoords_{ 1.f, 1.f } {
        for (auto & sprite : sprites_) {
            sprite = Nz::Sprite::New();
            sprite->SetSize(0.f, 0.f);
        }   
        sprites_[4]->SetSize(size_);
        material();
    }

    template <typename... Args>
    static sliced_sprite::ref sliced_sprite::make(Args &&... args) {
        auto obj{ std::make_unique<sliced_sprite>(std::forward<Args>(args)...) };
            obj->SetPersistent(false);
        return obj.release();
    }

    inline Nz::Vector2f sliced_sprite::size() const {
        return size_;
    }

    inline void sliced_sprite::size(Nz::Vector2f const & val) {
        size_ = val;

        recompute();
    }

    inline Nz::Rectf sliced_sprite::texture_coords() const {
        return texcoords_;
    }

    inline void sliced_sprite::texture_coords(Nz::Rectf const & coords) {
        texcoords_.x = std::clamp(coords.x, 0.f, 1.f);
        texcoords_.y = std::clamp(coords.y, 0.f, 1.f);
        texcoords_.width = std::clamp(coords.width, 0.f, 1.f);
        texcoords_.height = std::clamp(coords.height, 0.f, 1.f);
        
        recompute();
    }

    inline Nz::Rectf sliced_sprite::slice() const {
        return slice_;
    }

    inline void sliced_sprite::slice(Nz::Rectf const & coords) {
        slice_.x = std::clamp(coords.x, 0.f, 1.f);
        slice_.y = std::clamp(coords.y, 0.f, 1.f);
        slice_.width = std::clamp(coords.width, 0.f, 1.f);
        slice_.height = std::clamp(coords.height, 0.f, 1.f);
        
        recompute();
    }

    inline void sliced_sprite::material() {
        Nz::MaterialRef mat = Nz::Material::New();
            mat->EnableFaceCulling(false);
            mat->EnableScissorTest(true);
        material(std::move(mat));
    }

    inline void sliced_sprite::material(Nz::MaterialRef mat, bool resize) {
        material(sprites_[0]->GetSkin(), std::move(mat), resize);
    }

    inline void sliced_sprite::material(std::size_t skin, Nz::MaterialRef mat, bool resize) {
        for (auto & sprite : sprites_) {
            sprite->SetMaterial(skin, mat, false);
        }
        if (resize) {
            if (auto & newmat{ sprites_[0]->GetMaterial() }) {
                auto & diffuse{ newmat->GetDiffuseMap() };
                if (diffuse && diffuse->IsValid()) {
                    size(Nz::Vector2f{ Nz::Vector2ui{ diffuse->GetSize() } });
                }
            }
        }
    }

    inline void sliced_sprite::texture(Nz::TextureRef tex, bool resize) {
        texture(sprites_[0]->GetSkin(), std::move(tex), resize);
    }

    inline void sliced_sprite::texture(std::size_t skin, Nz::TextureRef tex, bool resize) {
        for (auto & sprite : sprites_) {
            sprite->SetTexture(skin, tex, false);
        }
        if (resize) {
            if (auto & mat{ sprites_[0]->GetMaterial(skin) }) {
                auto & diffuse{ mat->GetDiffuseMap() };
                if (diffuse && diffuse->IsValid()) {
                    size(Nz::Vector2f{ Nz::Vector2ui{ diffuse->GetSize() } });
                }
            }
        }
    }

    inline Nz::Color sliced_sprite::color() const {
        return sprites_[0]->GetColor();
    }

    inline void sliced_sprite::color(Nz::Color const & col) {
        for (auto & sprite : sprites_) {
            sprite->SetColor(col);
        }
    }

    template <typename Functor>
    void sliced_sprite::for_each(Functor f) const {
        for (auto & sprite : sprites_) {
            f(sprite);
        }
    }

    inline void sliced_sprite::recompute() {
        auto tx0{ texcoords_.x };
        auto tx1{ tx0 + texcoords_.width * slice_.x };
        auto tx2{ tx1 + texcoords_.width * slice_.width };
        auto ty0{ texcoords_.y };
        auto ty1{ ty0 + texcoords_.height * slice_.y };
        auto ty2{ ty1 + texcoords_.height * slice_.height };
        auto tw1{ tx1 - tx0 };
        auto tw2{ tx2 - tx1 };
        auto tw3{ texcoords_.width - tw2 - tw1 };
        auto th1{ ty1 - ty0 };
        auto th2{ ty2 - ty1 };
        auto th3{ texcoords_.height - th2 - th1 };

        sprites_[0]->SetTextureCoords({ tx0, ty0, tw1, th1 });
        sprites_[1]->SetTextureCoords({ tx1, ty0, tw2, th1 });
        sprites_[2]->SetTextureCoords({ tx2, ty0, tw3, th1 });
        sprites_[3]->SetTextureCoords({ tx0, ty1, tw1, th2 });
        sprites_[4]->SetTextureCoords({ tx1, ty1, tw2, th2 });
        sprites_[5]->SetTextureCoords({ tx2, ty1, tw3, th2 });
        sprites_[6]->SetTextureCoords({ tx0, ty2, tw1, th3 });
        sprites_[7]->SetTextureCoords({ tx1, ty2, tw2, th3 });
        sprites_[8]->SetTextureCoords({ tx2, ty2, tw3, th3 });

        auto & tex{ sprites_[0]->GetMaterial()->GetDiffuseMap() };

        Nz::Vector2f texsize{ (tex) ? Nz::Vector2f{ Nz::Vector2ui{ tex->GetSize() } } : size_ };

        auto w1{ tw1 * texsize.x };
        auto w3{ tw3 * texsize.x };
        auto w2{ size_.x - w1 - w3 };
        auto h1{ th1 * texsize.y };
        auto h3{ th3 * texsize.y };
        auto h2{ size_.y - h1 - h3 };
        auto x0{ 0.f };
        auto x1{ w1 };
        auto x2{ x1 + w2 };
        auto y0{ 0.f };
        auto y1{ th1 * texsize.y };
        auto y2{ y1 + h2 };

        sprites_[0]->SetOrigin({ -x0, -y0, 0.f });
        sprites_[1]->SetOrigin({ -x1, -y0, 0.f });
        sprites_[2]->SetOrigin({ -x2, -y0, 0.f });
        sprites_[3]->SetOrigin({ -x0, -y1, 0.f });
        sprites_[4]->SetOrigin({ -x1, -y1, 0.f });
        sprites_[5]->SetOrigin({ -x2, -y1, 0.f });
        sprites_[6]->SetOrigin({ -x0, -y2, 0.f });
        sprites_[7]->SetOrigin({ -x1, -y2, 0.f });
        sprites_[8]->SetOrigin({ -x2, -y2, 0.f });

        sprites_[0]->SetSize({ w1, h1 });
        sprites_[1]->SetSize({ w2, h1 });
        sprites_[2]->SetSize({ w3, h1 });
        sprites_[3]->SetSize({ w1, h2 });
        sprites_[4]->SetSize({ w2, h2 });
        sprites_[5]->SetSize({ w3, h2 });
        sprites_[6]->SetSize({ w1, h3 });
        sprites_[7]->SetSize({ w2, h3 });
        sprites_[8]->SetSize({ w3, h3 });
    }

}
