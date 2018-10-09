#ifndef HPP_EVENT_INCLUDED
#define HPP_EVENT_INCLUDED
    
    #include <functional>

    #include <Nazara/Platform/EventHandler.hpp>
    #include <Nazara/Core/Signal.hpp>

    #include <base_event.hpp>

    #include <object.hpp>

    namespace ex {

        //struct event_concept
        //{
        //    class object_type;
        //    class event_type;

        //    static Nz::Signal<Nz::EventHandler const *, event_type const &> & signal(Nz::EventHandler const &);
        //    static bool trigger(object_type const &, event_type const &);
        //};

        template <typename Event>
        class event
            : public base_event
        {
            Nz::EventHandlerHandle eventhandle_;
            ex::handle<typename Event::object_type> object_;
            typename Nz::Signal<Nz::EventHandler const *, typename Event::event_type const &>::ConnectionGuard slot_;

            public:
                event() = delete;
                event(ex::handle<typename Event::object_type> object, Nz::EventHandlerHandle handle);
                event(event const &) = delete;
                event(event &&) = default;
                ~event() = default;

                void activation(std::function<void()> const & clbk) override;
                bool activation() const override;
        };

    }

    #include "event.inl"

#endif /* HPP_EVENT_INCLUDED */