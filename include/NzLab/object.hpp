#ifndef HPP_EX_OBJECT_INCLUDED
#define HPP_EX_OBJECT_INCLUDED
    
    #include <vector>

    namespace ex {

        template <typename Base> class object;

        template <typename Base>
        class base_handle
        {
            friend object<Base>;

            protected:
                Base * object_ { nullptr };

            public:
                base_handle(base_handle const & handle);
                base_handle(base_handle && handle);
                explicit base_handle(object<Base> * object = nullptr);
                base_handle(std::initializer_list<Base *> object);

                virtual ~base_handle();

                base_handle & operator =(base_handle const & handle);
                base_handle & operator =(base_handle && handle);

                void reset(base_handle const & handle);
                void reset(base_handle && handle);
                void reset(object<Base> * object = nullptr);

                base_handle & swap(base_handle & handle);

                bool valid() const;
                explicit operator bool() const;

                Base * get() const;

                Base * operator ->() const;

                Base * operator *() const;

            private:
                void on_object_moved(Base * newObj);
                void on_object_destroyed();
        };

        template <typename T>
        class handle
            : public base_handle<std::enable_if_t<std::is_base_of_v<typename T::base_type, T>, typename T::base_type>>
        {
            using base = base_handle<typename T::base_type>;

            public:
                using base::base_handle;
                using base::operator =;

                T * get() const;
                T * operator ->() const;
                T * operator *() const;

        };

        template <typename T>
        class owner
            : public handle<T>
        {
            using base = handle<T>;

            public:
                owner() = default;
                owner(owner const & owner) = delete;
                owner(owner && owner);
                explicit owner(object<typename T::base_type> * object = nullptr) {
                    reset(object);
                }
                owner(std::initializer_list<T *> object);

                ~owner();

                owner & operator =(owner && owner);
                owner & operator =(owner const & owner) = delete;

                T * release();

                void reset(owner && owner);
                void reset(object<typename T::base_type> * obj = nullptr);

            private:
                void destroy();
        };

        template <typename Base>
        class object
        {
            friend base_handle<Base>;

            std::vector<base_handle<Base> *> handles_;

            public:
                using base_type = Base;

                object() = default;
                object(object const &);
                object(object && object);

                virtual ~object();

                object & operator =(object const &);
                object & operator =(object && object);

            private:
                void register_handle(base_handle<base_type> * handle);
                void unregister_handle(base_handle<base_type> * handle);
                void unregister_handles();
        };

        template <typename T, typename... Args>
            owner<T> make_owner(Args&&... args);

    }

    namespace std {

        template<typename Base>
        void swap(ex::base_handle<Base> & lhs, ex::base_handle<Base> & rhs);

    }

    #include <NzLab/object.inl>

#endif /* HPP_EX_OBJECT_INCLUDED */
