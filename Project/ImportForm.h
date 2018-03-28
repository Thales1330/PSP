#ifndef IMPORTFORM_H
#define IMPORTFORM_H

#include "base/PropertiesFormBase.h"

#include <wx/geometry.h>
#include <wx/msgdlg.h>
#include <wx/textfile.h>
#include <bitset>

class Workspace;

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

    ParseAnarede(wxFileName lstFile, wxFileName pwfFile);
    ~ParseAnarede() {}
    bool Parse();
    std::vector<Component> GetComponents() const { return m_components; }
    std::vector<PowerLine> GetLines() const { return m_lines; }

   protected:
    bool GetLenghtAndRotationFromBusCode(wxString code, double& lenght, int& rotationID);
    wxString GetLSTLineNextValue(wxString line, int& currentPos);

    wxFileName m_lstFile;
    wxFileName m_pwfFile;

    std::vector<Component> m_components;
    std::vector<PowerLine> m_lines;
    
};

#endif  // IMPORTFORM_H
