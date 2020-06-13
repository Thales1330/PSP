/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
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

#ifndef RATELIMITERFORM_H
#define RATELIMITERFORM_H
#include "ElementFormBase.h"

class RateLimiter;

/**
 * @class RateLimiterForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the rate limit control data.
 * @file RateLimiterForm.h
 */
class RateLimiterForm : public RateLimiterFormBase
{
   public:
    RateLimiterForm(wxWindow* parent, RateLimiter* rateLimiter);
    virtual ~RateLimiterForm();
    bool ValidateData();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_OK); }
    virtual void OnOKButtonClick(wxCommandEvent& event);

    wxWindow* m_parent = nullptr;
    RateLimiter* m_rateLimiter = nullptr;
};
#endif  // RATELIMITERFORM_H
