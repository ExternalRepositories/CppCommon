//
// Created by Ivan Shynkarenka on 27.07.2016
//

#include "catch.hpp"

#include "memory/allocator.h"
#include "memory/allocator_arena.h"
#include "memory/allocator_null.h"
#include "memory/allocator_stack.h"

#include <list>
#include <map>
#include <vector>
#include <unordered_map>

using namespace CppCommon;

TEST_CASE("Default allocator", "[CppCommon][Memory]")
{
    DefaultMemoryManager manger;
    DefaultAllocator<uint8_t> alloc(manger);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    uint8_t* ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    alloc.deallocate(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    ptr = alloc.allocate(10);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 10);
    REQUIRE(manger.allocations() == 1);
    alloc.deallocate(ptr, 10);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
}

TEST_CASE("Null allocator", "[CppCommon][Memory]")
{
    NullMemoryManager manger;
    NullAllocator<uint8_t, true> alloc(manger);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    uint8_t* ptr = alloc.allocate(1);
    REQUIRE(ptr == nullptr);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    alloc.deallocate(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    ptr = alloc.allocate(10);
    REQUIRE(ptr == nullptr);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    alloc.deallocate(ptr, 10);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
}

TEST_CASE("Stack allocator", "[CppCommon][Memory]")
{
    StackMemoryManager<11, 1> manger;
    StackAllocator<uint8_t, 11, true, 1> alloc(manger);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    uint8_t* ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);
    alloc.deallocate(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);

    ptr = alloc.allocate(10);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 10);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);
    alloc.deallocate(ptr, 10);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);

    ptr = alloc.allocate(1);
    REQUIRE(ptr == nullptr);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);

    alloc.reset();
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);
    alloc.deallocate(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);
}

TEST_CASE("Arena allocator with fixed buffer", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    uint8_t buffer[11];
    ArenaMemoryManager<DefaultMemoryManager, 1> manger(auxiliary, buffer, 11);
    ArenaAllocator<uint8_t, DefaultMemoryManager, true, 1> alloc(manger);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    uint8_t* ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);
    alloc.deallocate(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);

    ptr = alloc.allocate(10);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 10);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);
    alloc.deallocate(ptr, 10);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);

    ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);
    alloc.deallocate(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);

    alloc.reset();
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);
    alloc.deallocate(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);
}

TEST_CASE("Arena allocator with dynamic buffer", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    ArenaMemoryManager<DefaultMemoryManager, 1> manger(auxiliary, 11);
    ArenaAllocator<uint8_t, DefaultMemoryManager, true, 1> alloc(manger);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    uint8_t* ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    alloc.deallocate(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    ptr = alloc.allocate(10);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 10);
    REQUIRE(manger.allocations() == 1);
    alloc.deallocate(ptr, 10);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    alloc.deallocate(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    alloc.reset();
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    alloc.deallocate(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
}

TEST_CASE("Arena allocator with stl direct containers", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    ArenaMemoryManager<DefaultMemoryManager> manger(auxiliary, 10);
    ArenaAllocator<int, DefaultMemoryManager> alloc(manger);

    std::vector<int, decltype(alloc)> v(alloc);
    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    v.clear();

    std::list<int, decltype(alloc)> l(alloc);
    l.push_back(0);
    l.push_back(1);
    l.push_back(2);
    l.clear();
}

TEST_CASE("Arena allocator with stl associative containers", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    ArenaMemoryManager<DefaultMemoryManager> manger(auxiliary, 10);
    ArenaAllocator<std::pair<const int, int>, DefaultMemoryManager> alloc(manger);

    std::map<int, int, std::less<int>, decltype(alloc)> m(alloc);
    m[0] = 0;
    m[1] = 10;
    m[2] = 20;
    m.clear();

    std::unordered_map<int, int, std::hash<int>, std::equal_to<int>, decltype(alloc)> u(alloc);
    u[0] = 0;
    u[1] = 10;
    u[2] = 20;
    u.clear();
}