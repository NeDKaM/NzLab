namespace ex {

    void base_interface::anchor(base_interface const & element, ex::anchor const & value) {
        anchor_ = value;
        Nz::Vector2f elmsize{ element.size() };
        Nz::Vector2f siz{ size() };
        SetInitialPosition(elmsize * anchor_.min + Nz::Vector2f{ element.GetPosition() });
        size({
            (anchor_.max.x != anchor_.min.x) ? elmsize.x * (anchor_.max.x - anchor_.min.x) : siz.x
            , (anchor_.max.y != anchor_.min.y) ? elmsize.y * (anchor_.max.y - anchor_.min.y) : siz.y
        });
    }

    ex::anchor base_interface::anchor() const {
        return anchor_;
    }

}
