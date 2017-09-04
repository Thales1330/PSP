#ifndef POWERELEMENT_H
#define POWERELEMENT_H

#include "Element.h"
#include "ElementPlotData.h"

/**
 * @enum ElectricalUnit
 * @brief Electrical units.
*/
enum ElectricalUnit {
    UNIT_PU = 0, /**< Per unit (p.u.) */
    UNIT_V,      /**< Volt */
    UNIT_kV,     /**< Kilovolts */
    UNIT_A,      /**< Ampere */
    UNIT_kA,     /**< Kiloampere */
    UNIT_W,      /**< Watts */
    UNIT_kW,     /**< Kilowatts */
    UNIT_MW,     /**< Megawatts */
    UNIT_VA,     /**< Volt-ampere */
    UNIT_kVA,    /**< Kilovolt-ampere */
    UNIT_MVA,    /**< Megavolt-ampere */
    UNIT_VAr,    /**< Volt-ampere reactive */
    UNIT_kVAr,   /**< Kilovolt-ampere reactive */
    UNIT_MVAr,   /**< Megavolt-ampere reactive */
    UNIT_OHM,    /**< Ohm */
    UNIT_OHM_km, /**< Ohm/km */
    UNIT_S,      /**< Siemens */
    UNIT_S_km,   /**< Siemens/km */
    UNIT_DEGREE, /**< Degree */
    UNIT_RADIAN  /**< Radian */
};

/**
 * @enum FaultData
 * @brief Information about fault (type and location).
*/
enum FaultData {
    FAULT_THREEPHASE = 0, /**< Three-phase fault */
    FAULT_2LINE,          /**< Line-to-line fault */
    FAULT_2LINE_GROUND,   /**< Double line-to-ground fault */
    FAULT_LINE_GROUND,    /**< Line-to-ground fault */
    FAULT_LINE_A,         /**< Fault on phase A or phase AB */
    FAULT_LINE_B,         /**< Fault on phase B or phase BC */
    FAULT_LINE_C          /**< Fault on phase C or phase CA */
};

/**
 * @enum SwitchingType
 * @brief Type of switching.
*/
enum SwitchingType {
    SW_INSERT = 0, /**< Insert element */
    SW_REMOVE      /**< Remove element */
};

/**
 * @enum PowerFlowDirection
 * @brief Direction of power flow arrows.
*/
enum PowerFlowDirection {
    PF_NONE = 0,     /**< No direction (no arrows printed) */
    PF_TO_BUS,       /**< Element to bus */
    PF_TO_ELEMENT,   /**< Bus to element */
    PF_BUS1_TO_BUS2, /**< First bus to secont bus (branch elements) */
    PF_BUS2_TO_BUS1  /**< Second bus to first bus (branch elements) */
};

/**
 * @class SwitchingData
 * @author Thales Lima Oliveira
 * @date 18/01/2017
 * @file PowerElement.h
 * @brief Switching data of power elements.
 */
struct SwitchingData {
    std::vector<SwitchingType> swType; /**< Type of switching */
    std::vector<double> swTime;        /**< Time of switching */
};

/**
 * @class IntegrationConstant
 * @author Thales Lima Oliveira
 * @date 24/05/2017
 * @file PowerElement.h
 * @brief Integration constants to calculate dynamic elements through trapezoidal integration method
 */
struct IntegrationConstant {
    double c; /**< C value */
    double m; /**< M value */
};

/**
 * @class PowerElement
 * @author Thales Lima Oliveira
 * @date 18/01/2017
 * @file PowerElement.h
 * @brief Base class of power elements.
 */
class PowerElement : public Element
{
   public:
    /**
     * @brief Constructor
     */
    PowerElement();
    /**
     * @brief Destructor.
     */
    ~PowerElement();

    /**
     * @brief Get the correct switch position.
     * @param parent Bus with switch.
     * @param parentPoint Position of node on parent.
     * @param secondPoint Next point in element.
     */
    virtual wxPoint2DDouble GetSwitchPoint(Element* parent,
                                           wxPoint2DDouble parentPoint,
                                           wxPoint2DDouble secondPoint) const;

    /**
     * @brief Check if switch contains position.
     * @param position position to be checked.
     */
    virtual bool SwitchesContains(wxPoint2DDouble position) const;

    /**
     * @brief Update the switch position.
     */
    virtual void UpdateSwitches();

    /**
     * @brief Draw switch.
     */
    virtual void DrawSwitches() const;

    /**
     * @brief Calculate the points of the power flow arrows.
     * @param edges Points of the element that arrows point.
     */
    virtual void CalculatePowerFlowPts(std::vector<wxPoint2DDouble> edges);

    /**
     * @brief Draw power flow arrows.
     */
    virtual void DrawPowerFlowPts() const;

    /**
     * @brief Set nominal voltage of the element.
     * @param nominalVoltage Value of the nominal voltage.
     * @param nominalVoltageUnit Unit of the nominal voltage.
     */
    virtual void SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit);

    /**
     * @brief Set the switching data of the element.
     * @param data Switching data.
     */
    virtual void SetSwitchingData(SwitchingData data) { m_swData = data; }
    /**
     * @brief Returns the switching data of the element.
     * @return Element switching data.
     */
    virtual SwitchingData GetSwitchingData() { return m_swData; }
    /**
     * @brief Set the direction of the power flow.
     * @param pfDirection Power flow direction.
     */
    virtual void SetPowerFlowDirection(PowerFlowDirection pfDirection) { m_pfDirection = pfDirection; }
    /**
     * @brief Return the direction of the power flow.
     * @return Power flow direction.
     */
    virtual PowerFlowDirection GetPowerFlowDirection() const { return m_pfDirection; }
    /**
     * @brief Fill the plot data.
     * @param plotData Plot data to be filled.
     * @return true if the plot data was successfully filled, false otherwise.
     */
    virtual bool GetPlotData(ElementPlotData& plotData) { return false; }
    /**
     * @brief Check if the power element have dynamic event.
     * @return true if the element have dynamic an event, false otherwise.
     */
    virtual bool HaveDynamicEvent() const { return m_dynEvent; }
    /**
     * @brief Set if the power element have dynamic event.
     * @param dynEvent Event occurrence.
     */
    virtual void SetDynamicEvent(bool dynEvent = true) { m_dynEvent = dynEvent; }
   protected:
    SwitchingData m_swData;
    std::vector<std::vector<wxPoint2DDouble> > m_powerFlowArrow;
    PowerFlowDirection m_pfDirection = PF_NONE;

    OpenGLColour m_busColour;
    OpenGLColour m_onlineElementColour;
    OpenGLColour m_offlineElementColour;
    OpenGLColour m_closedSwitchColour;
    OpenGLColour m_openedSwitchColour;
    OpenGLColour m_powerFlowArrowColour;
    OpenGLColour m_dynamicEventColour;

    bool m_dynEvent = false;
};

#endif  // POWERELEMENT_H
