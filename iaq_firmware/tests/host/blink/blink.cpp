#include "wifi.h"

#include <stdio.h>

#include <iostream>

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
TEST_GROUP(BlinkTest){
    void setup() override{
        // Code here will be called before each test in this group
    }

    void teardown() override{
        // Code here will be called after each test in this group
    }
};

TEST(BlinkTest, ReconnectTest)
{
    // This is a simple test to check if the reConnect function works
    reConnect();
    CHECK(true); // Replace with actual checks as needed
}

int main(int ac, char **av)
{

    printf("\nRunning tests with CppUTest\n\n");

    const char *fav[3] = {"sunspec", "-c", "-v"};

    return CommandLineTestRunner::RunAllTests(3, fav);
}

