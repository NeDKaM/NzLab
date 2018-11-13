namespace ex {
        
    template <typename ElementType, typename Events>
    template <typename... EventsArgs>
    container<ElementType, Events>::container(EventsArgs &&... args) 
        : base_interface<Events>{ std::forward<EventsArgs>(args)... } {

    }

    template <typename ElementType, typename Events>
    template <typename Interface>
    handle<Interface> container<ElementType, Events>::insert(owner<Interface> && owner) {
        static_assert(std::is_base_of_v<ElementType, Interface>, "Element type does not match");
        return insert(owner.release());
    }
        
    template <typename ElementType, typename Events>
    template <typename Interface, typename... Args>
    handle<Interface> container<ElementType, Events>::insert(Args &&... args) {
        static_assert(std::is_base_of_v<ElementType, Interface>, "Element type does not match");
        return insert(new Interface(std::forward<Args>(args)...));
    }
      
    template <typename ElementType, typename Events>
    template <typename Interface>
    owner<Interface> container<ElementType, Events>::release(Interface & element) {
        static_assert(std::is_base_of_v<ElementType, Interface>, "Element type does not match");
        if (auto it = std::find_if(elements_.begin(), elements_.end(), [&element](owner_type const & elm) { return &element == *elm; })
        ; it != elements_.end()) {
            element.SetParent(nullptr);
            owner<Interface> owner{ it->release() };
            elements_.erase(it);
            return owner;
        }
        NazaraError("Element is not part of container");
        return {};
    }
    
    template <typename ElementType, typename Events>
    void container<ElementType, Events>::size(Nz::Vector2f const & value) {
        ex::padding pad{ padding() };
        contentsize_ = value - Nz::Vector2f{ pad.left + pad.right, pad.top + pad.bottom };
    }

    template <typename ElementType, typename Events>
    Nz::Vector2f container<ElementType, Events>::size() const {
        ex::padding pad{ padding() };
        return contentsize_ + Nz::Vector2f{ pad.left + pad.right, pad.top + pad.bottom };
    }
      
    template <typename ElementType, typename Events>
    std::size_t container<ElementType, Events>::count() const {
        return elements_.size();
    }
       
    template <typename ElementType, typename Events>
    template <typename Interface>
    handle<Interface> container<ElementType, Events>::insert(Interface * element) {
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
    
    template <typename ElementType, typename Events>
    void container<ElementType, Events>::show(bool value) {
        for_each([value](ElementType * elm) {
            elm->show(value);
        });
    }
    
    template <typename ElementType, typename Events>
    void container<ElementType, Events>::scissor(bool value) {
        ex::padding pad{ padding() };
        Nz::Vector2i pos{ Nz::Vector2f{ GetPosition() } + Nz::Vector2f{ pad.left, pad.top } };
        Nz::Recti rect{ (value) ? Nz::Recti{ pos.x, pos.y, static_cast<int>(contentsize_.x), static_cast<int>(contentsize_.y) } : Nz::Recti{ -1, -1 } };
        scissor(rect);
    }
    
    template <typename ElementType, typename Events>
    void container<ElementType, Events>::scissor(Nz::Recti const & rect) {
        base_interface<Events>::scissor(rect);
        if (rect.width < 0 && rect.height < 0) {
            // No constraint applied to elements
            return;
        }
        for_each([this, &rect](ElementType * elm) {
            Nz::Recti elmscis{ static_cast<ex::base_interface<typename ElementType::events_type> * const>(elm)->scissor() };
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
        });
    }
    
    template <typename ElementType, typename Events>
    template <typename Functor>
    void container<ElementType, Events>::for_each(Functor f) {
        elements_.erase(std::remove_if(elements_.begin(), elements_.end(), [&](owner_type const & elm) {
            if (elm.valid()) {
                f(*elm);
                return false;
            }
            return true;
        }), elements_.end());
    }
    
    template <typename ElementType, typename Events>
    template <typename Functor>
    void container<ElementType, Events>::for_each(Functor f) const {
        elements_.erase(std::remove_if(elements_.begin(), elements_.end(), [&](owner_type const & elm) {
            if (elm.valid()) {
                f(static_cast<ElementType const *>(*elm));
                return false;
            }
            return true;
        }), elements_.end());
    }

}
