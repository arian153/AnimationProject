#include "SoftContactPoint.hpp"

namespace CS460
{
    SoftContactPoint::SoftContactPoint()
    {
    }

    SoftContactPoint::~SoftContactPoint()
    {
    }

    void SoftContactPoint::Clear()
    {
    }

    SoftContactPoint& SoftContactPoint::operator=(const SoftContactPoint& rhs)
    {
        return *this;
    }

    bool SoftContactPoint::operator==(const SoftContactPoint& rhs) const
    {
        return false;
    }

    void SoftContactPoint::Swap()
    {
    }

    void SoftContactPoint::UpdateContactPoint(const SoftContactPoint& rhs)
    {
    }

    SoftContactPoint SoftContactPoint::SwappedContactPoint() const
    {
        return SoftContactPoint();
    }
}
