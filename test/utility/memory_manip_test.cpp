#include <siplasplas/utility/memory_manip.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace ::cpp::detail;

TEST(MemoryManipTest, checkAlingment_alignedData)
{
    std::aligned_storage_t<1, alignof(int)> data;
    EXPECT_TRUE(is_aligned(&data, alignof(int)));
    EXPECT_EQ(0, missalignment(&data, alignof(int)));
}

TEST(MemoryManipTest, checkAlingment_unalignedData)
{
    std::aligned_storage_t<1, alignof(int)> data;
    EXPECT_FALSE(is_aligned(reinterpret_cast<char*>(&data) + 1, alignof(int)));
    EXPECT_EQ(alignof(int) - 1, missalignment(reinterpret_cast<char*>(&data) + 1, alignof(int)));
}

TEST(MemoryManipTest, alignedPtr_returnsAlignedMemory)
{
    char buffer[256];

    EXPECT_TRUE(is_aligned(aligned_ptr(&buffer, alignof(int)), alignof(int)));
    EXPECT_TRUE(is_aligned(aligned_ptr(&buffer, alignof(long)), alignof(long)));
    EXPECT_TRUE(is_aligned(aligned_ptr(&buffer, alignof(long long)), alignof(long long)));
}

#if UINTPTR_MAX == UINT64_MAX
TEST(MemoryManipTest, taggedPtr_tagUntagAreSimmetric)
{
    char lvalue;
    EXPECT_EQ(&lvalue, untagPointer(tagPointer(&lvalue, 0x00)));
    EXPECT_EQ(&lvalue, untagPointer(tagPointer(&lvalue, 0x01)));
    EXPECT_EQ(&lvalue, untagPointer(tagPointer(&lvalue, 0x03)));
    EXPECT_EQ(&lvalue, untagPointer(tagPointer(&lvalue, 0x07)));
    EXPECT_EQ(&lvalue, untagPointer(tagPointer(&lvalue, 0x0F)));
    EXPECT_EQ(&lvalue, untagPointer(tagPointer(&lvalue, 0x1F)));
    EXPECT_EQ(&lvalue, untagPointer(tagPointer(&lvalue, 0x3F)));
    EXPECT_EQ(&lvalue, untagPointer(tagPointer(&lvalue, 0x7F)));
    EXPECT_EQ(&lvalue, untagPointer(tagPointer(&lvalue, 0xFF)));
}

TEST(MemoryManipTest, taggedPtr_readTaggedPointer_readsRightData)
{
    char lvalue;
    EXPECT_EQ(0x00, readTaggedPointer(tagPointer(&lvalue, 0x00)));
    EXPECT_EQ(0x01, readTaggedPointer(tagPointer(&lvalue, 0x01)));
    EXPECT_EQ(0x03, readTaggedPointer(tagPointer(&lvalue, 0x03)));
    EXPECT_EQ(0x07, readTaggedPointer(tagPointer(&lvalue, 0x07)));
    EXPECT_EQ(0x0F, readTaggedPointer(tagPointer(&lvalue, 0x0F)));
    EXPECT_EQ(0x1F, readTaggedPointer(tagPointer(&lvalue, 0x1F)));
    EXPECT_EQ(0x3F, readTaggedPointer(tagPointer(&lvalue, 0x3F)));
    EXPECT_EQ(0x7F, readTaggedPointer(tagPointer(&lvalue, 0x7F)));
    EXPECT_EQ(0xFF, readTaggedPointer(tagPointer(&lvalue, 0xFF)));
}
#endif

TEST(MemoryManipTest, alignedMalloc_returnsAlignedMemory)
{
    // Note the test leaks memory, but this is needed to not compromise the
    // reults of the test with the (yet untested) behavior of aligned_free())

    EXPECT_TRUE(is_aligned(aligned_malloc(1, alignof(int)), alignof(int)));
    EXPECT_TRUE(is_aligned(aligned_malloc(1, alignof(long)), alignof(long)));
    EXPECT_TRUE(is_aligned(aligned_malloc(1, alignof(long long)), alignof(long long)));

    EXPECT_TRUE(is_aligned(aligned_malloc(1, alignof(int),       16), alignof(int)));
    EXPECT_TRUE(is_aligned(aligned_malloc(1, alignof(long),      32), alignof(long)));
    EXPECT_TRUE(is_aligned(aligned_malloc(1, alignof(long long), 64), alignof(long long)));
    EXPECT_TRUE(is_aligned(aligned_malloc(1, alignof(long long), 128), alignof(long long)));
    EXPECT_TRUE(is_aligned(aligned_malloc(1, alignof(long long), 256), alignof(long long)));
}

TEST(MemoryManipTest, alignedMalloc_allocatesSupportedAlignmentAndFreeWorks)
{
    void* ptr = nullptr;
    EXPECT_NE(nullptr, ptr = aligned_malloc(1, 16));
    aligned_free(ptr);
    EXPECT_NE(nullptr, ptr = aligned_malloc(1, 32));
    aligned_free(ptr);
    EXPECT_NE(nullptr, ptr = aligned_malloc(1, 64));
    aligned_free(ptr);
    EXPECT_NE(nullptr, ptr = aligned_malloc(1, 128));
    aligned_free(ptr);
    EXPECT_NE(nullptr, ptr = aligned_malloc(1, 256));
    aligned_free(ptr);
    EXPECT_NE(nullptr, ptr = aligned_malloc(1, 16,  10));
    aligned_free(ptr, 10);
    EXPECT_NE(nullptr, ptr = aligned_malloc(1, 32,  100));
    aligned_free(ptr, 100);
    EXPECT_NE(nullptr, ptr = aligned_malloc(1, 64,  1000));
    aligned_free(ptr, 1000);
    EXPECT_NE(nullptr, ptr = aligned_malloc(1, 128, 10000));
    aligned_free(ptr, 10000);
    EXPECT_NE(nullptr, ptr = aligned_malloc(1, 256, 100000));
    aligned_free(ptr, 100000);

    // Up to 256 byte alignment is supported by default,
    // the following calls should fail with the default
    // config:

    EXPECT_EQ(nullptr, ptr = aligned_malloc(1, 512));
    EXPECT_EQ(nullptr, ptr = aligned_malloc(1, 1024));
    EXPECT_EQ(nullptr, ptr = aligned_malloc(1, 2048));
    EXPECT_EQ(nullptr, ptr = aligned_malloc(1, 4096));
}
