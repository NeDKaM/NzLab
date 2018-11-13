namespace ex {

    template <typename Gfx>
    style<Gfx>::option::option(modifier_type const & modifier)
        : modifier_{ modifier } {
    }

    template <typename Gfx>
    style<Gfx>::option::option(modifier_type && modifier)
        : modifier_{ std::move(modifier) } {
    }

    template <typename Gfx>
    template <typename Modifier>
    style<Gfx>::option::option(Modifier modifier)
        : modifier_{ std::move(modifier) } {
    }

    template <typename Gfx>
    void style<Gfx>::option::apply(typename Gfx::object_type & object) {
        modifier_(object);
    }

}
