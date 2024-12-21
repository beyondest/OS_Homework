#include "pch.h"
#include <random>
#define ASSERT_JOB_EQ(expected, actual) \
    ASSERT_EQ(expected.ad, actual.ad); \
    ASSERT_EQ(expected.name, actual.name); \
    ASSERT_EQ(expected.size, actual.size);

#define ASSERT_USEJOB_EQ(puse,j) \
    ASSERT_EQ(puse->ad, j->ad); \
    ASSERT_EQ(puse->name, j->name); \
    ASSERT_EQ(puse->size, j->size); \

extern "C" {
#include "LinkList.h"
#include "crtdbg.h"
}
namespace InitFreeTest
{
    TEST(InitUseLinkList, InitEmptyUseLinkList)
    {
        struct use* usehead = InitUseLinkList();
        ASSERT_EQ(usehead, nullptr); // The linked list is empty
    }
    TEST(InitRestLinkList, InitNonEmptyRestLinkList)
    {
        struct rest* resthead = InitRestLinkList();
        ASSERT_EQ(resthead->ad, FirstAddress); // The first address is the default value
        ASSERT_EQ(resthead->size, SUM);
        ASSERT_EQ(resthead->next, nullptr); // The end of the linked list is null
        resthead = FreeRestLinkList(resthead); // Free the linked list
    }
    TEST(FreeRestLinkList, FreeNonEmptyRestLinkList)
    {
        struct rest* resthead = InitRestLinkList();
        resthead = FreeRestLinkList(resthead);
        ASSERT_EQ(resthead, nullptr);
    }
    TEST(FreeRestLinkList, FreeEmptyRestLinkList)
    {
        struct rest* resthead = nullptr;
        ASSERT_NO_THROW(resthead = FreeRestLinkList(resthead));
        ASSERT_EQ(resthead, nullptr);
    }
    TEST(FreeUseLinkList, FreeEmptyUseLinkList)
    {
        struct use* usehead = InitUseLinkList();
        ASSERT_NO_THROW(usehead = FreeUseLinkList(usehead));
        ASSERT_EQ(usehead, nullptr);
    }
    TEST(FreeUseLinkList, FreeNonEmptyUseLinkList)
    {
        struct use* usehead = InitUseLinkList();
        struct job* job0 = (struct job*)malloc(sizeof(struct job));
        if(!job0)exit(1);
        job0->ad = 1;
        job0->size = 100;
        job0->name = 111;
        usehead = (struct use*)malloc(sizeof(struct use));
        if(!usehead)exit(1);
        usehead->ad = job0->ad;
        usehead->name = job0->name;
        usehead->size = job0->size;
        usehead->next = nullptr;
        usehead = FreeUseLinkList(usehead);
        ASSERT_EQ(usehead, nullptr);
        free(job0);
    }
}

namespace FunctionTest
{
    // LinkList test fixture for setting up and tearing down test cases
    class LinkList : public ::testing::Test
    {
    protected:
        struct use* usehead = nullptr;
        struct rest* resthead = nullptr;
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_int_distribution<> dis;

        // Set up the test environment
        void SetUp() override
        {
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 100);
            std::uniform_int_distribution<> dis2(101, 200);
            resthead = InitRestLinkList();
            usehead = InitUseLinkList();
        }

        // Tear down the test environment
        void TearDown() override
        {
            FreeRestLinkList(resthead);
            FreeUseLinkList(usehead);
        }

