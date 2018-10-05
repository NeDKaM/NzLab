#ifndef HPP_BASE_EVENT_INCLUDED
#define HPP_BASE_EVENT_INCLUDED

    #include <functional>

    namespace ex {

        struct base_event
        {
            virtual ~base_event() = default;

            virtual void activation(std::function<void()> const &) = 0;
            virtual bool activation() const = 0;
        };

    }

#endif /* HPP_BASE_EVENT_INCLUDED */
