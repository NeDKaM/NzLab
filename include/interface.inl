namespace ex {

    template <typename Gfx>
    interface<Gfx>::interface(Ndk::World & world, typename Gfx::value_type const & value) {
        gfx_ = gfx_type::New();

        entity_ = world.CreateEntity();
            auto & node = entity_->AddComponent<Ndk::NodeComponent>();
                node.SetParent(this);
            auto & gfx = entity_->AddComponent<Ndk::GraphicsComponent>();
                gfx.Attach(gfx_);

        data(value);
    }

    template <typename Gfx>
    void interface<Gfx>::data(typename Gfx::value_type const & value) {
        Gfx::apply(gfx_, value);
    }

    template <typename Gfx>
    Nz::Vector2f interface<Gfx>::size() const {
        return Gfx::size(gfx_);
    }

    template <typename Gfx>
    void interface<Gfx>::size(Nz::Vector2f const & value) {
        Gfx::size(gfx_, value);
    }

    template <typename Gfx>
    void interface<Gfx>::show(bool value) {
        entity_->Enable(value);
    }

}