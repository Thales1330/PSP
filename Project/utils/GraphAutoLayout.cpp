#include "GraphAutoLayout.h"
//#include "ImportForm.h"

GraphAutoLayout::GraphAutoLayout() {}

GraphAutoLayout::~GraphAutoLayout() {}

GraphAutoLayout::GraphAutoLayout(std::vector<ParseMatpower::BusData*> busData,
                                 std::vector<ParseMatpower::BranchData*> branchData)
{
    m_busData = busData;
    m_branchData = branchData;
    for(auto it = m_busData.begin(); it != m_busData.end(); ++it) {
        GraphLayoutNode node;
        m_nodes.push_back(node);
    }
    for(auto it = m_branchData.begin(); it != m_branchData.end(); ++it) {
        ParseMatpower::BranchData* branch = *it;
        float weight = 1.0;
        if(branch->tap > 1e-3) weight = 2.0;
        AddLink(branch->busConnections.first - 1, branch->busConnections.second - 1, weight);
    }
}

void GraphAutoLayout::AddLink(size_t index1, size_t index2, float weight)
{
    // If the two indices are the same, or if the weight is zero, do nothing (no link)
    if(index1 == index2 || weight == 0.f) { return; }
    // If the number of nodes is lesser than one of the indices, extend the nodes vector
    size_t maxIndex = std::max(index1, index2);
    size_t nodesMaxIndex = m_nodes.size() - 1;
    for(size_t i = nodesMaxIndex; i < maxIndex; i++) {
        GraphLayoutNode node;
        m_nodes.push_back(node);
    }
    // Add an edge
    m_edges.push_back({.node1 = m_nodes[index1], .node2 = m_nodes[index2], .weight = weight});
}

void GraphAutoLayout::Compute(size_t iterations)
{
    wxProgressDialog pbd(_("Importing..."), _("Initializing..."), iterations, nullptr,
                         wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_CAN_ABORT | wxPD_SMOOTH);

    float nodesCount = m_nodes.size();

    // Initialize nodes positions on a circle
    float a = 0.f;
    float da = 2.f * M_PI / nodesCount;
    for(auto node = m_nodes.begin(); node != m_nodes.end(); node++) {
        node->position.x = nodesCount * std::cos(a);
        node->position.y = nodesCount * std::sin(a);
        a += da;
    }

    // Initial parameters; other values can be chosen for area;
    float area = nodesCount;
    float k2 = area / nodesCount;
    float k = sqrt(k2);

    for(size_t i = 0; i < iterations; i++) {
        // Temperature cools down; starts at 1, ends at 0
        // (other formulas can be used for the cooling)
        float temperature = 1.f - i / (float)iterations;
        temperature *= temperature;

        // Calculate repulsive forces
        for(auto node1 = m_nodes.begin(); node1 != m_nodes.end(); node1++) {
            node1->displacement = {0.f, 0.f};
            for(auto node2 = m_nodes.begin(); node2 != m_nodes.end(); node2++) {
                float dx = node1->position.x - node2->position.x;
                float dy = node1->position.y - node2->position.y;
                if(dx && dy) {
                    float d2 = dx * dx + dy * dy;
                    float coefficient = k2 / d2;
                    node1->displacement.x += coefficient * dx;
                    node1->displacement.y += coefficient * dy;
                }
            }
        }

        // Calculate attractive forces
        for(auto edge = m_edges.begin(); edge != m_edges.end(); edge++) {
            float dx = edge->node1.position.x - edge->node2.position.x;
            float dy = edge->node1.position.y - edge->node2.position.y;
            float d2 = dx * dx + dy * dy;
            float coefficient = sqrt(d2) / k * edge->weight;
            edge->node1.displacement.x -= dx * coefficient;
            edge->node1.displacement.y -= dy * coefficient;
            edge->node2.displacement.x += dx * coefficient;
            edge->node2.displacement.y += dy * coefficient;
        }

        //  Calculate positions
        float sum = 0.f;
        for(auto node = m_nodes.begin(); node != m_nodes.end(); node++) {
            float d2 = node->displacement.x * node->displacement.x + node->displacement.y * node->displacement.y;
            float d = sqrt(d2);
            if(d > temperature) {
                float coefficient = temperature / d;
                node->displacement.x *= coefficient;
                node->displacement.y *= coefficient;
                sum += temperature;
            } else {
                sum += d;
            }
            node->position.x += node->displacement.x;
            node->position.y += node->displacement.y;
        }

        if(!pbd.Update(i, wxString::Format("Iteration = %d", i))) {
            pbd.Update(iterations);
            i = iterations;
        }
    }
}

void GraphAutoLayout::CalculatePositions(int iterations, double scale)
{
    Compute(iterations);
    int index = 0;
    for(auto it = m_busData.begin(); it != m_busData.end(); ++it) {
        (*it)->busPosition = wxPoint2DDouble(m_nodes[index].position.x * scale, m_nodes[index].position.y * scale);
        index++;
    }
}
