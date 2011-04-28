#include "lsst/meas/multifit/SimpleInterpreter.h"

namespace lsst { namespace meas { namespace multifit {

lsst::afw::geom::Point2D SimpleInterpreter::extractPointMu(ID id) const {
    grid::Object const & obj = grid::find(_grid->objects, id);
    return obj.makePoint(getMuCRef().data());
}

void SimpleInterpreter::insertPointMu(ID id, lsst::afw::geom::Point2D const & mu) {
    grid::Object const & obj = grid::find(_grid->objects, id);
    obj.readPoint(getMuRef().data(), mu);
}

Eigen::Matrix2d SimpleInterpreter::extractPointSigma(ID id) const {
    grid::Object const & obj = grid::find(_grid->objects, id);
    return obj.extractPointMatrix(getSigmaCRef());
}

void SimpleInterpreter::insertPointSigma(ID id, Eigen::Matrix2d const & sigma) {
    grid::Object const & obj = grid::find(_grid->objects, id);
    invalidateTarget();
    obj.insertPointMatrix(getSigmaRef(), sigma);
}

Ellipse SimpleInterpreter::extractEllipseMu(ID id) const {
    grid::Object const & obj = grid::find(_grid->objects, id);
    return obj.makeEllipse(getMuCRef().data());
}

void SimpleInterpreter::insertEllipseMu(ID id, Ellipse const & mu) {
    grid::Object const & obj = grid::find(_grid->objects, id);
    obj.readEllipse(getMuRef().data(), mu);
}

Eigen::Matrix5d SimpleInterpreter::extractEllipseSigma(ID id) const {
    grid::Object const & obj = grid::find(_grid->objects, id);
    return obj.extractEllipseMatrix(getSigmaCRef());
}

void SimpleInterpreter::insertEllipseSigma(ID id, Eigen::Matrix5d const & sigma) {
    grid::Object const & obj = grid::find(_grid->objects, id);
    invalidateTarget();
    obj.insertEllipseMatrix(getSigmaRef(), sigma);
}

void ParameterSimpleInterpreter::ensureCompatibility() {
    if (_grid->getParameterCount() != _target->getSize()) {
        throw LSST_EXCEPT(
            lsst::pex::exceptions::InvalidParameterException,
            (boost::format("Grid parameter size (%d) does not match distribution size (%d).")
             % _grid->getParameterCount() % _target->getSize()).str()
        );
    }
}

}}} // namespace lsst::meas::multifit
