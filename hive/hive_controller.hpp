class hive_controller_t {
    private:
        uint32_t HIVE_BUFFER;
        uint32_t HIVE_REGISTERS;
        uint32_t HIVE_REGISTER_SIZE;
        float CORE_TO_BUS_CLOCK_RATIO;
        
        uint32_t LINE_SIZE;
        cache_t **data_cache;

        uint64_t i;
        
        uint32_t offset;
        uint32_t last_instruction;
        bool hive_lock;
        bool* hive_register_free;
        uint32_t nano_requests_number;
        uint32_t* nano_requests_ready;
        uint32_t* hive_op_latencies;

        uint32_t* instruction_count;
        uint32_t* total_latency_count;
        uint32_t* min_latency_count;
        uint32_t* max_latency_count;

        uint64_t op_set_count;
        uint64_t op_count_latency;
        uint64_t last_lock;

        package_state_t* hive_register_state;
        memory_package_t** hive_sub_requests;
        std::vector<memory_package_t*> hive_instructions;

        INSTANTIATE_GET_SET_ADD (uint32_t, HIVE_BUFFER)
        INSTANTIATE_GET_SET_ADD (uint32_t, HIVE_REGISTERS)
        INSTANTIATE_GET_SET_ADD (uint32_t, HIVE_REGISTER_SIZE)
        INSTANTIATE_GET_SET_ADD (uint32_t, LINE_SIZE)
        INSTANTIATE_GET_SET_ADD (float, CORE_TO_BUS_CLOCK_RATIO)
        
        void print_hive_instructions();
        void instruction_ready (size_t index);
        void process (int index);
        void check_transmit (int index);
        void check_wait (int index);
        void hive_dispatch (int index);
        void set_sub_requests (memory_package_t* request);
        void check_sub_requests (size_t hive_register);
        void reset_sub_requests (size_t hive_register);
        
    public:
        hive_controller_t();
        ~hive_controller_t();
        void clock();
        void allocate();
        void statistics(FILE *output);
        void reset_statistics();
        bool addRequest (memory_package_t* request);
};
