#include <iostream>
#include "Planner.hpp"
#include "Executor.hpp"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
int main(void)
{
    DataEngine engine;

    // Read join relations
    string line;
    unsigned relId = 0;
    while (getline(cin, line) && line != "Done") {
        engine.addRelation(relId, line.c_str());
        engine.buildCompleteHist(relId, 100, 10);
        ++relId;
    }


    // Preparation phase (not timed)
    // Build histograms, indices,...

    // ------------ test histograms ---------------------------

//    Histogram& h = *new Histogram(engine.relations[0], 6, 4000/100);
//    delete &h;

//    Histogram& h = engine.histograms.at(HistKey (13,6));
//    cout << "Result: " << h.getEstimatedKeys(7254, 8120) << endl;
//    cout << "Result: " << h.getEstimatedKeys(4000, 10000) << endl;

//    return 0;

    // ------------ test histograms ---------------------------

    // Do index crazy.

    // The test harness will send the first query after 1 second.

    vector<QueryInfo> queries;
    vector<ResultInfo> resultsInfo;
    while (getline(cin, line) && line != "F") {
        // Load next query batch.
        do {
            queries.emplace_back().parseQuery(line);
        } while (getline(cin, line) && line != "F");

        // Reserve memory for results if necessary.
        resultsInfo.reserve(queries.size());
        // Generate an execution plan for all `queries`.
        Plan *plan = Planner::generatePlan(engine, queries);
        // Execute the generated plan.
        Executor::executePlan(*plan, resultsInfo);

        // Print results.
        ResultInfo::printResults(resultsInfo);

        // Clear info before loading next query batch.
        resultsInfo.clear();
        queries.clear();

        delete plan;
    }

    return 0;
}
//---------------------------------------------------------------------------
