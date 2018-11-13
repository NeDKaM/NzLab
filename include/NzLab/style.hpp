#ifndef HPP_EX_STYLE_INCLUDED
#define HPP_EX_STYLE_INCLUDED

    namespace ex {

        template <typename Gfx>
        struct style
        {
            struct option;

            virtual ~style() = default;

            virtual void apply(typename Gfx::object_type &) = 0;
        };

        template <typename Gfx>
        struct style<Gfx>::option 
            : public style<Gfx>
        {
            using modifier_type = std::function<void(typename Gfx::object_type &)>;

            option() = delete;
            option(option const &) = default;
            option(option &&) = default;
            option(modifier_type const &);
            option(modifier_type &&);

            template <typename Modifier>
                option(Modifier);

            option & operator =(option const &) = default;
            option & operator =(option &&) = default;

            void apply(typename Gfx::object_type &) override;

            private:
                modifier_type modifier_;
        };
    }

    #include <NzLab/style.inl>

#endif
