#pragma once
#include <memory>
#include "Configuration.h"
#include "Projection.h"

namespace lanelet {
using ErrorMessages = std::vector<std::string>;
namespace io_handlers {
/**
 * @brief Base class for all handlers (writers and parsers)
 */
class IOHandler {  // NOLINT
 public:
  using Ptr = std::shared_ptr<IOHandler>;
  explicit IOHandler(const Projector& projector, const io::Configuration& config = io::Configuration())
      : projector_{&projector}, config_{&config} {}
  virtual ~IOHandler() = default;

  /**
   * @brief returns the extension supported by this parser
   * @return extension (including the dot)
   * Extension can be empty if extension
   */
  static constexpr const char* extension() { return ""; }

  /**
   * @brief returns the name of this handler. Must not be empty for child
   * classes
   * @return name
   */
  static constexpr const char* name() { return ""; }

  const Projector& projector() const { return *projector_; }

  const io::Configuration config() { return *config_; }

 protected:
  IOHandler() = default;  // workaround for gcc5 bug

 private:
  const Projector* projector_{};       //!< projection object for lat/lon
                                       //!< from/to x/y conversions
  const io::Configuration* config_{};  //!< config object for additional parameters.
                                       //! Parser should always use default parameters
                                       //! if parameters are missing
};
}  // namespace io_handlers
}  // namespace lanelet