namespace ex {

    template <typename Events>
    template <typename... EventsArgs>
    base_interface<Events>::base_interface(EventsArgs &&... args) 
        : anchor_{ }
        , padding_{ }
        , scissor_{ }
        , events_{ std::forward<EventsArgs>(args)... } {
    }

    template <typename Events>
    void base_interface<Events>::anchor(Nz::Vector3f const & position, Nz::Vector2f const & size, ex::anchor const & value) {
        anchor_ = value;
        Nz::Vector2f siz{ this->size() };
        SetInitialPosition(size * anchor_.min + Nz::Vector2f{ position });
        this->size({
            (anchor_.max.x != anchor_.min.x) ? size.x * (anchor_.max.x - anchor_.min.x) : siz.x
            , (anchor_.max.y != anchor_.min.y) ? size.y * (anchor_.max.y - anchor_.min.y) : siz.y
        });
    }

    template <typename Events>
    void base_interface<Events>::anchor(base_interface const & element, ex::anchor const & value) {
        anchor(element.GetPosition(), element.size(), value);
    }

    template <typename Events>
    ex::anchor base_interface<Events>::anchor() const {
        return anchor_;
    }

    template <typename Events>
    void base_interface<Events>::padding(ex::padding const & value) {
        Nz::Vector2f siz{ size() };
        padding_ = value;
        size(siz);
    }

    template <typename Events>
    ex::padding base_interface<Events>::padding() const {
        return padding_;
    }

    template <typename Events>
    void base_interface<Events>::scissor(Nz::Recti const & value) {
        scissor_ = value;
    }

    template <typename Events>
    Nz::Recti base_interface<Events>::scissor() const {
        return scissor_;
    }

    template <typename Events>
    void base_interface<Events>::InvalidateNode() {
        Node::InvalidateNode();
        Nz::Vector2i pos{ Nz::Vector2f{ GetPosition() } };
        Nz::Recti current{ scissor() };
        Nz::Recti rect{ pos.x, pos.y, current.width, current.height };
        scissor(rect);
    }

    template <typename Events>
    Events & base_interface<Events>::events() {
        return events_;
    }

}
