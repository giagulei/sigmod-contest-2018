#pragma once
#include <vector>
#include <cstdint>
#include <optional>
//---------------------------------------------------------------------------
struct FilterInfo;
struct SelectInfo;
//---------------------------------------------------------------------------
#ifdef NDEBUG
    #define DEBUG(x)
    #define DEBUGLN(x)
#else
    #define DEBUG(x) do { std::cerr << x ; } while (0)
    #define DEBUGLN(x) do { std::cerr << x << std::endl; } while (0)
#endif
//---------------------------------------------------------------------------
using RelationId = unsigned;
//---------------------------------------------------------------------------
using uint64Pair = std::pair<uint64_t, uint64_t>;
//---------------------------------------------------------------------------
using unsignedPair = std::pair<unsigned, unsigned>;
//---------------------------------------------------------------------------
using IteratorPair = std::pair<std::vector<uint64_t>::iterator, std::vector<uint64_t>::iterator>;
//---------------------------------------------------------------------------
class DataReaderMixin {
    // TODO: Would be nice not to pass `filterInfo` and then ignore it...

    public:
    /// Should be implemented by any class intended as a data storer
    /// and return the rowIds that satisfy the `filterInfo` condition.
    ///
    /// In our case `DataNode`, `Relation` and `Index`.
    /// In the first two cases `filterInfo` is ignored, in the
    /// later the `index` should use `filterInfo` to narrow down the
    /// return range of ids.
    virtual IteratorPair getIdsIterator(SelectInfo& selectInfo, FilterInfo* filterInfo) = 0;

    /// Should be implemented by any class intended as a data storer
    /// and return the values of the column specified by `selectInfo
    /// that satisfy the `filterInfo` condition.
    ///
    /// In our case `DataNode`, `Relation` and `Index`.
    /// In the first two cases `filterInfo` is ignored, in the
    /// later the `index` should use `filterInfo` to narrow down the
    /// return range of values.
    virtual std::optional<IteratorPair> getValuesIterator(SelectInfo& selectInfo,
                                                          FilterInfo* filterInfo) = 0;
};
//---------------------------------------------------------------------------
template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{
    template<typename S, typename T>
    struct hash<pair<S, T>>
    {
        inline size_t operator()(const pair<S, T>& v) const
        {
            size_t seed = 0;
            ::hash_combine(seed, v.first);
            ::hash_combine(seed, v.second);

            return seed;
        }
    };
}
//---------------------------------------------------------------------------