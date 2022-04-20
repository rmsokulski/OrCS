class transaction_t {
    public:
        uint8_t status;
        uint64_t readyAt;
        uint64_t unique_conversion_id;

        void package_clean() {
            this->status = 0;
            this->readyAt = 0;
            this->unique_conversion_id = 0;
        }
};

class vima_controller_t {
    private:
        uint32_t VIMA_BUFFER;
        uint32_t VIMA_VECTOR_SIZE;
        uint32_t VIMA_CACHE_ASSOCIATIVITY;
        uint32_t VIMA_CACHE_LATENCY;
        uint32_t VIMA_CACHE_SIZE;
        uint32_t VIMA_UNBALANCED;
        float CORE_TO_BUS_CLOCK_RATIO;
        
        uint32_t lines;
        uint32_t sets;
        uint32_t* vima_op_latencies;
        uint64_t current_cache_access_latency;
        vima_vector_t** cache;
        vima_vector_t* read1;
        vima_vector_t* read1_unbalanced;
        vima_vector_t* read2;
        vima_vector_t* read2_unbalanced;
        vima_vector_t* write;
        vima_vector_t* write_unbalanced;
        std::vector<memory_package_t*> vima_buffer;
        std::vector<memory_package_t*> sub_request_queue;
        std::vector<vima_vector_t*> working_vectors;
        

        // Transactions controller
        // 0 -> Esperando CPU
        // 1 -> Sucesso
        // 2 -> FALHA
        // <<Transaction status, readyAt>, unique_conversion_id>
        circular_buffer_t<transaction_t> CPU_transactions_status;

        uint64_t index_bits_mask;
        uint64_t index_bits_shift;

        uint64_t tag_bits_mask;
        uint64_t tag_bits_shift;

        uint64_t cache_reads;
        uint64_t cache_writes;
        uint64_t cache_hits;
        uint64_t cache_misses;
        uint64_t cache_accesses;
        uint64_t cache_writebacks;

        uint64_t i;
        uint64_t current_index;
        uint64_t request_count;
        uint64_t total_wait;

        // Communications
        uint32_t BURST_WIDTH;
        uint32_t LINE_SIZE;
        uint32_t latency_burst;


        INSTANTIATE_GET_SET_ADD (uint32_t, VIMA_BUFFER)
        INSTANTIATE_GET_SET_ADD (uint32_t, VIMA_VECTOR_SIZE)
        INSTANTIATE_GET_SET_ADD (uint32_t, VIMA_CACHE_ASSOCIATIVITY)
        INSTANTIATE_GET_SET_ADD (uint32_t, VIMA_CACHE_LATENCY)
        INSTANTIATE_GET_SET_ADD (uint32_t, VIMA_CACHE_SIZE)
        INSTANTIATE_GET_SET_ADD (uint32_t, VIMA_UNBALANCED)
        INSTANTIATE_GET_SET_ADD (float, CORE_TO_BUS_CLOCK_RATIO)
        INSTANTIATE_GET_SET_ADD (uint32_t, BURST_WIDTH)
        INSTANTIATE_GET_SET_ADD (uint32_t, LINE_SIZE)
        INSTANTIATE_GET_SET_ADD (uint32_t, latency_burst)


        INSTANTIATE_GET_SET_ADD (uint32_t, lines)
        INSTANTIATE_GET_SET_ADD (uint32_t, sets)
        INSTANTIATE_GET_SET_ADD (uint64_t, cache_reads)
        INSTANTIATE_GET_SET_ADD (uint64_t, cache_writes)
        INSTANTIATE_GET_SET_ADD (uint64_t, cache_hits)
        INSTANTIATE_GET_SET_ADD (uint64_t, cache_misses)
        INSTANTIATE_GET_SET_ADD (uint64_t, cache_accesses)
        INSTANTIATE_GET_SET_ADD (uint64_t, cache_writebacks)
        
        void print_vima_instructions();
        void check_cache (int index);
        vima_vector_t* search_cache (uint64_t address, cache_status_t* result);
        void check_completion (int index);
        void write_to_cache (int index);
        
        // Get channel to access DATA
        inline uint64_t get_index(uint64_t addr) {
            return (addr & this->index_bits_mask) >> this->index_bits_shift;
        }

        inline uint64_t get_tag(uint64_t addr) {
            return (addr & this->tag_bits_mask) >> this->tag_bits_shift;
        }
        
    public:
        vima_controller_t();
        ~vima_controller_t();
        void clock();
        void allocate();
        bool addRequest (memory_package_t* request);
        void instruction_ready (size_t index);
        void statistics();
        void reset_statistics();
        inline void confirm_transaction(uint8_t status, uint64_t unique_conversion_id);
};

inline void vima_controller_t::confirm_transaction(uint8_t status, uint64_t unique_conversion_id) {
    transaction_t new_transaction;
    new_transaction.package_clean();
    new_transaction.status = status;
    new_transaction.readyAt = orcs_engine.get_global_cycle() + this->latency_burst;
    new_transaction.unique_conversion_id = unique_conversion_id;
    this->CPU_transactions_status.push_back(new_transaction);
}
