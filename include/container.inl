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
        ex::padding pad{ padding() };
        Nz::Vector3f pos{ GetPosition() + Nz::Vector3f{ pad.left, pad.top, 0.f } };

        for (auto & element : elements_) {
            if (!*element) {
                continue;
            }
            element->anchor(pos, contentsize_, element->anchor());
        }
    }

    void container::size(Nz::Vector2f const & value) {
        ex::padding pad{ padding() };
        contentsize_ = value - Nz::Vector2f{ pad.left + pad.right, pad.top + pad.bottom };
        collocate();
    }

    Nz::Vector2f container::size() const {
        ex::padding pad{ padding() };
        return contentsize_ + Nz::Vector2f{ pad.left + pad.right, pad.top + pad.bottom };
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
        ex::padding pad{ padding() };
        Nz::Vector2i pos{ Nz::Vector2f{ GetPosition() } + Nz::Vector2f{ pad.left, pad.top } };
        Nz::Recti rect{ (value) ? Nz::Recti{ pos.x, pos.y, static_cast<int>(contentsize_.x), static_cast<int>(contentsize_.y) } : Nz::Recti{ -1, -1 } };
        scissor(rect);
    }

    void container::scissor(Nz::Recti const & rect) {
        base_interface::scissor(rect);
        if (rect.width >= 0 && rect.height >= 0) {
            // No constraint applied to elements
            return;
        }
        for (auto & elm : elements_) {
            Nz::Recti elmscis{ elm->scissor() };
            if (elmscis.width >= 0 && elmscis.height >= 0) {
                elmscis.x = std::max(elmscis.x, rect.x);
                elmscis.y = std::max(elmscis.y, rect.y);
                elmscis.width = std::min(elmscis.width, rect.width);
                elmscis.height = std::min(elmscis.height, rect.height);
            }
            else {
                // Element has no constraint but we still apply container's one
                elmscis = rect;
            }
            elm->scissor(elmscis);
        }
    }

}
