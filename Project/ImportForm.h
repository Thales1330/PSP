/*
 *  Copyright (C) 2018  Thales Lima Oliveira <thales@ufu.br>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef IMPORTFORM_H
#define IMPORTFORM_H

#include "PropertiesFormBase.h"

#include <wx/geometry.h>
#include <wx/msgdlg.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <bitset>
#include <complex>

class Workspace;
class Bus;
class SyncGenerator;
class SyncMotor;
class Load;
class Capacitor;
class Inductor;
class Capacitor;
class IndMotor;
class Transformer;
class Line;
class PropertiesData;

class GraphAutoLayout;

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
    bool ImportCEPELFiles();
    bool ImportMatpowerFiles();
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
        int id = 0;                 /**< Bus electrical ID */
        bool isOnline = true;       /**< Element is online */
        int type = 0;               /**< Bus Type: 0 = PQ; 1 = PV; 2 = Ref.; 3 = PQ with voltage between */
        wxString voltageBase = "0"; /**< Voltage base identifier */
        wxString busName = "Bus";   /**< Bus name */
        double voltage = 1.0;       /**< Bus abs voltage (controlled value for PV and Ref. types) */
        double angle = 0.0;         /**< Angle of voltage */
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
        ElementTypeAnarede type = ANA_LINE;                        /**< Element type */
        std::pair<int, int> busConnections = std::make_pair(0, 0); /**< Branch connection IDs */
        bool isOnline = true;                                      /**< Element is online */
        double resistance = 0.0;                                   /**< Branch resistance */
        double indReactance = 0.0;                                 /**< Branch inductive reactance */
        double capSusceptance = 0.0;                               /**< Branch capacitive susceptance */
        double tap = 0.0;                                          /**< Transformer tap */
        double phaseShift = 0.0;                                   /**< Transformer phase shift */
    };
    struct IndElementData {
        int id = 0;                                              /**< Group electrical ID */
        int busConnection = 0;                                   /**< Branch connection ID */
        ElementTypeAnarede type = ANA_IND_GENERATOR;             /**< Element type */
        bool isOnline = true;                                    /**< Element is online */
        std::complex<double> power = std::complex<double>(0, 0); /**< Element power */
        int numUnits = 0;                                        /**< Number of unities */
    };
    struct IndGenData : IndElementData {
        double minReactivePower = -9999.0; /**< Minimal reactive power */
        double maxReactivePower = 99999.0; /**< Maximum reactive power */
        double xt = 1.0;                   /**< Transformer reactance of each generator */
        double xd = 1.0;                   /**< Synchronous direct-axis reactance of each generator */
        double xq = 1.0;                   /**< Synchronous quadrature-axis reactance of each generator */
        double xl = 1.0;                   /**< Leakage reactance of each generator */
        double ratedPower = 100.0;         /**< Rated power of each generator */
    };

    ParseAnarede(wxFileName lstFile, wxFileName pwfFile);
    ~ParseAnarede() { ClearData(); }

    bool Parse();

    std::vector<Component*> GetComponents() const { return m_components; }
    std::vector<PowerLine*> GetLines() const { return m_lines; }
    std::vector<BranchData*> GetBranchData() const { return m_branchData; }
    std::vector<BusData*> GetBusData() const { return m_busData; }
    std::vector<IndElementData*> GetIndElementData() const { return m_indElementData; }
    wxString GetProjectName() const { return m_projectName; }
    double GetMVAPowerBase() const { return m_mvaBase; }

    wxPoint2DDouble GetNodePositionFromID(Bus* bus, double scale, int nodeID);
    BusData* GetBusDataFromID(int id);
    BranchData* GetBranchDataFromID(int id, int fromBus, int toBus, ElementTypeAnarede type);
    IndElementData* GetIndElementDataFromID(int id, int busID, ElementTypeAnarede type);

    void ClearData();

   protected:
    bool GetLenghtAndRotationFromBusCode(wxString code, double& lenght, int& rotationID);
    wxString GetLSTLineNextValue(wxString line, int& currentPos);
    bool StrToElementType(wxString strType, ElementTypeAnarede& type);
    bool ParsePWFExeCode(wxString data, wxString exeCode);
    bool GetPWFStructuredData(wxString data,
                              unsigned int startPos,
                              unsigned int dataLenght,
                              int& value,
                              int decimalPos = -1);
    bool GetPWFStructuredData(wxString data,
                              unsigned int startPos,
                              unsigned int dataLenght,
                              double& value,
                              int decimalPos = -1);

    wxFileName m_lstFile;
    wxFileName m_pwfFile;

    std::vector<Component*> m_components;
    std::vector<PowerLine*> m_lines;
    std::vector<BusData*> m_busData;
    std::vector<BranchData*> m_branchData;
    std::vector<IndElementData*> m_indElementData;

    wxString m_projectName = _("Imported project");
    double m_mvaBase = 100.0;
};

