#ifndef	COMMONTYPES_H
#define COMMONTYPES_H

#include <wx/colour.h>
#include <vector>

struct VoltageLevelColour {
	double voltage = 0.0;       // Voltage in kV
	wxColour colour = wxColour(0, 78, 255);
};

struct GUIColour {
	wxColour background = wxColour(250, 249, 246);
	wxColour text = wxColour(0, 0, 0);
	wxColour enabled = wxColour(0, 0, 0);
	wxColour disable = wxColour(128, 128, 128);
	wxColour selection = wxColour(0, 125, 255, 125);
	wxColour altSelection = wxColour(0, 230, 0, 125);
	wxColour bus = wxColour(0, 78, 255);
	wxColour slackBus = wxColour(0, 78, 255);
	wxColour eventElement = wxColour(255, 130, 0);
	wxColour pfArrow = wxColour(255, 130, 0);
	wxColour swClose = wxColour(0, 102, 0);
	wxColour swOpen = wxColour(255, 26, 26);
	std::vector<VoltageLevelColour> voltageLevels;
};

/**
 * @enum ElectricalUnit
 * @brief Electrical units.
 */
enum class ElectricalUnit : int {
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
	UNIT_var,    /**< Volt-ampere reactive */
	UNIT_kvar,   /**< Kilovolt-ampere reactive */
	UNIT_Mvar,   /**< Megavolt-ampere reactive */
	UNIT_OHM,    /**< Ohm */
	UNIT_OHM_km, /**< Ohm/km */
	UNIT_S,      /**< Siemens */
	UNIT_S_km,   /**< Siemens/km */
	UNIT_DEGREE, /**< Degree */
	UNIT_RADIAN,  /**< Radian */
	UNIT_NONE
};

enum PowerFlowMethod { NEWTON_RAPHSON = 0, GAUSS_SEIDEL, GAUSS_NEWTON };
enum GUITheme { THEME_LIGHT = 0, THEME_DARK };
enum class HarmLoadConnection { PARALLEL = 0, SERIES, DISCONNECTED };
enum class PlotLib { wxMATH_PLOT = 0, wxCHART_DIR = 1 };
enum class BusFreqEstimation { ANGLE_DERIVATION = 0, WASHOUT_FILTER = 1 };

#endif