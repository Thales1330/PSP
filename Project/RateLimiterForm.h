#ifndef RATELIMITERFORM_H
#define RATELIMITERFORM_H
#include "ElementForm.h"

class RateLimiter;

class RateLimiterForm : public RateLimiterFormBase
{
   public:
    RateLimiterForm(wxWindow* parent, RateLimiter* rateLimiter);
    virtual ~RateLimiterForm();
    bool ValidateData();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) {EndModal(wxID_OK);}
    virtual void OnOKButtonClick(wxCommandEvent& event);
    
    wxWindow* m_parent = NULL;
    RateLimiter* m_rateLimiter = NULL;
};
#endif  // RATELIMITERFORM_H
