namespace ex {

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

}
