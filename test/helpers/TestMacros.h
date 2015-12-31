#ifndef TESTMACROS_H
#define TESTMACROS_H

#include "catch.hpp"

#define FIXTURE_SCENARIO(msg, ...) \
        SCENARIO_METHOD(FIXTURE, msg, TEST_TAG __VA_ARGS__)

#endif // TESTMACROS_H

