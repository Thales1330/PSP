// Code based in: "Weighted graphs: generate a layout in C++"
// https://rodic.fr/blog/c-weighted-graph-layout-fruchterman-reingold/

#ifndef GRAPHAUTOLAYOUT_H
#define GRAPHAUTOLAYOUT_H

#include <cmath>
//#include <vector>
#include <wx/gdicmn.h>
#include <wx/progdlg.h>

// class ParseMatpower;
#include "../forms/ImportForm.h"

class GraphAutoLayout
{
   public:
    struct GraphLayoutNode {
        wxRealPoint position;
        wxRealPoint displacement;
    };

    struct GraphLayoutEdge {
        GraphLayoutNode &node1;
        GraphLayoutNode &node2;
        float weight;
    };

    GraphAutoLayout();
    GraphAutoLayout(std::vector<ParseMatpower::BusData*> busData, std::vector<ParseMatpower::BranchData*> branchData);
    ~GraphAutoLayout();
    
    void CalculatePositions(int iterations, double scale);

   protected:
    void AddLink(size_t index1, size_t index2, float weight = 1.f);
    void Compute(size_t iterations);

    std::vector<GraphLayoutNode> m_nodes;
    std::vector<GraphLayoutEdge> m_edges;

    std::vector<ParseMatpower::BusData*> m_busData;
    std::vector<ParseMatpower::BranchData*> m_branchData;
};

#endif  // GRAPHAUTOLAYOUT_H
