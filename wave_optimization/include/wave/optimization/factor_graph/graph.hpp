#ifndef WAVE_OPTIMIZATION_FACTOR_GRAPH_GRAPH_HPP
#define WAVE_OPTIMIZATION_FACTOR_GRAPH_GRAPH_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <memory>

#include "wave/utils/utils.hpp"
#include "wave/optimization/factor_graph/factor.hpp"
#include "wave/optimization/factor_graph/variable.hpp"

namespace wave {

class FactorGraph {
 public:
    std::vector<std::shared_ptr<Factor>> factors;
    std::vector<std::shared_ptr<FactorVariable>> variables;

    FactorGraph() {}

    template <typename V>
    void add(const VariableId &v1, const MatX &z) {
        auto var = std::make_shared<V>(v1);
        auto factor = std::make_shared<UnaryFactor>(var, z);

        this->factors.push_back(factor);
        this->variables.push_back(var);
    }

    template <typename VA, typename VB>
    void add(const VariableId &v1, const VariableId &v2, const MatX &z) {
        auto var1 = std::make_shared<VA>(v1);
        auto var2 = std::make_shared<VB>(v2);
        auto factor = std::make_shared<BinaryFactor>(var1, var2, z);

        this->factors.push_back(factor);
        this->variables.push_back(var1);
        this->variables.push_back(var2);
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    const FactorGraph &graph) {
        int index = 0;

        for (auto f : graph.factors) {
            os << "f" << index << " -- ";
            os << *f << std::endl;
            index++;
        }

        return os;
    }
};

}  // end of wave namespace
#endif
