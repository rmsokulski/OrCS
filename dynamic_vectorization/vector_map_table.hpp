class vector_map_table_t {
    public:
        set_associative_t<vector_map_table_entry_t> entries;
        table_of_loads_t *TL;
        Vectorizer_t *vectorizer;
        register_rename_table_t *register_rename_table;
        circular_buffer_t <opcode_package_t> *inst_list;


        vector_map_table_t (int32_t num_entries,
                             int32_t associativity,
                             register_rename_table_t *RRT,
                             Vectorizer_t *vectorizer, 
                             table_of_loads_t *TL, 
                             circular_buffer_t <opcode_package_t> *inst_list);
        ~vector_map_table_t ();
        vector_map_table_entry_t* allocate_entry (uint64_t pc);
        bool compare_registers (opcode_package_t *inst, vector_map_table_entry_t *vrmt_entry);
        DV::DV_ERROR convert_to_validation (opcode_package_t *inst, vector_map_table_entry_t *vrmt_entry, int32_t validation_index);

        vector_map_table_entry_t *find_pc (uint64_t pc);
        void invalidate (vector_map_table_entry_t *vrmt_entry);
        bool new_store (opcode_package_t *inst);

        DV::DV_ERROR validate (opcode_package_t *inst, vector_map_table_entry_t *vrmt_entry);
        void fill_vectorial_part (opcode_package_t *inst, bool is_load, int32_t vr_id, int32_t num_part);
        DV::DV_ERROR vectorize (opcode_package_t * inst, vector_map_table_entry_t **vrmt_entry, bool forward);

        void list_contents();

        // Check if two addresses are in the same cache line
        inline bool same_block(uint64_t addr_a, uint64_t addr_b);
};

inline bool vector_map_table_t::same_block(uint64_t addr_a, uint64_t addr_b) {
    // Considerando caches de 64 bytes
    uint64_t block_a = addr_a >> 6;
    uint64_t block_b = addr_b >> 6;
    if (block_a == block_b) return true;
    return false;
}