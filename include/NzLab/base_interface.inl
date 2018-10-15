namespace ex {

    base_interface::base_interface() 
        : anchor_{ }
        , padding_{ }
        , scissor_{ } {
    }

    void base_interface::anchor(Nz::Vector3f const & position, Nz::Vector2f const & size, ex::anchor const & value) {
        anchor_ = value;
        Nz::Vector2f siz{ this->size() };
        SetInitialPosition(size * anchor_.min + Nz::Vector2f{ position });
        this->size({
            (anchor_.max.x != anchor_.min.x) ? size.x * (anchor_.max.x - anchor_.min.x) : siz.x
            , (anchor_.max.y != anchor_.min.y) ? size.y * (anchor_.max.y - anchor_.min.y) : siz.y
        });
    }

    void base_interface::anchor(base_interface const & element, ex::anchor const & value) {
        anchor(element.GetPosition(), element.size(), value);
    }

    ex::anchor base_interface::anchor() const {
        return anchor_;
    }

    void base_interface::padding(ex::padding const & value) {
        Nz::Vector2f siz{ size() };
        padding_ = value;
        size(siz);
    }

    ex::padding base_interface::padding() const {
        return padding_;
    }

    void base_interface::scissor(Nz::Recti const & value) {
        scissor_ = value;
    }

    Nz::Recti base_interface::scissor() const {
        return scissor_;
    }

    void base_interface::InvalidateNode() {
        Node::InvalidateNode();
        Nz::Vector2i pos{ Nz::Vector2f{ GetPosition() } };
        Nz::Recti current{ scissor() };
        Nz::Recti rect{ pos.x, pos.y, current.width, current.height };
        scissor(rect);
    }
}
