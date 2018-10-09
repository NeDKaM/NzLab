#ifndef HPP_ANCHOR_INCLUDED
#define HPP_ANCHOR_INCLUDED

    #include <Nazara/Math/Vector2.hpp>

    namespace ex {

        struct anchor
        {
            Nz::Vector2f min, max;

            using align = Nz::Vector2f;

            static align begin() { return { 0.f, 0.f }; }
            static align end() { return { 1.f, 1.f }; }
            static align center() { return { 0.5f, 0.5f }; }
            static align stretch() { return { 0.f, 1.f }; }

            static anchor ease(align const & horizontal = begin(), align const & vertical = begin()) {
                return { { horizontal.x, vertical.x }, { horizontal.y, vertical.y } };
            }
        };

    }

#endif /* HPP_ANCHOR_INCLUDED */

