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

#ifndef CONTROLELEMENTCONTAINER_H
#define CONTROLELEMENTCONTAINER_H

#include <vector>

class ControlEditor;
class ControlElement;

#include "ConnectionLine.h"
#include "Constant.h"
#include "Exponential.h"
#include "Gain.h"
#include "IOControl.h"
#include "Limiter.h"
#include "Multiplier.h"
#include "RateLimiter.h"
#include "Sum.h"
#include "TransferFunction.h"
#include "Divider.h"

/**
 * @class ControlElementContainer
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Class that can contain all control elements.
 * Can identify (using RTTI) the elements from a generic list and store them separately.
 * @file ControlElementContainer.h
 */
class ControlElementContainer
{
   public:
    ControlElementContainer();
    ~ControlElementContainer();

    virtual void FillContainer(ControlEditor* editor);
    virtual void FillContainer(std::vector<ControlElement*> controlElementList,
                               std::vector<ConnectionLine*> connectionLineList);
    virtual void GetContainerCopy(std::vector<ControlElement*>& controlElementList,
                                  std::vector<ConnectionLine*>& connectionLineList);
    virtual void ClearContainer();

    std::vector<ControlElement*> GetControlElementsList() const { return m_ctrlElementsList; }
    std::vector<ConnectionLine*> GetConnectionLineList() const { return m_cLineList; }
    std::vector<Constant*> GetConstantList() const { return m_constantList; }
    std::vector<Exponential*> GetExponentialList() const { return m_exponentialList; }
    std::vector<Gain*> GetGainList() const { return m_gainList; }
    std::vector<IOControl*> GetIOControlList() const { return m_ioControlList; }
    std::vector<Limiter*> GetLimiterList() const { return m_limiterList; }
    std::vector<Multiplier*> GetMultiplierList() const { return m_multiplierList; }
    std::vector<RateLimiter*> GetRateLimiterList() const { return m_rateLimiterList; }
    std::vector<Sum*> GetSumList() const { return m_sumList; }
    std::vector<TransferFunction*> GetTFList() const { return m_tfList; }
    std::vector<Divider*> GetDividerList() const { return m_dividerList; }
   protected:
    std::vector<ControlElement*> m_ctrlElementsList;
    std::vector<Constant*> m_constantList;
    std::vector<ConnectionLine*> m_cLineList;
    std::vector<Exponential*> m_exponentialList;
    std::vector<Gain*> m_gainList;
    std::vector<IOControl*> m_ioControlList;
    std::vector<Limiter*> m_limiterList;
    std::vector<Multiplier*> m_multiplierList;
    std::vector<RateLimiter*> m_rateLimiterList;
    std::vector<Sum*> m_sumList;
    std::vector<TransferFunction*> m_tfList;
    std::vector<Divider*> m_dividerList;
};

#endif  // CONTROLELEMENTCONTAINER_H
