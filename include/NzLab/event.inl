namespace ex {

    template <typename Event>
    event<Event>::event(ex::handle<typename Event::object_type> object, Nz::EventHandlerHandle handle)
        : eventhandle_(std::move(handle))
        , object_(std::move(object)) {
    }

    template <typename Event>
    void event<Event>::activation(callback_type const & clbk) {
        if (!clbk) {
            slot_.Disconnect();
            return;
        }
        slot_.Connect(Event::signal(*eventhandle_.GetObject()), [this, &clbk](Nz::EventHandler const *, typename Event::event_type const & event) {
            if (!object_.valid()) {
                return;
            }
            if (Event::trigger(*object_.get(), event)) {
                clbk(event);
            }
        });
    }

    template <typename Event>
    bool event<Event>::activation() const {
        return slot_.IsConnected();
    }

}