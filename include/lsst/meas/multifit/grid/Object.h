// -*- LSST-C++ -*-
/* 
 * LSST Data Management System
 * Copyright 2008, 2009, 2010 LSST Corporation.
 * 
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the LSST License Statement and 
 * the GNU General Public License along with this program.  If not, 
 * see <http://www.lsstcorp.org/LegalNotices/>.
 */

#ifndef LSST_MEAS_MULTIFIT_GRID_Object
#define LSST_MEAS_MULTIFIT_GRID_Object

#include "lsst/meas/multifit/definition/Object.h"
#include "lsst/meas/multifit/grid/parameters.h"
#include "lsst/meas/multifit/grid/Array.h"
#include "lsst/meas/multifit/grid/Source.h"

namespace lsst { namespace meas { namespace multifit { namespace grid {

class Object : public definition::Object {
public:

    typedef Array<Source> SourceArray;

    Object(definition::Object const & definition_, int offset, int frame_count, int filter_count);

    Object(Object const & other) :
        definition::Object(other),
        coefficient_offset(other.coefficient_offset),
        coefficient_count(other.coefficient_count),
        sources(),
        extra(0)
    {}

    int coefficient_offset;
    int coefficient_count;

    SourceArray sources;

    mutable void * extra;

    PositionComponent & getPosition() const {
        return static_cast<PositionComponent &>(*this->position);
    }

    RadiusComponent & getRadius() const {
        return static_cast<RadiusComponent &>(*this->radius);
    }

    EllipticityComponent & getEllipticity() const {
        return static_cast<EllipticityComponent &>(*this->ellipticity);
    }

    /// @brief Construct the point corresponding to this object from a parameter vector.
    agl::PointD makePoint(double const * param_iter) const;

    /**
     *  Perturb a point by changing the nth position parameter.
     *  Returns the offset of the nth position parameter and the
     *  size of the perturbation.
     */
    std::pair<int,double> perturbPoint(agl::PointD & point, int n) const;

    /**
     *  Unperturb a point by changing the nth position parameter.
     */
    void unperturbPoint(agl::PointD & point, int n, double perturbation) const {
        point[n] -= perturbation;
    }

    /// @brief Construct the ellipse corresponding to this object from a parameter vector.
    agl::Ellipse makeEllipse(double const * param_iter) const;

    /**
     *  Perturb an ellipse by changing the nth ellipse parameter.
     *  Returns the offset of the nth ellipse parameter and the
     *  size of the perturbation.
     */
    std::pair<int,double> perturbEllipse(agl::Ellipse & elllipse, int n) const;

    /**
     *  Unperturb a point by changing the nth position parameter.
     */
    void unperturbEllipse(agl::Ellipse & ellipse, int n, double perturbation) const;

};

}}}} // namespace lsst::meas::multifit::grid

#endif // !LSST_MEAS_MULTIFIT_GRID_Object