#include <lanelet2_io/Io.h>
#include <boost/python.hpp>

namespace py = boost::python;
using namespace lanelet;

std::shared_ptr<LaneletMap> loadWrapper(const std::string& filename, const Origin& origin) {
  return load(filename, nullptr, origin);
}
std::shared_ptr<LaneletMap> loadProjectorWrapper(const std::string& filename, const Projector& projector) {
  return load(filename, projector);
}
py::tuple loadWithErrorWrapper(const std::string& filename, const Projector& projector) {
  ErrorMessages errs;
  LaneletMapPtr map = load(filename, projector, &errs);
  return py::make_tuple(map, errs);
}
void writeWrapper(const std::string& filename, const LaneletMap& map, const Projector& projector) {
  write(filename, map, projector);
}

ErrorMessages writeWithErrorWrapper(const std::string& filename, const LaneletMap& map, const Projector& projector) {
  ErrorMessages errs;
  write(filename, map, projector, &errs);
  return errs;
}

BOOST_PYTHON_MODULE(PYTHON_API_MODULE_NAME) {  // NOLINT
  using namespace py;
  auto core = import("lanelet2.core");
  auto proj = import("lanelet2.projection");

  class_<Origin, std::shared_ptr<Origin>>("Origin", init<>()).add_property("position", &Origin::position);

  def("load", loadProjectorWrapper, (arg("filename"), arg("projector") = DefaultProjector()));
  def("load", loadWrapper, (arg("filename"), arg("origin")));
  def("loadRobust", loadWithErrorWrapper, arg("filename"),
      "Loads a map robustly. Parser errors are returned as second member of "
      "the tuple. If there are errors, the map will be incomplete somewhere.");

  def("write", writeWrapper, (arg("filename"), arg("map"), arg("projector") = DefaultProjector()),
      "Writes the map to a file. The extension determines which format will "
      "be used (usually .osm)");
  def("writeRobust", writeWithErrorWrapper, (arg("filename"), arg("map"), arg("projector") = DefaultProjector()),
      "Writes a map robustly and returns writer errors. If there are errors, "
      "the map will be incomplete somewhere.");
}
