/// DETAIL DESCRIPTION: Almost all errors and messages use this definition.
/// It will DEACTIVATE all the other messages below
#define ORCS_PRINTF(...) printf(__VA_ARGS__);

#define ORCS_DEBUG 1
#ifdef ORCS_DEBUG
// #define CBUFFER_DEBUG
// ~ #define CONFIGURATOR_DEBUG
// ~ #define TRACE_READER_DEBUG
// ~ #define TRACE_GENERATOR_DEBUG
// #define PROCESSOR_DEBUG
// ~ #define SYNC_DEBUG
// ~ #define BRANCH_PREDICTOR_DEBUG
#define CACHE_DEBUG
// ~ #define PREFETCHER_DEBUG
// ~ #define LINE_USAGE_PREDICTOR_DEBUG
// #define MEMORY_CONTROLLER_DEBUG
// ~ #define ROUTER_DEBUG
// ~ #define INTERCONNECTION_CTRL_DEBUG
// ~ #define DIRECTORY_CTRL_DEBUG
// ~ #define SHOW_FREE_PACKAGE
#define DEBUG_PRINTF(...)                   \
    {                                       \
        if (orcs_engine.get_global_cycle()) \
        {                                   \
            ORCS_PRINTF("DEBUG: ");         \
            ORCS_PRINTF(__VA_ARGS__);       \
        }                                   \
    }
#else
#define DEBUG_PRINTF(...)
#endif

#define ERROR_INFORMATION() {\
                                ORCS_PRINTF("ERROR INFORMATION\n");\
                                ORCS_PRINTF("ERROR: File: %s at Line: %u\n", __FILE__, __LINE__);\
                                ORCS_PRINTF("ERROR: Function: %s\n", __PRETTY_FUNCTION__);\
                                ORCS_PRINTF("ERROR: Cycle: %" PRIu64 "\n", orcs_engine.get_global_cycle());\
                            }


#define ERROR_ASSERT_PRINTF(v, ...) if (!(v)) {\
                                        ERROR_INFORMATION();\
                                        ORCS_PRINTF("ERROR_ASSERT: %s\n", #v);\
                                        ORCS_PRINTF("\nERROR: ");\
                                        ORCS_PRINTF(__VA_ARGS__);\
                                        exit(EXIT_FAILURE);\
                                    }

#define ERROR_PRINTF(...) {\
                              ERROR_INFORMATION();\
                              ORCS_PRINTF("\nERROR: ");\
                              ORCS_PRINTF(__VA_ARGS__);\
                              exit(EXIT_FAILURE);\
                          }

#define INFO_PRINTF(...){\
                        ORCS_PRINTF("INFO : EVENT OCUURED AT Cycle: %" PRIu64 "\n", orcs_engine.get_global_cycle());\
                        exit(EXIT_SUCCESS);\
                        }
// ============================================================================
/// MACROS to create get_ and set_ methods for variables.
// ============================================================================
/// Creates the Get, Set methods automatically.
#define INSTANTIATE_GET_SET(TYPE, NAME) \
    inline void set_ ## NAME(TYPE input_NAME) {\
        this->NAME = input_NAME;\
    };\
    inline TYPE get_ ## NAME() {\
        return this->NAME;\
    };

/// Creates the Get, Set and Add methods automatically (useful to statistics).
#define INSTANTIATE_GET_SET_ADD(TYPE, NAME) \
    inline void add_ ## NAME() {\
        this->NAME++;\
    };\
    inline void set_ ## NAME(TYPE input_NAME) {\
        this->NAME = input_NAME;\
    };\
    inline TYPE get_ ## NAME() {\
        return this->NAME;\
    };
