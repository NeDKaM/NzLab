namespace ex {

    template <typename Gfx, typename Events>
    template <typename... EventsArgs>
    interface<Gfx, Events>::interface(Ndk::World & world, EventsArgs &&... args)
        : base_interface<Events>{ std::forward<EventsArgs>(args)... } {
        gfx_ = Gfx::make();
        entity_ = world.CreateEntity();
            auto & node = entity_->AddComponent<Ndk::NodeComponent>();
                node.SetParent(this);
            auto & gfx = entity_->AddComponent<Ndk::GraphicsComponent>();
                gfx.Attach(Gfx::renderable(*gfx_.get()));
    }

    template <typename Gfx, typename Events>
    void interface<Gfx, Events>::data(typename Gfx::value_type const & value) {
        Gfx::data(*gfx_.get(), value);
        size(size());
    }

    template <typename Gfx, typename Events>
    Nz::Vector2f interface<Gfx, Events>::size() const {
        ex::padding pad{ padding() };
        return Gfx::size(*gfx_.get()) + Nz::Vector2f{ pad.left + pad.right, pad.top + pad.bottom };
    }

    template <typename Gfx, typename Events>
    void interface<Gfx, Events>::size(Nz::Vector2f const & value) {
        ex::padding pad{ padding() };
        Nz::Vector2f contentsize{ value.x - pad.left - pad.right, value.y - pad.top - pad.bottom };
        Gfx::size(*gfx_.get(), contentsize);
        entity_->GetComponent<Ndk::NodeComponent>().SetPosition(Nz::Vector2f{ pad.left, pad.top });
        Nz::Vector2i pos{ Nz::Vector2f{ GetPosition() } + Nz::Vector2f{ pad.left, pad.top } };
        scissor({ pos.x, pos.y, static_cast<int>(contentsize.x), static_cast<int>(contentsize.y) });
    }

    template <typename Gfx, typename Events>
    void interface<Gfx, Events>::show(bool value) {
        entity_->Enable(value);
    }

    template <typename Gfx, typename Events>
    void interface<Gfx, Events>::scissor(Nz::Recti const & rect) {
        base_interface<Events>::scissor(rect);
        entity_->GetComponent<Ndk::GraphicsComponent>().SetScissorRect(rect);
    }
    
    template <typename Gfx, typename Events>
    void interface<Gfx, Events>::style(ex::style<Gfx> & opt) {
        opt.apply(*gfx_.get());
        size(size());
    }

}


