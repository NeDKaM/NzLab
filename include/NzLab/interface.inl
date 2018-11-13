namespace ex {

    template <typename Gfx>
    interface<Gfx>::interface(Ndk::World & world) {
        gfx_ = gfx_type::New();

        gfx_ = Gfx::make();
        entity_ = world.CreateEntity();
            auto & node = entity_->AddComponent<Ndk::NodeComponent>();
                node.SetParent(this);
            auto & gfx = entity_->AddComponent<Ndk::GraphicsComponent>();
    }

    template <typename Gfx>
    interface<Gfx>::interface(Ndk::World & world, typename Gfx::value_type const & value) 
        : interface(world) {
        data(value);
                gfx.Attach(Gfx::renderable(*gfx_.get()));
    }

    template <typename Gfx>
    void interface<Gfx>::data(typename Gfx::value_type const & value) {
        Gfx::data(*gfx_.get(), value);
        size(size());
    }

    template <typename Gfx>
    Nz::Vector2f interface<Gfx>::size() const {
        ex::padding pad{ padding() };
        return Gfx::size(*gfx_.get()) + Nz::Vector2f{ pad.left + pad.right, pad.top + pad.bottom };
    }

    template <typename Gfx>
    void interface<Gfx>::size(Nz::Vector2f const & value) {
        ex::padding pad{ padding() };
        Nz::Vector2f contentsize{ value.x - pad.left - pad.right, value.y - pad.top - pad.bottom };
        Gfx::size(*gfx_.get(), contentsize);
        entity_->GetComponent<Ndk::NodeComponent>().SetPosition(Nz::Vector2f{ pad.left, pad.top });
        Nz::Vector2i pos{ Nz::Vector2f{ GetPosition() } + Nz::Vector2f{ pad.left, pad.top } };
        scissor({ pos.x, pos.y, static_cast<int>(contentsize.x), static_cast<int>(contentsize.y) });
    }

    template <typename Gfx>
    void interface<Gfx>::show(bool value) {
        entity_->Enable(value);
    }

    template <typename Gfx>
    void interface<Gfx>::scissor(Nz::Recti const & rect) {
        base_interface::scissor(rect);
        entity_->GetComponent<Ndk::GraphicsComponent>().SetScissorRect(rect);
    }

}