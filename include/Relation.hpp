#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "Plan.hpp"
#include "Mixins.hpp"
//---------------------------------------------------------------------------
class Relation: public AbstractDataNode {
    private:
    /// Owns memory (false if it was mmaped)
    bool ownsMemory;
    /// Loads data from a file
    void loadRelation(const char* fileName);

    public:
    /// The relation's ID.
    const RelationId relId;
    /// The number of tuples
    uint64_t size;
    /// The join column containing the keys
    std::vector<uint64_t*> columns;
    /// The ids of the tuples.
    std::vector<uint64_t> ids;

    /// Stores a relation into a file (binary)
    void storeRelation(const std::string& fileName);
    /// Stores a relation into a file (csv)
    void storeRelationCSV(const std::string& fileName);
    /// Dump SQL: Create and load table (PostgreSQL)
    void dumpSQL(const std::string& fileName,unsigned relationId);

    /// Checks if the nodes it depends on are `processed`
    /// and if so sets its flag to processed too.
    void execute();
    /// Frees any resources allocated by the node.
    void freeResources() { }

    /// Returns an `IteratorPair` over all the `DataNode`'s ids.
    /// Ignores `filterInfo`, requires it being `NULL`.
    IteratorPair getIdsIterator(SelectInfo& selectInfo, FilterInfo* filterInfo);
    /// Returns an `IteratorPair` over all the `DataNode`'s values
    /// of the column specified by `selectInfo`.
    /// Ignores `filterInfo`, requires it being `NULL`.
    IteratorPair getValuesIterator(SelectInfo& selectInfo, FilterInfo* filterInfo);

    /// Constructor without mmap
    Relation(RelationId relId, uint64_t size, std::vector<uint64_t*>&& columns) : ownsMemory(true), relId(relId), size(size), columns(columns) {}
    /// Constructor using mmap
    Relation(RelationId relId, const char* fileName);
    /// Delete copy constructor
    Relation(const Relation& other)=delete;
    /// Move constructor
    Relation(Relation&& other)=default;
    /// The destructor
    ~Relation();
};
//---------------------------------------------------------------------------
