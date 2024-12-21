#include "pch.h"
#include <random>
extern "C"
{
#include "Algorithm.h"
#include "main.h"
#include "LinkList.h"

}

namespace FunctionTest
{
    class Allocation : public ::testing::Test
    {
    protected:
        struct rest* resthead = nullptr;
        struct job* job = nullptr;

        // Set up the test environment
        void SetUp() override
        {
            resthead = nullptr;
            job = nullptr;
        }

        // Tear down the test environment
        void TearDown() override
        {
            FreeRestLinkList(resthead);
            if (job) free(job);
        }

        // Helper function to create a rest block
        struct rest* createRestBlock(int address, int size) {
            struct rest* r = (struct rest*)malloc(sizeof(struct rest));
            if (!r) exit(1);
            r->ad = address;
            r->size = size;
            r->next = nullptr;
            return r;
        }

        // Helper function to create a job
        struct job* createJob(int address, int size, int name) {
            struct job* j = (struct job*)malloc(sizeof(struct job));
            if (!j) exit(1);
            j->ad = address;
            j->size = size;
            j->name = name;
            return j;
        }
    };

    // Test 1: Allocate a job in an empty rest list
    TEST_F(Allocation, FFAllocate_EmptyRestList)
    {
        job = createJob(0, 50, 1);
        resthead = FFAllocate(resthead, job);
        ASSERT_EQ(job->ad, 0);
        ASSERT_EQ(resthead, nullptr);
    }

    // Test 2: Allocate a job in a rest list with a single block that fits exactly
    TEST_F(Allocation, FFAllocate_SingleBlockExactFit)
    {
        resthead = createRestBlock(100, 50);
        job = createJob(0, 50, 1);
        resthead = FFAllocate(resthead, job);
        ASSERT_EQ(job->ad, 100);
        ASSERT_EQ(resthead, nullptr);
    }

    // Test 3: Allocate a job in a rest list with a single block that is larger
    TEST_F(Allocation, FFAllocate_SingleBlockLarger)
    {
        resthead = createRestBlock(100, 100);
        job = createJob(0, 50, 1);
        resthead = FFAllocate(resthead, job);
        ASSERT_EQ(job->ad, 100);
        ASSERT_EQ(resthead->ad, 150);
        ASSERT_EQ(resthead->size, 50);
        ASSERT_EQ(resthead->next, nullptr);
    }

    //// Test 4: Allocate a job in a rest list with multiple blocks, first block exactly fits
    TEST_F(Allocation, FFAllocate_MultipleBlocksFirstFit)
    {
        resthead = createRestBlock(100, 50);
        resthead->next = createRestBlock(200, 100);
        job = createJob(0, 50, 1);
        resthead = FFAllocate(resthead, job);
        ASSERT_EQ(job->ad, 100);
        ASSERT_EQ(resthead->ad, 200);
        ASSERT_EQ(resthead->size, 100);
        ASSERT_EQ(resthead->next, nullptr);
    }
    // TEST 5: Allocate a job in a rest list with multiple blocks, first block is larger
	TEST_F(Allocation, FFAllocate_MultipleBlocksFirstLarger)
	{
		resthead = createRestBlock(100, 100);
		resthead->next = createRestBlock(250, 50);
		job = createJob(0, 50, 1);
		resthead = FFAllocate(resthead, job);
		ASSERT_EQ(job->ad, 100);
		ASSERT_EQ(resthead->ad, 150);
		ASSERT_EQ(resthead->size, 50);
		ASSERT_EQ(resthead->next->ad, 250);
        ASSERT_EQ(resthead->next->size, 50);
		ASSERT_EQ(resthead->next->next, nullptr);
	}
    // Test 6: Allocate a job in a rest list with multiple blocks, second block exactly fits, no third block
    TEST_F(Allocation, FFAllocate_MultipleBlocksSecondFit)
    {
        resthead = createRestBlock(100, 30);
        resthead->next = createRestBlock(200, 50);
        job = createJob(0, 50, 1);
        resthead = FFAllocate(resthead, job);
        ASSERT_EQ(job->ad, 200);
        ASSERT_EQ(resthead->ad, 100);
        ASSERT_EQ(resthead->size, 30);
        ASSERT_EQ(resthead->next, nullptr);
    }
	// Test 7: Allocate a job in a rest list with multiple blocks, second block is larger, no third block
	TEST_F(Allocation, FFAllocate_MultipleBlocksSecondLarger)
	{
		resthead = createRestBlock(100, 30);
		resthead->next = createRestBlock(200, 100);
		job = createJob(0, 50, 1);
		resthead = FFAllocate(resthead, job);
		ASSERT_EQ(job->ad, 200);
		ASSERT_EQ(resthead->ad, 100);
		ASSERT_EQ(resthead->size, 30);
		ASSERT_EQ(resthead->next->ad, 250);
		ASSERT_EQ(resthead->next->size, 50);
		ASSERT_EQ(resthead->next->next, nullptr);
	}
	// Test 8: Allocate a job in a rest list with multiple blocks, second block exactly fits, with third block
	TEST_F(Allocation, FFAllocate_MultipleBlocksSecondFitWithThird)
	{
		resthead = createRestBlock(100, 30);
		resthead->next = createRestBlock(200, 50);
		resthead->next->next = createRestBlock(300, 100);
		job = createJob(0, 50, 1);
		resthead = FFAllocate(resthead, job);
		ASSERT_EQ(job->ad, 200);
		ASSERT_EQ(resthead->ad, 100);
		ASSERT_EQ(resthead->size, 30);
		ASSERT_EQ(resthead->next->ad, 300);
		ASSERT_EQ(resthead->next->size, 100);
		ASSERT_EQ(resthead->next->next, nullptr);
	}
	// Test 9: Allocate a job in a rest list with multiple blocks, second block is larger, with third block
	TEST_F(Allocation, FFAllocate_MultipleBlocksSecondLargerWithThird)
	{
		resthead = createRestBlock(100, 30);
		resthead->next = createRestBlock(200, 100);
		resthead->next->next = createRestBlock(300, 50);
		job = createJob(0, 50, 1);
		resthead = FFAllocate(resthead, job);
		ASSERT_EQ(job->ad, 200);
		ASSERT_EQ(resthead->ad, 100);
		ASSERT_EQ(resthead->size, 30);
		ASSERT_EQ(resthead->next->ad, 250);
		ASSERT_EQ(resthead->next->size, 50);
		ASSERT_EQ(resthead->next->next->ad, 300);
		ASSERT_EQ(resthead->next->next->size, 50);
		ASSERT_EQ(resthead->next->next->next, nullptr);
	}
	// Test 10: Allocate a job in a rest list with multiple blocks, no block fits
	TEST_F(Allocation, FFAllocate_MultipleBlocksNoFit)
	{
		resthead = createRestBlock(100, 30);
		resthead->next = createRestBlock(200, 50);
		job = createJob(0, 100, 1);
		resthead = FFAllocate(resthead, job);
		ASSERT_EQ(job->ad, 0);
		ASSERT_EQ(resthead->ad, 100);
		ASSERT_EQ(resthead->size, 30);
		ASSERT_EQ(resthead->next->ad, 200);
		ASSERT_EQ(resthead->next->size, 50);
		ASSERT_EQ(resthead->next->next, nullptr);
	}

}
