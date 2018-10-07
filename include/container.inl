namespace ex {

    template <typename Interface, typename>
    handle<Interface> container::insert(owner<Interface> && owner) {
        return insert(owner.release());
    }

    template <typename Interface, typename... Args, typename>
    handle<Interface> container::insert(Args &&... args) {
        return insert(new Interface(std::forward<Args>(args)...));
    }

    template <typename Interface, typename>
    inline owner<Interface> container::release(Interface * element) {
        if (auto it = std::find_if(elements_.begin(), elements_.end(), [element](owner_type const & elm) { return element == *elm; })
        ; it != elements_.end()) {
            element->SetParent(nullptr);
            owner<Interface> owner{ it->release() };
            elements_.erase(it);
            return owner;
        }
        NazaraError("Element is not part of container");
        return {};
    }

    void container::collocate() const
    {
        for (auto & element : elements_) {
            if (!*element) {
                continue;
            }
            ex::anchor anch { element->anchor() };
            Nz::Vector2f elmsize { element->size() };
            element->SetInitialPosition(size_ * anch.min);
            element->size({
                (anch.max.x != anch.min.x) ? size_.x * (anch.max.x - anch.min.x) : elmsize.x
                , (anch.max.y != anch.min.y) ? size_.y * (anch.max.y - anch.min.y) : elmsize.y
            });
        }
    }

    void container::size(Nz::Vector2f const & value) {
        size_ = value;
        collocate();
    }

    Nz::Vector2f container::size() const {
        return size_;
    }

    std::size_t container::count() const {
        return elements_.size();
    }

    template <typename Interface>
    handle<Interface> container::insert(Interface * element) {
        if (!element) {
            NazaraError("Element is not valid");
            return {};
        }
        element->SetParent(this);
        for (auto & elm : elements_) {
            if (*elm == element) {
                return { element };
            }
        }
        return { *elements_.emplace_back(element) };
    }

    void container::show(bool value) {
        for (auto & element : elements_) {
            element->show(value);
        }
    }

    void container::scissor(bool value) {
        Nz::Vector2i pos{ Nz::Vector2f{ GetPosition() } };
        Nz::Vector2i siz{ size() };
        Nz::Recti rect{ (value) ? Nz::Recti{ pos.x, pos.y, siz.x, siz.y } : Nz::Recti{ -1, -1 } };
        scissor(rect);
    }

    void container::scissor(Nz::Recti const & rect) {
        for (auto & elm : elements_) {
            elm->scissor(rect);
        }
    }

}