        // Helper function to create a job
        struct job* createJob(int address, int size, int name) {
            struct job* j = (struct job*)malloc(sizeof(struct job));
            if (!j) exit(1);
            j->ad = address < 0 ? dis(gen) : address;
            j->size = size < 0 ? dis(gen) : size;
            j->name = name < 0 ? dis(gen) : name;
            return j;
        }
    };

    // Test adding a job to an empty use list
    TEST_F(LinkList, AddJob2UseLlist_AddToEmptyList)
    {
        struct job* job0 = createJob(-1, -1, -1);
        usehead = AddJob2UseLlist(usehead, job0);
        ASSERT_USEJOB_EQ(usehead, job0);
        ASSERT_EQ(usehead->next, nullptr);
        free(job0);
    }

    // Test adding a job to a non-empty use list
    TEST_F(LinkList, AddJob2UseLlist_AddToNonEmptyList)
    {
        struct job* job0 = createJob(-1, -1, 111);
        usehead = AddJob2UseLlist(usehead, job0);
        struct job* job1 = createJob(-1, -1, 222);
        usehead = AddJob2UseLlist(usehead, job1);

        ASSERT_USEJOB_EQ(usehead, job0);
        ASSERT_USEJOB_EQ(usehead->next, job1);
        ASSERT_EQ(usehead->next->next, nullptr);
        free(job0);
        free(job1);
    }

    // Test removing a job from an empty use list
    TEST_F(LinkList, DelJobFromeUseList_RemoveFromEmptyList)
    {
        struct job* job0 = createJob(-1, -1, -1);
        ASSERT_NO_THROW(usehead = DelJobFromUseLlist(usehead, job0););
        ASSERT_EQ(usehead, nullptr);
        free(job0);
    }

    // Test removing the first job from a use list
    TEST_F(LinkList, DelJobFromeUseList_RemoveFromFirst)
    {
        struct job* job0 = createJob(-1, -1, 111);
        usehead = AddJob2UseLlist(usehead, job0);
        struct job* job1 = createJob(-1, -1, 222);
        usehead = AddJob2UseLlist(usehead, job1);
        ASSERT_USEJOB_EQ(usehead, job0);
        ASSERT_USEJOB_EQ(usehead->next, job1);
        ASSERT_EQ(usehead->next->next, nullptr);
        ASSERT_NO_THROW(usehead = DelJobFromUseLlist(usehead, job0););
        ASSERT_USEJOB_EQ(usehead, job1);
        ASSERT_EQ(usehead->next, nullptr);
        free(job0);
        free(job1);
    }

    // Test removing a job from the middle of a use list
    TEST_F(LinkList, DelJobFromeUseList_RemoveFromMiddle)
    {
        struct job* job0 = createJob(-1, -1, 111);
        usehead = AddJob2UseLlist(usehead, job0);
        struct job* job1 = createJob(-1, -1, 222);
        usehead = AddJob2UseLlist(usehead, job1);
        struct job* job2 = createJob(-1, -1, 333);
        usehead = AddJob2UseLlist(usehead, job2);
        ASSERT_USEJOB_EQ(usehead, job0);
        ASSERT_USEJOB_EQ(usehead->next, job1);
        ASSERT_USEJOB_EQ(usehead->next->next, job2);
        ASSERT_NO_THROW(usehead = DelJobFromUseLlist(usehead, job1););
        ASSERT_USEJOB_EQ(usehead, job0);
        ASSERT_USEJOB_EQ(usehead->next, job2);
        ASSERT_EQ(usehead->next->next, nullptr);
        free(job0);
        free(job1);
        free(job2);
    }

    // Test removing the last job from a use list
    TEST_F(LinkList, DelJobFromeUseList_RemoveFromLast)
    {
        struct job* job0 = createJob(-1, -1, 111);
        usehead = AddJob2UseLlist(usehead, job0);
        struct job* job1 = createJob(-1, -1, 222);
        usehead = AddJob2UseLlist(usehead, job1);
        ASSERT_USEJOB_EQ(usehead, job0);
        ASSERT_USEJOB_EQ(usehead->next, job1);
        ASSERT_NO_THROW(usehead = DelJobFromUseLlist(usehead, job1););
        ASSERT_USEJOB_EQ(usehead, job0);
        ASSERT_EQ(usehead->next, nullptr);
        free(job0);
        free(job1);
    }
    class RecycleRestLlistTest :public ::FunctionTest::LinkList
    {
    protected:

        // Helper function to create a rest list
        struct rest* createRestList(std::vector<std::pair<int, int>> blocks) {
            struct rest* head = nullptr;
            struct rest* current = nullptr;

            for (const auto& block : blocks) {
                struct rest* newRest = (struct rest*)malloc(sizeof(struct rest));
                if (!newRest) exit(1);
                newRest->ad = block.first;
                newRest->size = block.second;
                newRest->next = nullptr;
                if (!head) {
                    head = newRest;
                    current = head;
                }
                else {
                    current->next = newRest;
                    current = newRest;
                }
            }
            return head;
        }
        // Helper function to verify rest list contents, only check ad and size
        void verifyRestList(struct rest* head, std::vector<std::pair<int, int>> expected) {
            size_t index = 0;
            struct rest* current = head;
            while (current && index < expected.size()) {
                EXPECT_EQ(current->ad, expected[index].first);
                EXPECT_EQ(current->size, expected[index].second);
                current = current->next;
                index++;
            }
            EXPECT_EQ(index, expected.size());
            EXPECT_EQ(current, nullptr);
        }
    };
    // Test 1: Recycle a job into an empty rest list
    TEST_F(RecycleRestLlistTest, EmptyRestList) {
        struct job* testJob = createJob(100, 50, -1);
        struct rest* result = RecycleRestLlist(testJob, nullptr);
        std::vector<std::pair<int, int>> expected = { {100, 50} };
        verifyRestList(result, expected);
        free(testJob);
    }

    // Test 2: Insert a job before an existing block and merge with it
    TEST_F(RecycleRestLlistTest, InsertBeforeWithMerging) {
        struct rest* resthead = createRestList({ {200, 50} });
        struct job* testJob = createJob(150, 50, -1);
        struct rest* result = RecycleRestLlist(testJob, resthead);
        std::vector<std::pair<int, int>> expected = { {150, 100} };
        verifyRestList(result, expected);
        free(testJob);
    }

    // Test 3: Insert a job before an existing block without merging
    TEST_F(RecycleRestLlistTest, InsertBeforeNoMerging) {
        struct rest* resthead = createRestList({ {200, 50} });
        struct job* testJob = createJob(100, 50, -1);
        struct rest* result = RecycleRestLlist(testJob, resthead);
        std::vector<std::pair<int, int>> expected = { {100, 50}, {200, 50} };
        verifyRestList(result, expected);
        free(testJob);
    }

    // Test 4: Insert a job between existing blocks without merging
    TEST_F(RecycleRestLlistTest, InsertBetweenNoMerging) {
        struct rest* resthead = createRestList({ {100, 50}, {300, 50} });
        struct job* testJob = createJob(200, 50, -1);
        struct rest* result = RecycleRestLlist(testJob, resthead);
        std::vector<std::pair<int, int>> expected = { {100, 50}, {200, 50}, {300, 50} };
        verifyRestList(result, expected);
        free(testJob);
    }

    // Test 5: Insert a job between existing blocks and merge with both sides
    TEST_F(RecycleRestLlistTest, InsertBetweenMergeBothSides) {
        struct rest* resthead = createRestList({ {100, 50}, {200, 50} });
        struct job* testJob = createJob(150, 50, -1);
        struct rest* result = RecycleRestLlist(testJob, resthead);
        std::vector<std::pair<int, int>> expected = { {100, 150} };
        verifyRestList(result, expected);
        free(testJob);
    }

    // Test 6: Insert a job between existing blocks and merge with the front block
    TEST_F(RecycleRestLlistTest, InsertBetweenMergeFront) {
        struct rest* resthead = createRestList({ {100, 50}, {300, 50} });
        struct job* testJob = createJob(150, 50, -1);
        struct rest* result = RecycleRestLlist(testJob, resthead);
        std::vector<std::pair<int, int>> expected = { {100, 100}, {300, 50} };
        verifyRestList(result, expected);
        free(testJob);
    }

    // Test 7: Insert a job between existing blocks and merge with the back block
    TEST_F(RecycleRestLlistTest, InsertBetweenMergeBack) {
        struct rest* resthead = createRestList({ {50, 50}, {200, 50} });
        struct job* testJob = createJob(150, 50, -1);
        struct rest* result = RecycleRestLlist(testJob, resthead);
        std::vector<std::pair<int, int>> expected = { {50, 50}, {150, 100} };
        verifyRestList(result, expected);
        free(testJob);
    }

    // Test 8: Insert a job at the end of the list without merging
    TEST_F(RecycleRestLlistTest, InsertAtEndNoMerging) {
        struct rest* resthead = createRestList({ {100, 50} });
        struct job* testJob = createJob(200, 50, -1);
        struct rest* result = RecycleRestLlist(testJob, resthead);
        std::vector<std::pair<int, int>> expected = { {100, 50}, {200, 50} };
        verifyRestList(result, expected);
        free(testJob);
    }

    // Test 9: Insert a job at the end of the list and merge with the last block
    TEST_F(RecycleRestLlistTest, InsertAtEndWithMerging) {
        struct rest* resthead = createRestList({ {100, 50} });
        struct job* testJob = createJob(150, 50, -1);
        struct rest* result = RecycleRestLlist(testJob, resthead);
        std::vector<std::pair<int, int>> expected = { {100, 100} };
        verifyRestList(result, expected);
        free(testJob);
    }

}



