#pragma once
#include <vector>
#include <unordered_map>
#include "Plan.hpp"
#include "Parser.hpp"
#include "DataEngine.hpp"
//---------------------------------------------------------------------------
class Planner {
    public:
    /// Generates a plan for a single query
    static Plan* generateSingleQueryPlan(DataEngine &engine, QueryInfo &q);
    /// Generates a plan for the `queries`.
    static Plan* generatePlan(DataEngine &engine, std::vector<QueryInfo> &queries);
    private:
    /// Adds a base-relation datanode in the graph
    static void addBaseRelationNode(RelationId& r, DataEngine& engine, Plan *splan, std::unordered_map<RelationId, vector<SelectInfo>>& columnsToPush, 
                  std::unordered_map<RelationId, AbstractNode*>& lastAttached);
    /// Adds a filter operator node in the graph
    static void addFilterNode(FilterInfo& f, Plan *splan, std::unordered_map<RelationId, vector<SelectInfo>>& columnsToPush, 
                  std::unordered_map<RelationId, AbstractNode*>& lastAttached);
    /// Adds a join operator node in the graph
    static void addJoinNode(PredicateInfo& p, Plan *splan, std::unordered_map<RelationId, vector<SelectInfo>>& columnsToPush, 
                  std::unordered_map<RelationId, AbstractNode*>& lastAttached);
    /// Adds an aggregate-selection operator node in the graph
    static void addAggrNode(QueryInfo& q, Plan *splan, std::unordered_map<RelationId, AbstractNode*>& lastAttached);
    /// Prints the graph of the plan
    static void printPlanGraph(Plan* plan);
    static bool nodeMatching(vector<AbstractNode *> v, string label);
};
//---------------------------------------------------------------------------
