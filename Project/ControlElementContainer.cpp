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

#include "ControlElementContainer.h"
#include "ControlEditor.h"
#include "ControlElement.h"

ControlElementContainer::ControlElementContainer() { ClearContainer(); }
ControlElementContainer::~ControlElementContainer() {}
void ControlElementContainer::FillContainer(ControlEditor* editor)
{
    ClearContainer();
    m_ctrlElementsList = editor->GetControlElementList();
    m_cLineList = editor->GetConnectionLineList();
    auto cElementList = editor->GetControlElementList();
    for(auto it = cElementList.begin(), itEnd = cElementList.end(); it != itEnd; ++it) {
        if(Constant* constant = dynamic_cast<Constant*>(*it)) {
            m_constantList.push_back(constant);
        } else if(Exponential* exponential = dynamic_cast<Exponential*>(*it)) {
            m_exponentialList.push_back(exponential);
        } else if(Gain* gain = dynamic_cast<Gain*>(*it)) {
            m_gainList.push_back(gain);
        } else if(IOControl* ioControl = dynamic_cast<IOControl*>(*it)) {
            m_ioControlList.push_back(ioControl);
        } else if(Limiter* limiter = dynamic_cast<Limiter*>(*it)) {
            m_limiterList.push_back(limiter);
        } else if(Multiplier* multiplier = dynamic_cast<Multiplier*>(*it)) {
            m_multiplierList.push_back(multiplier);
        } else if(RateLimiter* rateLimiter = dynamic_cast<RateLimiter*>(*it)) {
            m_rateLimiterList.push_back(rateLimiter);
        } else if(Sum* sum = dynamic_cast<Sum*>(*it)) {
            m_sumList.push_back(sum);
        } else if(TransferFunction* tf = dynamic_cast<TransferFunction*>(*it)) {
            m_tfList.push_back(tf);
        } else if(Divider* divider = dynamic_cast<Divider*>(*it)) {
            m_dividerList.push_back(divider);
        }
    }
}

void ControlElementContainer::ClearContainer()
{
    m_cLineList.clear();
    m_constantList.clear();
    m_exponentialList.clear();
    m_gainList.clear();
    m_ioControlList.clear();
    m_limiterList.clear();
    m_multiplierList.clear();
    m_rateLimiterList.clear();
    m_sumList.clear();
    m_tfList.clear();
}

void ControlElementContainer::FillContainer(std::vector<ControlElement*> controlElementList,
                                            std::vector<ConnectionLine*> connectionLineList)
{
    ClearContainer();
    m_ctrlElementsList = controlElementList;
    m_cLineList = connectionLineList;

    for(auto it = controlElementList.begin(), itEnd = controlElementList.end(); it != itEnd; ++it) {
        if(Constant* constant = dynamic_cast<Constant*>(*it)) {
            m_constantList.push_back(constant);
        } else if(Exponential* exponential = dynamic_cast<Exponential*>(*it)) {
            m_exponentialList.push_back(exponential);
        } else if(Gain* gain = dynamic_cast<Gain*>(*it)) {
            m_gainList.push_back(gain);
        } else if(IOControl* ioControl = dynamic_cast<IOControl*>(*it)) {
            m_ioControlList.push_back(ioControl);
        } else if(Limiter* limiter = dynamic_cast<Limiter*>(*it)) {
            m_limiterList.push_back(limiter);
        } else if(Multiplier* multiplier = dynamic_cast<Multiplier*>(*it)) {
            m_multiplierList.push_back(multiplier);
        } else if(RateLimiter* rateLimiter = dynamic_cast<RateLimiter*>(*it)) {
            m_rateLimiterList.push_back(rateLimiter);
        } else if(Sum* sum = dynamic_cast<Sum*>(*it)) {
            m_sumList.push_back(sum);
        } else if(TransferFunction* tf = dynamic_cast<TransferFunction*>(*it)) {
            m_tfList.push_back(tf);
        } else if(Divider* divider = dynamic_cast<Divider*>(*it)) {
            m_dividerList.push_back(divider);
        }
    }
}

void ControlElementContainer::GetContainerCopy(std::vector<ControlElement*>& controlElementList,
                                               std::vector<ConnectionLine*>& connectionLineList)
{
    controlElementList.clear();
    connectionLineList.clear();

    // Copy connection lines
    for(auto it = m_cLineList.begin(), itEnd = m_cLineList.end(); it != itEnd; ++it) {
        ConnectionLine* copy = static_cast<ConnectionLine*>((*it)->GetCopy());
        connectionLineList.push_back(copy);
    }

    // Copy elements (exept connection line).
    int nodeID = 0;
    for(auto it = m_ctrlElementsList.begin(), itEnd = m_ctrlElementsList.end(); it != itEnd; ++it) {
        Element* oldElement = *it;
        ControlElement* copy = static_cast<ControlElement*>(oldElement->GetCopy());
        controlElementList.push_back(copy);
        // Copy nodes.
        std::vector<Node*> nodeList = copy->GetNodeList();
        std::vector<Node*> nodeListCopy;
        for(auto itN = nodeList.begin(), itEndN = nodeList.end(); itN != itEndN; ++itN) {
            Node* node = *itN;
            node->SetID(nodeID);
            Node* copyNode = new Node();
            *copyNode = *node;
            nodeListCopy.push_back(copyNode);
            nodeID++;
        }
        copy->SetNodeList(nodeListCopy);

        // Replace children to copies.
        auto childList = copy->GetChildList();
        for(auto itC = childList.begin(), itEndC = childList.end(); itC != itEndC; ++itC) {
            ConnectionLine* child = static_cast<ConnectionLine*>(*itC);
            // Replace child's parent to copy.
            for(auto itCL = connectionLineList.begin(), itEndCL = connectionLineList.end(); itCL != itEndCL; ++itCL) {
                ConnectionLine* copyLine = *itCL;
                if(copyLine->GetID() == child->GetID()) {
                    // Replace node.
                    nodeList = child->GetNodeList();
                    for(auto itN = nodeList.begin(), itEndN = nodeList.end(); itN != itEndN; ++itN) {
                        Node* node = *itN;
                        for(auto itCN = nodeListCopy.begin(), itEndCN = nodeListCopy.end(); itCN != itEndCN; ++itCN) {
                            Node* nodeCopy = *itCN;
                            if(node->GetID() == nodeCopy->GetID()) {
                                copyLine->ReplaceNode(node, nodeCopy);
                                break;
                            }
                        }
                    }
                    copyLine->ReplaceParent(oldElement, copy);
                    copy->ReplaceChild(child, copyLine);
                }
            }
        }
    }
}
