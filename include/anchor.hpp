#ifndef HPP_ANCHOR_INCLUDED
#define HPP_ANCHOR_INCLUDED

    #include <Nazara/Math/Vector2.hpp>

    namespace ex {

        struct anchor
        {
            Nz::Vector2f min, max;

            anchor() = default;
            anchor(Nz::Vector2f const & normMin, Nz::Vector2f const & normMax)
                : min(normMin)
                , max(normMax) {
            }

            using align_t = Nz::Vector2f;

            static align_t begin() { return { 0.f, 0.f }; }
            static align_t end() { return { 1.f, 1.f }; }
            static align_t center() { return { 0.5f, 0.5f }; }
            static align_t stretch() { return { 0.f, 1.f }; }

            static anchor ease(align_t const & horizontal = begin(), align_t const & vertical = begin()) {
                return { { horizontal.x, vertical.x }, { horizontal.y, vertical.y } };
            }
        };

    }

#endif /* HPP_ANCHOR_INCLUDED */

