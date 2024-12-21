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

    // Test 1: Best Fit Allocation - Allocate a job in an empty rest list
    TEST_F(Allocation, BFAllocate_EmptyRestList)
    {
        job = createJob(0, 50, 1);
        resthead = BFAllocate(resthead, job);
        ASSERT_EQ(job->ad, 0);
        ASSERT_EQ(resthead, nullptr);
    }

    // Test 2: Best Fit Allocation - Allocate a job in a rest list with a single block that fits exactly
    TEST_F(Allocation, BFAllocate_SingleBlockExactFit)
    {
        resthead = createRestBlock(100, 50);
        job = createJob(0, 50, 1);
        resthead = BFAllocate(resthead, job);
        ASSERT_EQ(job->ad, 100);
        ASSERT_EQ(resthead, nullptr);
    }

    // Test 3: Best Fit Allocation - Allocate a job in a rest list with a single block that is larger
    TEST_F(Allocation, BFAllocate_SingleBlockLarger)
    {
        resthead = createRestBlock(100, 100);
        job = createJob(0, 50, 1);
        resthead = BFAllocate(resthead, job);
        ASSERT_EQ(job->ad, 100);
        ASSERT_EQ(resthead->ad, 150);
        ASSERT_EQ(resthead->size, 50);
        ASSERT_EQ(resthead->next, nullptr);
    }

    // Test 4: Best Fit Allocation - Allocate a job in a rest list with multiple blocks, best fit block exactly fits
    TEST_F(Allocation, BFAllocate_MultipleBlocksBestFit)
    {
        resthead = createRestBlock(100, 30);
        resthead->next = createRestBlock(200, 50);
        resthead->next->next = createRestBlock(300, 100);
        job = createJob(0, 50, 1);
        resthead = BFAllocate(resthead, job);
        ASSERT_EQ(job->ad, 200);
        ASSERT_EQ(resthead->ad, 100);
        ASSERT_EQ(resthead->size, 30);
        ASSERT_EQ(resthead->next->ad, 300);
        ASSERT_EQ(resthead->next->size, 100);
        ASSERT_EQ(resthead->next->next, nullptr);
    }

    // Test 5: Best Fit Allocation - Allocate a job in a rest list with multiple blocks, best fit block is larger
    TEST_F(Allocation, BFAllocate_MultipleBlocksBestFitLarger)
    {
        resthead = createRestBlock(100, 30);
        resthead->next = createRestBlock(200, 90);
        resthead->next->next = createRestBlock(300, 70);
        job = createJob(0, 50, 1);
        resthead = BFAllocate(resthead, job);
        ASSERT_EQ(job->ad, 300);
        ASSERT_EQ(resthead->ad, 100);
        ASSERT_EQ(resthead->size, 30);
        ASSERT_EQ(resthead->next->ad, 200);
        ASSERT_EQ(resthead->next->size, 90);
        ASSERT_EQ(resthead->next->next->ad, 350);
        ASSERT_EQ(resthead->next->next->size, 20);
        ASSERT_EQ(resthead->next->next->next, nullptr);
    }

    // Test 6: Best Fit Allocation - Allocate a job in a rest list with multiple blocks, no block fits
    TEST_F(Allocation, BFAllocate_MultipleBlocksNoFit)
    {
        resthead = createRestBlock(100, 30);
        resthead->next = createRestBlock(200, 50);
        job = createJob(0, 100, 1);
        resthead = BFAllocate(resthead, job);
        ASSERT_EQ(job->ad, 0);
        ASSERT_EQ(resthead->ad, 100);
        ASSERT_EQ(resthead->size, 30);
        ASSERT_EQ(resthead->next->ad, 200);
        ASSERT_EQ(resthead->next->size, 50);
        ASSERT_EQ(resthead->next->next, nullptr);
    }

}