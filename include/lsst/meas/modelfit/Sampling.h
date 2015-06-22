// -*- lsst-c++ -*-
/*
 * LSST Data Management System
 * Copyright 2008-2013 LSST Corporation.
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

#ifndef LSST_MEAS_MODELFIT_Sampling_h_INCLUDED
#define LSST_MEAS_MODELFIT_Sampling_h_INCLUDED

#include "ndarray.h"
#include "lsst/afw/table/fwd.h"
#include "lsst/afw/table/Key.h"
#include "lsst/meas/modelfit/Mixture.h"
#include "lsst/meas/modelfit/Likelihood.h"
#include "lsst/meas/modelfit/Interpreter.h"

namespace lsst { namespace meas { namespace modelfit {

class SamplingObjective {
public:

    int getParameterDim() const { return _parameterNames.size(); }

    virtual Scalar operator()(
        ndarray::Array<Scalar const,1,1> const & parameters,
        afw::table::BaseRecord & sample
    ) const = 0;

    virtual ~SamplingObjective() {}

protected:

    SamplingObjective(
        afw::table::Schema & sampleSchema,
        Model::NameVector const & parameterNames,
        PTR(Model) model,
        PTR(Prior) prior,
        PTR(Likelihood) likelihood
    );

    Model::NameVector _parameterNames;
    PTR(Model) _model;
    PTR(Prior) _prior;
    ArrayKey _parameterKey;
    ArrayKey _nonlinearKey;
    ScalarKey _weightKey;
    PTR(Likelihood) _likelihood;
    ndarray::Array<Pixel,2,-1> _modelMatrix;
};

}}} // namespace lsst::meas::modelfit

#endif // !LSST_MEAS_MODELFIT_Sampling_h_INCLUDED