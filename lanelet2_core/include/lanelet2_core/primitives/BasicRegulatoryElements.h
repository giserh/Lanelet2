#pragma once

#include <string>
#include "../Forward.h"
#include "LineString.h"
#include "RegulatoryElement.h"

namespace lanelet {

/**
 * @brief Represents a traffic light restriction on the lanelet
 * @ingroup RegulatoryElementPrimitives
 * @ingroup Primitives
 */
class TrafficLight : public RegulatoryElement {
 public:
  using Ptr = std::shared_ptr<TrafficLight>;
  static constexpr char RuleName[] = "traffic_light";
  //! Directly construct a stop line from its required rule parameters.
  //! Might modify the input data in oder to get correct tags.
  static Ptr make(Id id, const AttributeMap& attributes, const LineStrings3d& trafficLights,
                  const LineString3d& stopLine) {
    return Ptr{new TrafficLight(id, attributes, trafficLights, stopLine)};
  }

  /**
   * @brief get the stop line for the traffic light
   * @return the stop line as LineString
   */
  ConstLineString3d stopLine() const;
  LineString3d stopLine();

  /**
   * @brief get the relevant traffic lights
   * @return the traffic lights
   *
   * There might be multiple traffic lights but they are required to show the
   * same signal.
   */
  ConstLineStrings3d trafficLights() const;
  LineStrings3d trafficLights();

  /**
   * @brief add a new traffic light
   * @param primitive the traffic light to add
   *
   * Traffic lights are represented as linestrings that start at the left edge
   * of a traffic light and end at the right edge.
   */
  void addTrafficLight(const LineString3d& primitive);

  /**
   * @brief remove a traffic light
   * @param primitive the primitive
   * @return true if the traffic light existed and was removed
   */
  bool removeTrafficLight(const LineString3d& primitive);

  /**
   * @brief set a new stop line, overwrite the old one
   * @param stopLine new stop line
   */
  void setStopLine(const LineString3d& stopLine);

 protected:
  friend class RegisterRegulatoryElement<TrafficLight>;
  TrafficLight(Id id, const AttributeMap& attributes, const LineStrings3d& trafficLights, const LineString3d& stopLine);
  explicit TrafficLight(const RegulatoryElementDataPtr& data);
};

//! Enum to distinguish maneuver types
enum class ManeuverType {
  Yield,       //!> Lanelet has right of way
  RightOfWay,  //!< Lanelet has to yield
  Unknown      //!< Lanelet ist not part of relation
};

//! @brief Defines right of way restrictions
//! @ingroup RegulatoryElementPrimitives
//! @ingroup Primitives
class RightOfWay : public RegulatoryElement {
 public:
  using Ptr = std::shared_ptr<RightOfWay>;
  static constexpr char RuleName[] = "right_of_way";

  /**
   * @brief Create a valid Right of Way object
   * @param id id for this rule
   * @param attributes for this rule. Might be extended if necessary
   * @param rightOfWay the lanelets that have right of way
   * @param yield the lanelets that have to yield
   * @param stopLine line where to stop. If there is none, stop at the end of
   * the lanelet.
   */
  static Ptr make(Id id, const AttributeMap& attributes, const Lanelets& rightOfWay, const Lanelets& yield,
                  const Optional<LineString3d>& stopLine = {}) {
    return Ptr{new RightOfWay(id, attributes, rightOfWay, yield, stopLine)};
  }

  //! returns whether a lanelet has to yield or has right of way
  ManeuverType getManeuver(const ConstLanelet& lanelet) const;

  //! get the lanelets have right of way
  ConstLanelets rightOfWayLanelets() const;
  Lanelets rightOfWayLanelets();

  //! get the lanelets that have to yield
  ConstLanelets yieldLanelets() const;
  Lanelets yieldLanelets();

  //! the stop line to stop at
  Optional<ConstLineString3d> stopLine() const;
  Optional<LineString3d> stopLine();

  //! Overwrites the stop line
  void setStopLine(const LineString3d& stopLine);

  //! Adds a lanelet for RightOfWay
  void addRightOfWayLanelet(const Lanelet& lanelet);

  //! Add yielding lanelet
  void addYieldLanelet(const Lanelet& lanelet);

  //! Removes a right of way lanelet and returns true on success
  bool removeRightOfWayLanelet(const Lanelet& lanelet);

  //! Removes a yielding lanelet and returns true no success
  bool removeYieldLanelet(const Lanelet& lanelet);

  //! Removes the stop line
  void removeStopLine();

 protected:
  friend class RegisterRegulatoryElement<RightOfWay>;
  RightOfWay(Id id, const AttributeMap& attributes, const Lanelets& rightOfWay, const Lanelets& yield,
             const Optional<LineString3d>& stopLine = {});
  explicit RightOfWay(const RegulatoryElementDataPtr& data);
};

//! Used as input argument to create TrafficSign regElem
struct TrafficSignsWithType {
  LineStrings3d trafficSigns;  //!< Lists relevant traffic signs
  std::string type{""};        //! Lists their type. If empty, it is assumed that this
                               //! is found in the attributes of trafficSigns.
                               //! the format is <country-code><ID>, eg de205.
};

//! @brief Expresses a generic traffic sign rule
//! @ingroup RegulatoryElementPrimitives
//! @ingroup Primitives
class TrafficSign : public RegulatoryElement {
 public:
  using Ptr = std::shared_ptr<TrafficSign>;
  static constexpr char RuleName[] = "traffic_sign";

