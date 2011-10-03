// -*- LSST-C++ -*-
/* 
 * LSST Data Management System
 * Copyright 2008, 2009, 2010, 2011 LSST Corporation.
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

#ifndef LSST_MEAS_MULTIFIT_GRID_SourceComponent
#define LSST_MEAS_MULTIFIT_GRID_SourceComponent

#include "lsst/afw/geom/AffineTransform.h"
#include "lsst/meas/multifit/ModelBasis.h"

namespace lsst { namespace meas { namespace multifit { namespace grid {

class Frame;
class ObjectComponent;

class SourceComponent {
public:

    afw::geom::Point2D const getReferencePoint() const;

    Frame const & frame;
    ObjectComponent const & object;

    /**
     *  @brief Set the elements of an integration vector that correspond to this source.
     *
     *  By calling fillIntegration on several sources with the same array, an integration
     *  vector can be constructed that computes the sum of the fluxes of those sources.
     *
     *  The full vector should be set to zero; fillIntegration only operates on a segment.
     *  If fillIntegration is called on the same SourceComponent multiple times (or on multiple sources
     *  with the same coefficient offsets), 
     */
    void fillIntegration(lsst::ndarray::Array<Pixel,1,1> const & integration) const;

    /// @brief Compute the flux defined by a custom integration vector.
    static double computeFlux(
        lsst::ndarray::Array<Pixel const,1,1> const & integration,
        lsst::ndarray::Array<Pixel const,1,1> const & coefficients
    );

    /// @brief Compute the variance of the flux defined by a custom integration vector.
    static double computeFluxVariance(
        lsst::ndarray::Array<Pixel const,1,1> const & integration,
        lsst::ndarray::Array<Pixel const,2,1> const & covariance
    );

    /// @brief Return the flux of the object on this frame, given a coefficient vector.
    double computeFlux(lsst::ndarray::Array<Pixel const,1,1> const & coefficients) const;

    /**
     *  @brief Return the variance of the flux of the object on this frame, given a coefficient 
     *         covariance matrix.
     */
    double computeFluxVariance(lsst::ndarray::Array<Pixel const,2,1> const & covariance) const;

    int const getCoefficientOffset() const;

    int const getCoefficientCount() const;

    afw::geom::AffineTransform const & getTransform() const { return _transform; }

    ModelBasis::Ptr const getBasis() const { return _basis; }

    afw::detection::LocalPsf::Ptr const getLocalPsf() const { return _localPsf; }

private:

    friend class grid::Initializer;

    SourceComponent(
        Frame const & frame, ObjectComponent const & object, 
        CONST_PTR(afw::image::Wcs) const & wcs
    );

    afw::geom::AffineTransform _transform;
    ModelBasis::Ptr _basis;
    afw::detection::LocalPsf::Ptr _localPsf;
};

}}}} // namespace lsst::meas::multifit::grid

#endif // !LSST_MEAS_MULTIFIT_GRID_sources