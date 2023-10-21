#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "libmylibrary.h"

TEST_CASE("testing the library") {
    CHECK(library_function() == 42);
}
