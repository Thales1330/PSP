#ifndef TRANSFERFUNCTION_H
#define TRANSFERFUNCTION_H

#include "ControlElement.h"

#include <wx/dcscreen.h>
#include "wxGLString.h"

class TransferFunctionForm;

class TransferFunction : public ControlElement
{
   public:
    struct SpaceState {
        std::vector<std::vector<double> > A;
        std::vector<double> B;
        std::vector<double> C;
        double D;
    };

    TransferFunction(int id);
    ~TransferFunction();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);

    virtual std::vector<double> GetNumerator() const { return m_numerator; }
    virtual std::vector<double> GetDenominator() const { return m_denominator; }
    virtual void SetNumerator(std::vector<double> numerator) { m_numerator = numerator; }
    virtual void SetDenominator(std::vector<double> denominator) { m_denominator = denominator; }
    virtual void UpdateTFText();
    virtual SpaceState GetSpaceState() { return m_ss; }
    virtual void CalculateSpaceState(int maxIteration = 100, double error = 1e-3);
    virtual bool Solve(double input, double timeStep);

   protected:
    virtual void SetText(wxString numerator, wxString denominator);
    virtual wxString GetSuperscriptNumber(int number);
    virtual void GetTFString(wxString& numerator, wxString& denominator);

    wchar_t m_supNumber[10];

    wxGLString* m_glStringNum = NULL;
    wxGLString* m_glStringDen = NULL;
    int m_fontSize = 10;

    std::vector<double> m_numerator;
    std::vector<double> m_denominator;
    SpaceState m_ss;

    std::vector<double> m_x;
    std::vector<double> m_dx;
    double m_error = 1e-3;
    int m_maxIteration = 100;
};

#endif  // TRANSFERFUNCTION_H