  /**
   * @brief Create a valid TrafficSign object
   * @param id id of traffic sign rule
   * @param attributes attributes for it (might be extended if necessary)
   * @param trafficSigns list of the traffic signs defining the rule
   * @param cancellingTrafficSigns list of traffic signs where the rule is
   * cancelled. Can be empty.
   * @param refLines lines from where the rule becomes valid. Can be empty.
   * @param cancelLines lines after which a rule becomes invalid. Can be empty.
   */
  static Ptr make(Id id, const AttributeMap& attributes, const TrafficSignsWithType& trafficSigns,
                  const TrafficSignsWithType& cancellingTrafficSigns = {}, const LineStrings3d& refLines = {},
                  const LineStrings3d& cancelLines = {}) {
    return Ptr(new TrafficSign(id, attributes, trafficSigns, cancellingTrafficSigns, refLines, cancelLines));
  }

  //! returns the traffic signs
  /** There might be multiple but they are all required to show the same symbol.
   */
  ConstLineStrings3d trafficSigns() const;
  LineStrings3d trafficSigns();

  //! get the id/number of the sign(s)
  /** The id is in the format [country-code][ID], e.g. de205.
   * The result can be dependant on country
   */
  std::string type() const;

  //! gets the line(s) from which a sign becomes valid.
  /**
   * There might or might not be such a line. If there is none, the sign is
   * valid for the whole lanelet
   */
  ConstLineStrings3d refLines() const;
  LineStrings3d refLines();

  //! get list of cancellingTrafficSigns, if existing
  ConstLineStrings3d cancellingTrafficSigns() const;
  LineStrings3d cancellingTrafficSigns();

  //! Type of the cancelling traffic signs
  std::string cancelType() const;

  //! gets the line(s) from which a sign becomes invalid.
  ConstLineStrings3d cancelLines() const;
  LineStrings3d cancelLines();

  //! Adds another traffic sign
  /** Traffic signs are represented as linestrings that start at the left edge
   * and end at the right edge of a traffic sign.
   */
  void addTrafficSign(const LineString3d& sign);

  //! remove a traffic sign and returns true on success
  bool removeTrafficSign(const LineString3d& sign);

  //! Add new cancelling traffic sign
  void addCancellingTrafficSign(const LineString3d& sign);

  //! remove a cancelling traffic sign, returns true on success
  bool removeCancellingTrafficSign(const LineString3d& sign);

  //! Add a new reference line
  void addRefLine(const LineString3d& line);

  //! Remove a reference line. Returns true on success.
  bool removeRefLine(const LineString3d& line);

  //! Add a new line from where the sign becomes inactive
  void addCancellingRefLine(const LineString3d& line);

  //! Remove a cancelling line. Returns true on success.
  bool removeCancellingRefLine(const LineString3d& line);

 protected:
  TrafficSign(Id id, const AttributeMap& attributes, const TrafficSignsWithType& trafficSigns,
              const TrafficSignsWithType& cancellingTrafficSigns = {}, const LineStrings3d& refLines = {},
              const LineStrings3d& cancelLines = {});

  friend class RegisterRegulatoryElement<TrafficSign>;
  explicit TrafficSign(const RegulatoryElementDataPtr& data);
};

/**
 * @brief Represents a speed limit that affects a lanelet
 * @ingroup RegulatoryElementPrimitives
 * @ingroup Primitives
 *
 * A speed limit is defined by one ore more traffic signs and cancelled by one
 * or more traffic signs. All lanelets affected by this refer to this traffic
 * sign.
 *
 * As an alternative, the type can also be specified using the sign_type tag of
 * the regulatory element. However this is not recommended, because will make it
 * hard to track where the speed limit originates.
 */
class SpeedLimit : public TrafficSign {
 public:
  using Ptr = std::shared_ptr<SpeedLimit>;
  static constexpr char RuleName[] = "speed_limit";

  //! Create a speed limit regulatory element. Similar to a traffic sign.
  static Ptr make(Id id, const AttributeMap& attributes, const TrafficSignsWithType& trafficSigns,
                  const TrafficSignsWithType& cancellingTrafficSigns = {}, const LineStrings3d& refLines = {},
                  const LineStrings3d& cancelLines = {}) {
    return Ptr(new SpeedLimit(id, attributes, trafficSigns, cancellingTrafficSigns, refLines, cancelLines));
  }

 protected:
  friend class RegisterRegulatoryElement<SpeedLimit>;
  SpeedLimit(Id id, const AttributeMap& attributes, const TrafficSignsWithType& trafficSigns,
             const TrafficSignsWithType& cancellingTrafficSigns = {}, const LineStrings3d& refLines = {},
             const LineStrings3d& cancelLines = {});
  explicit SpeedLimit(const RegulatoryElementDataPtr& data);
};
}  // namespace lanelet
