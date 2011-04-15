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
 
#include <iostream>
#include <cmath>
#include <vector>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ModelProjection

#include "boost/make_shared.hpp"
#include "boost/test/unit_test.hpp"
#include "boost/test/floating_point_comparison.hpp"

#include "ndarray.hpp"
#include "lsst/afw/detection/Footprint.h"
#include "lsst/afw/detection/Psf.h"
#include "lsst/afw/image/Wcs.h"
#include "lsst/afw/image/Utils.h"

#include "lsst/meas/multifit/core.h"
#include "lsst/meas/multifit/Model.h"

#include "lsst/meas/multifit/footprintUtils.h"
#include "lsst/meas/multifit/ComponentModel.h"
#include "lsst/meas/multifit/ModelProjection.h"
#include "lsst/meas/multifit/FourierModelProjection.h"
#include "lsst/meas/multifit/ModelFactory.h"
#include "lsst/meas/multifit/components/Astrometry.h"
#include "lsst/meas/multifit/components/PointSourceMorphology.h"

using namespace std;
namespace multifit = lsst::meas::multifit;
namespace geom = lsst::afw::geom;
namespace detection = lsst::afw::detection;

BOOST_AUTO_TEST_CASE(FourierModelProjection) {

    geom::PointD centroid = geom::PointD::make(35,65);
    double flux = 34.45;
    geom::AffineTransform transform;
    multifit::Model::Ptr psModel = multifit::ModelFactory::createPointSourceModel(
        flux, centroid
    );
    
    BOOST_CHECK_EQUAL(psModel->getLinearParameterSize(), 1);
    BOOST_CHECK_EQUAL(psModel->getNonlinearParameterSize(), 2);

    CONST_PTR(detection::Psf) psf = detection::createPsf("DoubleGaussian", 23, 23, 2.0);
    CONST_PTR(detection::Footprint) fp = psModel->computeProjectionFootprint(psf, transform);
    
    BOOST_CHECK(fp->getNpix() > 0);
    multifit::ModelProjection::Ptr projection = psModel->makeProjection(psf, transform, fp);
    BOOST_CHECK_EQUAL(projection->getModel(), psModel);
   
    multifit::ParameterVector linear(psModel->getLinearParameters());
    multifit::ParameterVector nonlinear(psModel->getNonlinearParameters());

    BOOST_CHECK_EQUAL(linear[0], flux);
    BOOST_CHECK_EQUAL(nonlinear[0], centroid[0]);
    BOOST_CHECK_EQUAL(nonlinear[1], centroid[1]);

    multifit::FourierModelProjection::Ptr asFourierProjection =
        boost::static_pointer_cast<multifit::FourierModelProjection>(projection);

    BOOST_CHECK(asFourierProjection);

    ndarray::Array<multifit::Pixel const, 1, 1> image;
    BOOST_CHECK_NO_THROW(shallow(image) = projection->computeModelImage());
    BOOST_CHECK_EQUAL(image.size(), fp->getNpix());
    lsst::afw::image::BBox bbox = fp->getBBox();
    lsst::afw::image::MaskedImage<multifit::Pixel> mi(
        bbox.getWidth(), 
        bbox.getHeight() 
    );
    mi.setXY0(bbox.getLLC());

    multifit::expandImage<multifit::Pixel>(*fp, mi, image, image);
    detection::setMaskFromFootprint<lsst::afw::image::MaskPixel>(
        mi.getMask().get(), *fp, 1
    );

    mi.getImage()->writeFits("psProjection_img.fits");
    mi.getMask()->writeFits("psProjection_msk.fits");

    BOOST_CHECK_NO_THROW(projection->computeLinearParameterDerivative());
    BOOST_CHECK_NO_THROW(projection->computeNonlinearParameterDerivative());
}