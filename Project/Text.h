#ifndef TEXT_H
#define TEXT_H

#include "Element.h"
#include "wxGLString.h"

class TextForm;

enum ElementType {
    TYPE_BUS,
    TYPE_CAPACITOR,
    TYPE_IND_MOTOR,
    TYPE_INDUCTOR,
    TYPE_LINE,
    TYPE_LOAD,
    TYPE_SYNC_GENERATOR,
    TYPE_SYNC_MOTOR,
    TYPE_TRANSFORMER
};

enum DataType {
    DATA_NAME,
    DATA_VOLTAGE,
    DATA_ANGLE,
    DATA_SC_CURRENT,
    DATA_SC_VOLTAGE,
    DATA_SC_POWER,
    DATA_ACTIVE_POWER,
    DATA_REACTIVE_POWER,
    DATA_PF_ACTIVE,
    DATA_PF_REACTIVE,
    DATA_PF_LOSSES,
    DATA_PF_CURRENT
};

class Text : public Element
{
   public:
    Text();
    Text(wxPoint2DDouble position);
    ~Text();

    virtual bool AddParent(Element* parent, wxPoint2DDouble position) { return true; };
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual void Draw(wxPoint2DDouble translation, double scale);
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void Rotate(bool clockwise = true);
    virtual bool ShowForm(wxWindow* parent, std::vector<Element*> elementList);
    virtual wxString GetText() const { return m_text; }
    virtual void SetText(wxString text);

    void SetDataType(const DataType& dataType) { m_dataType = dataType; }
    void SetDirection(int direction) { m_direction = direction; }
    void SetElement(Element* element) { m_element = element; }
    void SetElementNumber(int elementNumber) { m_elementNumber = elementNumber; }
    void SetElementType(const ElementType elementType) { m_elementType = elementType; }
    void SetFontSize(int fontSize) { m_fontSize = fontSize; }
    void SetUnit(const ElectricalUnit unit) { m_unit = unit; }
    const DataType GetDataType() const { return m_dataType; }
    int GetDirection() const { return m_direction; }
    Element* GetElement() { return m_element; }
    int GetElementNumber() const { return m_elementNumber; }
    const ElementType GetElementType() const { return m_elementType; }
    int GetFontSize() const { return m_fontSize; }
    const ElectricalUnit GetUnit() const { return m_unit; }
    
   protected:
    wxString m_text = _("Text");
    int m_fontSize = 10;

    Element* m_element;
    ElementType m_elementType;
    int m_elementNumber;
    DataType m_dataType;
    ElectricalUnit m_unit;
    int m_direction;
};

#endif  // TEXT_H
