#include <vector>
#include <iostream>
#include "DataEngine.hpp"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
DataEngine::~DataEngine() {
    unordered_map<HistKey , Histogram>::iterator it;
    for(it = histograms.begin(); it != histograms.end(); it++){
        delete &(it->second);
    }
}
//---------------------------------------------------------------------------
void DataEngine::addRelation(RelationId relId, const char* fileName)
// Loads a relation from disk
{
   this->relations.emplace_back(relId, fileName);
}
//---------------------------------------------------------------------------
void DataEngine::buildCompleteHist(RelationId rid, int sampleRatio, int numOfBuckets) {
    clock_t startTime = clock();

    Relation& r = this->relations[rid];
    for(unsigned colID = 0; colID < r.columns.size(); colID++){
        Histogram& h = *new Histogram(r, colID, r.size / sampleRatio);
        //h.createEquiWidth(numOfBuckets);
        //h.createExactEquiWidth(numOfBuckets);
        h.createEquiHeight(numOfBuckets);
        this->histograms.insert(pair<HistKey, Histogram> (pair<RelationId, unsigned>(rid, colID), h));
    }

    cout << "Time taken: " << ((double)(clock() - startTime)/CLOCKS_PER_SEC) << endl;
}
//---------------------------------------------------------------------------
Relation& DataEngine::getRelation(unsigned relationId)
// Loads a relation from disk
{
   if (relationId >= this->relations.size()) {
      cerr << "Relation with id: " << relationId << " does not exist" << endl;
      throw;
   }
   return this->relations[relationId];
}
//---------------------------------------------------------------------------
//uint64_t DataEngine::getEstimatedSelectivity(AbstractOperatorNode &op, DataNode &d){
//    FilterOperatorNode* filterOp;
//    JoinOperatorNode* joinOp;
//    if ((filterOp = dynamic_cast<FilterOperatorNode*>(&op)) != NULL){
//        #ifndef NDEBUG
//        cout << "Estimate for Filter operator" << endl;
//        #endif
//        return getFilterSelectivity(filterOp, d);
//    }else if ((joinOp = dynamic_cast<JoinOperatorNode*>(&op)) != NULL){
//        #ifndef NDEBUG
//        cout << "Estimate for Join operator" << endl;
//        #endif
//        return getJoinSelectivity(joinOp, d);
//    }else{
//        #ifndef NDEBUG
//       cout << "Uknown operator";
//        #endif
//       return 0;
//    }
//}
////---------------------------------------------------------------------------
//uint64_t DataEngine::getFilterSelectivity(FilterOperatorNode* filterOp, DataNode &d){
//    SelectInfo &si = (filterOp -> info).filterColumn;
//    Histogram& h = histograms.at(HistKey (si.relId, si.colId));
//    if ((filterOp -> info).comparison == FilterInfo::Comparison::Less){
//        return h.getEstimatedKeys(0, (filterOp-> info).constant);
//    }else if((filterOp -> info).comparison == FilterInfo::Comparison::Greater){
//        return h.getEstimatedKeys((filterOp-> info).constant, UINT64_MAX);
//    }else{
//        return h.getEstimatedKeys((filterOp-> info).constant, (filterOp-> info).constant);
//    }
//}
//--------------------------------------------------------------------------
uint64_t DataEngine::getFilterSelectivity(FilterInfo& filter){
    Histogram& h = histograms.at(HistKey (filter.filterColumn.relId, filter.filterColumn.colId));
    if (filter.comparison == FilterInfo::Comparison::Less){
        return h.getEstimatedKeys(0, filter.constant);
    }else if(filter.comparison == FilterInfo::Comparison::Greater){
        return h.getEstimatedKeys(filter.constant, UINT64_MAX);
    }else{
        return h.getEstimatedKeys(filter.constant, filter.constant);
    }
}
//--------------------------------------------------------------------------
uint64_t DataEngine::getJoinSelectivity(PredicateInfo& predicate){
    Histogram& hLeft = histograms.at(HistKey (predicate.left.relId, predicate.left.colId));
    Histogram& hRight = histograms.at(HistKey (predicate.right.relId, predicate.right.colId));
    uint64_t joinSize = 0;

    map<uint64_t,uint64_t>::iterator it;
    uint64_t prevBound = hRight.domainMinimum;
    for(it = hLeft.histo.begin(); it != hLeft.histo.end(); it++){
        joinSize += it->second * hRight.getEstimatedKeys(prevBound, it->first);
        prevBound = it->first;
    }
    return joinSize;
}
//--------------------------------------------------------------------------
//uint64_t DataEngine::getJoinSelectivity(JoinOperatorNode* joinOp, DataNode &d){
//    SelectInfo &leftRelation = (joinOp -> info).left;
//    SelectInfo &rightRelation = (joinOp -> info).right;
//    Histogram& hLeft = histograms.at(HistKey (leftRelation.relId, leftRelation.colId));
//    Histogram& hRight = histograms.at(HistKey (rightRelation.relId, rightRelation.colId));
//    uint64_t joinSize = 0;
//
//    map<uint64_t,uint64_t>::iterator it;
//    uint64_t prevBound = hRight.domainMinimum;
//    for(it = hLeft.histo.begin(); it != hLeft.histo.end(); it++){
//        joinSize += it->second * hRight.getEstimatedKeys(prevBound, it->first);
//        prevBound = it->first;
//    }
//   return joinSize;
//}

