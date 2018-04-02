#ifndef IMPORTFORM_H
#define IMPORTFORM_H

#include "base/PropertiesFormBase.h"

#include <wx/geometry.h>
#include <wx/msgdlg.h>
#include <wx/textfile.h>
#include <bitset>

class Workspace;
class Bus;
class SyncGenerator;
class Load;
class Capacitor;
class Inductor;
class IndMotor;
class Transformer;
class Line;

/**
 * @class ImportForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 27/03/2018
 * @brief Form to import other programs files to PSP
 * @file ImportForm.h
 */
class ImportForm : public ImportFormBase
{
   public:
    ImportForm(wxWindow* parent, Workspace* workspace);
    virtual ~ImportForm();

    Workspace* GetWorkspace() { return m_workspace; }

   protected:
    virtual void OnButtonCancelClick(wxCommandEvent& event);
    virtual void OnButtonOKClick(wxCommandEvent& event);
    bool ImportSelectedFiles();
    Bus* GetBusFromID(std::vector<Bus*> busList, int id);

    Workspace* m_workspace = NULL;
    wxWindow* m_parent;
};

/**
 * @enum ElementType
 * @brief ID of ANAREDE's elements.
 */
enum ElementTypeAnarede {
    ANA_BUS = 1,            /**< Bus */
    ANA_GENERATOR = 4,      /**< Generator */
    ANA_LOAD = 5,           /**< Load */
    ANA_SHUNT = 6,          /**< Shunt element */
    ANA_MIT = 7,            /**< Induction motor */
    ANA_TRANSFORMER = 9,    /**< Transformer */
    ANA_LINE = 14,          /**< Power line */
    ANA_IND_LOAD = 22,      /**< Independent load */
    ANA_IND_SHUNT = 23,     /**< Independent shunt element */
    ANA_IND_GENERATOR = 24, /**< Independent generator */
};

/**
 * @class ParseAnarede
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 27/03/2018
 * @brief Class responsible to parse ANAREDE files to import data to PSP.
 * @file ImportForm.h
 */
class ParseAnarede
{
   public:
    // Graphic files data structs
    struct Component {
        int id = 0;                        /**< Graphical ID */
        ElementTypeAnarede type = ANA_BUS; /**< Element type */
        double length = 0.0;               /**< Element lenght (only buses) */
        int rotationID = 0;                /**< Rotation ID (0, 1, 2 or 4) */
        wxPoint2DDouble position;          /**< X and Y coordinates */
        int electricalID = 0;              /**< Bus, Branch or Group electrical IDs */
        std::pair<int, int> busConnectionID[2] = {
            std::make_pair(0, 0), std::make_pair(0, 0)}; /**< In the form <graphicalBusID,electricBusID> */
        std::pair<int, int> busConnectionNode[2] = {std::make_pair(0, 0),
                                                    std::make_pair(0, 0)}; /**< In the form <graphicalBusID,nodeID> */
    };
    struct PowerLine {
        int id = 0;                         /**< Graphical ID */
        ElementTypeAnarede type = ANA_LINE; /**< Element type */
        int electricalID = 0;               /**< Bus, Branch or Group electrical IDs */
        std::pair<int, int> busConnectionID[2] = {
            std::make_pair(0, 0), std::make_pair(0, 0)}; /**< In the form <graphicalBusID,electricBusID> */
        std::pair<int, int> busConnectionNode[2] = {std::make_pair(0, 0),
                                                    std::make_pair(0, 0)}; /**< In the form <graphicalBusID,nodeID> */
        std::vector<wxPoint2DDouble> nodesPosition; /**< Coordinates of the line breaks, if any */
    };
    struct BusData {
        int id = 0;               /**< Bus electrical ID */
        bool isOnline = true;     /**< Element is online */
        int type = 0;             /**< Bus Type: 0 = PQ; 1 = PV; 2 = Ref.; 3 = PQ with voltage between */
        int voltageBase = 0;      /**< Voltage base ID */
        wxString busName = "Bus"; /**< Bus name */
        double voltage = 1.0;     /**< Bus abs voltage (controlled value for PV and Ref. types) */
        double angle = 0.0;       /**< Angle of voltage */
        std::complex<double> genPower = std::complex<double>(0, 0);  /**< Generated power */
        double minReactivePower = -9999.0;                           /**< Minimal reactive power */
        double maxReactivePower = 99999.0;                           /**< Maximum reactive power */
        int ctrlBusID = 0;                                           /**< Controlled bus ID */
        std::complex<double> loadPower = std::complex<double>(0, 0); /**< Load power */
        double shuntReactive =
            0.0; /**< Reactive power of shunt element. Positive for capacitor, negative for inductor. */
    };
    struct BranchData {
        int id = 0;                                                /**< Branch electrical ID */
        std::pair<int, int> busConnections = std::make_pair(0, 0); /**< Branch connection IDs */
        bool isOnline = true;                                      /**< Element is online */
        double resistance = 0.0;                                   /**< Branch resistance */
        double indReactance = 0.0;                                 /**< Branch inductive reactance */
        double capSusceptance = 0.0;                               /**< Branch capacitive susceptance */
        double tap = 1.0;                                          /**< Transformer tap */
        double phaseShift = 0.0;                                   /**< Transformer phase shift */
    };
    struct IndElementData {
        int id = 0;                                                  /**< Group electrical ID */
        ElementTypeAnarede type = ANA_IND_GENERATOR;                 /**< Element type */
        bool isOnline = true;                                        /**< Element is online */
        std::complex<double> loadPower = std::complex<double>(0, 0); /**< Element power */
        int numUnits = 0;                                            /**< Number of unities */
    };

    ParseAnarede(wxFileName lstFile, wxFileName pwfFile);
    ~ParseAnarede() {}

    bool Parse();

    std::vector<Component> GetComponents() const { return m_components; }
    std::vector<PowerLine> GetLines() const { return m_lines; }
    std::vector<BranchData> GetBranchData() const { return m_branchData; }
    std::vector<BusData> GetBusData() const { return m_busData; }
    std::vector<IndElementData> GetIndElementData() const { return m_indElementData; }
    wxString GetProjectName() const { return m_projectName; }

    wxPoint2DDouble GetNodePositionFromID(Bus* bus, double scale, int nodeID);

   protected:
    bool GetLenghtAndRotationFromBusCode(wxString code, double& lenght, int& rotationID);
    wxString GetLSTLineNextValue(wxString line, int& currentPos);
    bool StrToElementType(wxString strType, ElementTypeAnarede& type);
    bool ParsePWFExeCode(wxString data, wxString exeCode);
    bool GetPWFStructuredData(wxString data, int startPos, int dataLenght, int& value, int decimalPos = -1);
    bool GetPWFStructuredData(wxString data, int startPos, int dataLenght, double& value, int decimalPos = -1);

    wxFileName m_lstFile;
    wxFileName m_pwfFile;

    std::vector<Component> m_components;
    std::vector<PowerLine> m_lines;
    std::vector<BusData> m_busData;
    std::vector<BranchData> m_branchData;
    std::vector<IndElementData> m_indElementData;

    wxString m_projectName = _("Imported project");
    double m_mvaBase = 100.0;
};

#endif  // IMPORTFORM_H
