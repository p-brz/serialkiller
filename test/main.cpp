#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "utils/Log.h"

int main( int argc, char* const argv[] ){
    //TODO: allow define log level from command line
    calmframe::MainLog().setLogLevel(calmframe::LogLevel::Debug);

    Catch::Session session; // There must be exactly once instance
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode == 0 ){ // returnCode != 0 indicates a command line error
        returnCode = session.run();
    }

    return returnCode;
}