class ParseMatpower
{
   public:
    struct BusData {
        int id = 0;      /**< Bus electrical ID */
        int type = 0;    /**< Bus Type: 1 = PQ; 2 = PV; 3 = Ref.; 4 = isolated */
        double pd = 0.0; /**< Real power demand (MW) */
        double qd = 0.0; /**< Reactive power demand (MVAr) */
        double gs = 0.0; /**< Shunt condutance (MW, V = 1.0 p.u.) */
        double bs = 0.0; /**< Shunt susceptance (MVAr, V = 1.0 p.u.). Positive for capacitor, negative for inductor? */
        int area = 0;    /**< Bus area */
        double voltage = 1.0;     /**< Bus abs voltage (controlled value for PV and Ref. types) */
        double angle = 0.0;       /**< Angle of voltage */
        double baseVoltage = 138; /**< Base voltage (kV)*/
        wxString busName = "Bus"; /**< Bus name */
        wxPoint2DDouble busPosition = wxPoint2DDouble(0,0);  /**< Bus position */
    };
    struct GenData {
        int busID = 0;                     /**< Bus electrical ID */
        double pg = 0.0;                   /**< Real power output (MW) */
        double qg = 0.0;                   /**< Reactive power output (MVAr) */
        double maxReactivePower = 99999.0; /**< Maximum reactive power (MVAr) */
        double minReactivePower = -9999.0; /**< Minimal reactive power (MVAr) */
        double baseMVA = 100;              /**< Generator power base (MVA)*/
        bool isOnline = true; /**< Machine status (> 0 = machine in-service; <= 0 = machine out-of-service) */
    };
    struct BranchData {
        std::pair<int, int> busConnections = std::make_pair(0, 0); /**< Branch connection IDs */
        double resistance = 0.0;                                   /**< Branch resistance */
        double indReactance = 0.0;                                 /**< Branch inductive reactance */
        double capSusceptance = 0.0;                               /**< Branch capacitive susceptance */
        double tap = 0.0;        /**< Transformer tap. If equal zero the branch is a line element */
        double phaseShift = 0.0; /**< Transformer phase shift. Positive represents delay */
        bool isOnline = true;    /**< Element is online */
    };

    ParseMatpower(wxFileName mFile);
    ~ParseMatpower() { ClearData(); }

    void ClearData();

    bool Parse();
    
    std::vector<BranchData*> GetBranchData() const { return m_branchData; }
    std::vector<BusData*> GetBusData() const { return m_busData; }
    std::vector<GenData*> GetGenData() const { return m_genData; }
    double GetMVAPowerBase() const { return m_mvaBase; }
    
    BusData* GetBusDataFromID(int id);
    Bus* GetBusFromID(int id, std::vector<Bus*> busList);

   protected:
    wxStringTokenizer GetMFileTokenData(wxTextFile& mFile, wxString currentLine);
    wxFileName m_mFile;

    std::vector<BusData*> m_busData;
    std::vector<BranchData*> m_branchData;
    std::vector<GenData*> m_genData;

    wxString m_projectName = _("Imported project");
    double m_mvaBase = 100.0;
};

#endif  // IMPORTFORM_H
