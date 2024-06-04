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

#include<map>

#include "ControlElement.h"

#include "../../editors/ControlEditor.h"

ControlElementContainer::ControlElementContainer()
{
	ClearContainer();
}
ControlElementContainer::~ControlElementContainer()
{
	for (auto* cElement : m_cLineList) delete cElement;
	for (auto* cElement : m_constantList) delete cElement;
	for (auto* cElement : m_exponentialList) delete cElement;
	for (auto* cElement : m_gainList) delete cElement;
	for (auto* cElement : m_ioControlList) delete cElement;
	for (auto* cElement : m_limiterList) delete cElement;
	for (auto* cElement : m_multiplierList) delete cElement;
	for (auto* cElement : m_rateLimiterList) delete cElement;
	for (auto* cElement : m_sumList) delete cElement;
	for (auto* cElement : m_tfList) delete cElement;
	for (auto* cElement : m_dividerList) delete cElement;
	for (auto* cElement : m_mathExprList) delete cElement;
	ClearContainer();
}
void ControlElementContainer::FillContainer(ControlEditor* editor)
{
	ClearContainer();
	m_ctrlElementsList = editor->GetControlElementList();
	m_cLineList = editor->GetConnectionLineList();
	auto cElementList = editor->GetControlElementList();
	for (auto it = cElementList.begin(), itEnd = cElementList.end(); it != itEnd; ++it) {
		if (Constant* constant = dynamic_cast<Constant*>(*it)) {
			m_constantList.push_back(constant);
		}
		else if (Exponential* exponential = dynamic_cast<Exponential*>(*it)) {
			m_exponentialList.push_back(exponential);
		}
		else if (Gain* gain = dynamic_cast<Gain*>(*it)) {
			m_gainList.push_back(gain);
		}
		else if (IOControl* ioControl = dynamic_cast<IOControl*>(*it)) {
			m_ioControlList.push_back(ioControl);
		}
		else if (Limiter* limiter = dynamic_cast<Limiter*>(*it)) {
			m_limiterList.push_back(limiter);
		}
		else if (Multiplier* multiplier = dynamic_cast<Multiplier*>(*it)) {
			m_multiplierList.push_back(multiplier);
		}
		else if (RateLimiter* rateLimiter = dynamic_cast<RateLimiter*>(*it)) {
			m_rateLimiterList.push_back(rateLimiter);
		}
		else if (Sum* sum = dynamic_cast<Sum*>(*it)) {
			m_sumList.push_back(sum);
		}
		else if (TransferFunction* tf = dynamic_cast<TransferFunction*>(*it)) {
			m_tfList.push_back(tf);
		}
		else if (Divider* divider = dynamic_cast<Divider*>(*it)) {
			m_dividerList.push_back(divider);
		}
		else if (MathExpression* mathExpr = dynamic_cast<MathExpression*>(*it)) {
			m_mathExprList.push_back(mathExpr);
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
	m_dividerList.clear();
	m_mathExprList.clear();
}

void ControlElementContainer::FillContainer(std::vector<ControlElement*> controlElementList,
	std::vector<ConnectionLine*> connectionLineList)
{
	ClearContainer();
	m_ctrlElementsList = controlElementList;
	m_cLineList = connectionLineList;

	for (auto it = controlElementList.begin(), itEnd = controlElementList.end(); it != itEnd; ++it) {
		if (Constant* constant = dynamic_cast<Constant*>(*it)) {
			m_constantList.push_back(constant);
		}
		else if (Exponential* exponential = dynamic_cast<Exponential*>(*it)) {
			m_exponentialList.push_back(exponential);
		}
		else if (Gain* gain = dynamic_cast<Gain*>(*it)) {
			m_gainList.push_back(gain);
		}
		else if (IOControl* ioControl = dynamic_cast<IOControl*>(*it)) {
			m_ioControlList.push_back(ioControl);
		}
		else if (Limiter* limiter = dynamic_cast<Limiter*>(*it)) {
			m_limiterList.push_back(limiter);
		}
		else if (Multiplier* multiplier = dynamic_cast<Multiplier*>(*it)) {
			m_multiplierList.push_back(multiplier);
		}
		else if (RateLimiter* rateLimiter = dynamic_cast<RateLimiter*>(*it)) {
			m_rateLimiterList.push_back(rateLimiter);
		}
		else if (Sum* sum = dynamic_cast<Sum*>(*it)) {
			m_sumList.push_back(sum);
		}
		else if (TransferFunction* tf = dynamic_cast<TransferFunction*>(*it)) {
			m_tfList.push_back(tf);
		}
		else if (Divider* divider = dynamic_cast<Divider*>(*it)) {
			m_dividerList.push_back(divider);
		}
		else if (MathExpression* mathExpr = dynamic_cast<MathExpression*>(*it)) {
			m_mathExprList.push_back(mathExpr);
		}
	}
}

void ControlElementContainer::GetContainerCopy(std::vector<ControlElement*>& controlElementList,
	std::vector<ConnectionLine*>& connectionLineList)
{
	controlElementList.clear();
	connectionLineList.clear();

	std::map<Element*, Element*> cLineMap;
	std::map<Element*, Element*> cElementMap;

	// Copy connection lines
	for (ControlElement* cLine : m_cLineList) {
		ConnectionLine* copyLine = static_cast<ConnectionLine*>(cLine->GetCopy());
		connectionLineList.push_back(copyLine);
		cLineMap[cLine] = copyLine;
	}

	// Copy elements (exept connection line).
	std::map<Node*, Node*> nodeMap;
	for (ControlElement* cElement : m_ctrlElementsList) {
		ControlElement* copyElement = static_cast<ControlElement*>(cElement->GetCopy());
		cElementMap[cElement] = copyElement;
		controlElementList.push_back(copyElement);
		// Copy nodes.
		std::vector<Node*> nodeListCopy;
		for (Node* node : copyElement->GetNodeList()) {
			Node* copyNode = node->GetCopy();
			nodeMap[node] = copyNode;
			nodeListCopy.push_back(copyNode);
		}
		copyElement->SetNodeList(nodeListCopy);
	}

	// Correct the parent and child pointers
	for (ControlElement*& copyElement : controlElementList) {
		// Child
		for (Element* child : copyElement->GetChildList()) {
			copyElement->ReplaceChild(child, cLineMap[child]);
		}
	}
	for (ConnectionLine* copyLine : connectionLineList) {
		// Nodes
		for (Node* node : copyLine->GetNodeList()) {
			copyLine->ReplaceNode(node, nodeMap[node]);
		}

		// Parent element
		for (Element* parent : copyLine->GetParentList()) {
			copyLine->ReplaceParent(parent, cElementMap[parent]);
		}
		// Parent cLine
		Element* parentLine = static_cast<Element*>(copyLine->GetParentLine());
		copyLine->SetParentLine(static_cast<ConnectionLine*>(cLineMap[parentLine]));

		// Childs
		for (Element* child : copyLine->GetChildList()) {
			copyLine->ReplaceChild(child, cLineMap[child]);
		}
	}

	//int nodeID = 0;
	//for (auto it = m_ctrlElementsList.begin(), itEnd = m_ctrlElementsList.end(); it != itEnd; ++it) {
	//	ControlElement* oldElement = *it;
	//	ControlElement* copy = static_cast<ControlElement*>(oldElement->GetCopy());
	//	controlElementList.push_back(copy);
	//	// Copy nodes.
	//	std::vector<Node*> nodeList = copy->GetNodeList();
	//	std::vector<Node*> nodeListCopy;
	//	for (auto itN = nodeList.begin(), itEndN = nodeList.end(); itN != itEndN; ++itN) {
	//		Node* node = *itN;
	//		node->SetID(nodeID);
	//		Node* copyNode = new Node();
	//		*copyNode = *node;
	//		nodeListCopy.push_back(copyNode);
	//		nodeID++;
	//	}
	//	copy->SetNodeList(nodeListCopy);
	//
	//	// Replace children to copies.
	//	auto childList = copy->GetChildList();
	//	for (auto itC = childList.begin(), itEndC = childList.end(); itC != itEndC; ++itC) {
	//		ConnectionLine* child = static_cast<ConnectionLine*>(*itC);
	//		// Replace child's parent to copy.
	//		for (auto itCL = connectionLineList.begin(), itEndCL = connectionLineList.end(); itCL != itEndCL; ++itCL) {
	//			ConnectionLine* copyLine = *itCL;
	//			if (copyLine->GetID() == child->GetID()) {
	//				// Replace node.
	//				nodeList = child->GetNodeList();
	//				for (auto itN = nodeList.begin(), itEndN = nodeList.end(); itN != itEndN; ++itN) {
	//					Node* node = *itN;
	//					for (auto itCN = nodeListCopy.begin(), itEndCN = nodeListCopy.end(); itCN != itEndCN; ++itCN) {
	//						Node* nodeCopy = *itCN;
	//						if (node->GetID() == nodeCopy->GetID()) {
	//							copyLine->ReplaceNode(node, nodeCopy);
	//							break;
	//						}
	//					}
	//				}
	//				copyLine->ReplaceParent(oldElement, copy);
	//				copy->ReplaceChild(child, copyLine);
	//			}
	//		}
	//	}
	//}
}
