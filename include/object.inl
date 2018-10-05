namespace ex {

    // base_handle

    template <typename Base>
    base_handle<Base>::base_handle(base_handle const & handle) {
        reset(handle);
    }

    template <typename Base>
    base_handle<Base>::base_handle(base_handle && handle) {
        reset(std::move(handle));
    }

    template <typename Base>
    base_handle<Base>::base_handle(object<Base> * object) {
        reset(object);
    }

    template <typename Base>
    base_handle<Base>::base_handle(std::initializer_list<Base *> object) {
        (object.size()) ? reset(*object.begin()) : reset();
    }

    template <typename Base>
    base_handle<Base>::~base_handle() {
        reset();
    }

    template <typename Base>
    base_handle<Base> & base_handle<Base>::operator =(base_handle const & handle) {
        reset(handle);
        return *this;
    }
    template <typename Base>
    base_handle<Base> & base_handle<Base>::operator =(base_handle && handle) {
        reset(std::move(handle));
        return *this;
    }

    template <typename Base>
    void base_handle<Base>::reset(base_handle const & handle) {
        if (object_ == handle.object_) {
            return;
        }
        reset(handle.object_);
    }
    template <typename Base>
    void base_handle<Base>::reset(base_handle && handle) {
        if (this == &handle) {
            return;
        }
        reset(handle);
        handle.reset();
    }
    template <typename Base>
    void base_handle<Base>::reset(object<Base> * object) {
        if (object_ == object) {
            return;
        }
        if (object_) {
            object_->unregister_handle(this);
        }
        object_ = static_cast<Base *>(object);
        if (object_) {
            object->register_handle(this);
        }
    }

    template <typename Base>
    base_handle<Base> & base_handle<Base>::swap(base_handle & handle) {
        if (object_) {
            object_->unregister_handle(this);
            object_->register_handle(&handle);
        }
        if (handle.object_) {
            handle.object_->unregister_handle(&handle);
            handle.object_->register_handle(this);
        }
        std::swap(object_, handle.object_);
        return *this;
    }

    template <typename Base>
    bool base_handle<Base>::valid() const {
        return object_ != nullptr;
    }

    template <typename Base>
    Base * base_handle<Base>::get() const {
        return object_;
    }

    template <typename Base>
    Base * base_handle<Base>::operator ->() const {
        return object_;
    }
    
    template <typename Base>
    Base * base_handle<Base>::operator *() const {
        return object_;
    }

    template <typename Base>
    void base_handle<Base>::on_object_moved(Base * newObj) {
        object_ = newObj;
    }

    template <typename Base>
    void base_handle<Base>::on_object_destroyed() {
        object_ = nullptr;
    }

    // handle

    template <typename T>
    T * handle<T>::get() const {
        return dynamic_cast<T *>(base::object_);
    }

    template<typename T>
    T * handle<T>::operator->() const
    {
        return dynamic_cast<T *>(base::object_);
    }

    template<typename T>
    T * handle<T>::operator*() const
    {
        return dynamic_cast<T *>(base::object_);
    }

    // owner

    template <typename T>
    owner<T>::owner(owner && owner) {
        reset(std::move(owner));
    }

    template <typename T>
    owner<T>::owner(std::initializer_list<T *> object) {
        (object.size()) ? reset(*object.begin()) : reset();
    }

    template <typename T>
    owner<T>::~owner() {
        if (base::object_) {
            destroy();
        }
    }

    template <typename T>
    owner<T> & owner<T>::operator =(owner && owner) {
        reset(std::move(owner));
        return *this;
    }

    template <typename T>
    T * owner<T>::release() {
        T * ptr = base::get();
        base::reset();
        return ptr;
    }

    template <typename T>
    void owner<T>::reset(owner && owner) {
        if (base::object_) {
            destroy();
        }
        base::reset(owner);
        if (owner.get()) {
            owner.reset();
        }
    }
    template <typename T>
    void owner<T>::reset(object<typename T::base_type> * obj) {
        if (base::object_ == obj) {
            return;
        }
        if (base::object_) {
            destroy();
        }
        base::reset(obj);
    }

    template <typename T>
    void owner<T>::destroy() {
        delete base::get();
        base::object_ = nullptr;
    }

    // object

    template <typename Base>
    object<Base>::object(object const &) { }

    template <typename Base>
    object<Base>::object(object && object)
        : handles_(std::move(object.handles_)) {
        for (auto handle : handles_) {
            handle->on_object_moved(this);
        }
    }

    template <typename Base>
    object<Base>::~object() {
        unregister_handles();
    }

    template <typename Base>
    object<Base> & object<Base>::operator =(object const &) {
        return *this;
    }
    template <typename Base>
    object<Base> & object<Base>::operator =(object && object) {
        unregister_handles();
        handles_ = std::move(object.handles_);
        for (auto handle : handles_) {
            handle->on_object_moved(this);
        }
        return *this;
    }

    template <typename Base>
    void object<Base>::register_handle(base_handle<base_type> * handle) {
        handles_.push_back(handle);
    }
    template <typename Base>
    void object<Base>::unregister_handle(base_handle<base_type> * handle) {
        auto it = std::find(handles_.begin(), handles_.end(), handle);
        NazaraAssert(it != handles_.end(), "Handle not registered");
        std::swap(*it, handles_.back());
        handles_.pop_back();
    }
    template <typename Base>
    void object<Base>::unregister_handles() {
        for (auto handle : handles_) {
            handle->on_object_destroyed();
        }
        handles_.clear();
    }

    // make_owner

    template <typename T, typename... Args>
    owner<T> make_owner(Args&&... args) {
        return owner<T>(new T(std::forward<Args>(args)...));
    }
}

namespace std
{
    template<typename Base>
    void swap(ex::base_handle<Base> & lhs, ex::base_handle<Base> & rhs)
    {
        lhs.swap(rhs);
    }
}
