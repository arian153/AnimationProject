#include "Anchor.hpp"
#include "../Utility/VectorDef.hpp"

namespace CS460
{
    Anchor::Anchor()
    {
    }

    Anchor::~Anchor()
    {
    }

    eAnchor2D Anchor::ToAnchor2D(eAnchorLR left_right, eAnchorUD up_down)
    {
        return static_cast<eAnchor2D>(static_cast<size_t>(left_right) + static_cast<size_t>(up_down));
    }

    eAnchor3D Anchor::ToAnchor3D(eAnchorLR left_right, eAnchorUD up_down, eAnchorFB front_back)
    {
        return static_cast<eAnchor3D>(static_cast<size_t>(left_right) + static_cast<size_t>(up_down) + static_cast<size_t>(front_back));
    }

    Vector2 Anchor::AnchorVector2(eAnchor2D anchor)
    {
        size_t anchor_code = static_cast<size_t>(anchor);
        size_t x           = anchor_code / 100;
        size_t yz          = anchor_code % 100;
        size_t y           = yz / 10;
        return ToVector2(static_cast<eAnchorLR>(x * 100), static_cast<eAnchorUD>(y * 10));
    }

    Vector3 Anchor::AnchorVector3(eAnchor3D anchor)
    {
        size_t anchor_code = static_cast<size_t>(anchor);
        size_t x           = anchor_code / 100;
        size_t yz          = anchor_code % 100;
        size_t y           = yz / 10;
        size_t z           = yz % 10;
        return ToVector3(static_cast<eAnchorLR>(x * 100), static_cast<eAnchorUD>(y * 10), static_cast<eAnchorFB>(z));
    }

    Vector2 Anchor::ToVector2(eAnchorLR left_right, eAnchorUD up_down)
    {
        Vector2 result;
        switch (left_right)
        {
        case CS460::eAnchorLR::Left:
            result -= Math::Vector2::X_AXIS;
            break;
        case CS460::eAnchorLR::Center:
            break;
        case CS460::eAnchorLR::Right:
            result += Math::Vector2::X_AXIS;
            break;
        default:
            break;
        }
        switch (up_down)
        {
        case CS460::eAnchorUD::Up:
            result += Math::Vector2::Y_AXIS;
            break;
        case CS460::eAnchorUD::Center:
            break;
        case CS460::eAnchorUD::Down:
            result -= Math::Vector2::Y_AXIS;
            break;
        default:
            break;
        }
        return result;
    }

    Vector3 Anchor::ToVector3(eAnchorLR left_right, eAnchorUD up_down, eAnchorFB front_back)
    {
        Vector3 result;
        switch (left_right)
        {
        case CS460::eAnchorLR::Left:
            result -= Math::Vector3::X_AXIS;
            break;
        case CS460::eAnchorLR::Center:
            break;
        case CS460::eAnchorLR::Right:
            result += Math::Vector3::X_AXIS;
            break;
        default:
            break;
        }
        switch (up_down)
        {
        case CS460::eAnchorUD::Up:
            result += Math::Vector3::Y_AXIS;
            break;
        case CS460::eAnchorUD::Center:
            break;
        case CS460::eAnchorUD::Down:
            result -= Math::Vector3::Y_AXIS;
            break;
        default:
            break;
        }
        switch (front_back)
        {
        case CS460::eAnchorFB::Front:
            result += Math::Vector3::Z_AXIS;
            break;
        case CS460::eAnchorFB::Center:
            break;
        case CS460::eAnchorFB::Back:
            result -= Math::Vector3::Z_AXIS;
            break;
        default:
            break;
        }
        return result;
    }
}
