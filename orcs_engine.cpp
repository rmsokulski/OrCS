#include "simulator.hpp"

// =====================================================================
orcs_engine_t::orcs_engine_t() {
	this->trace_reader = NULL;
	this->processor = NULL;
	this->branchPredictor = NULL;
	this->cacheManager = NULL;
	this->memory_controller = NULL;
	this->configuration = NULL;
	this->use_pin = false;
}
// =====================================================================
void orcs_engine_t::allocate(uint32_t NUMBER_OF_PROCESSORS) {
	// Statistics Time
	utils_t::process_mem_usage(&this->stat_vm_allocate, &this->stat_rss_allocate);
	gettimeofday(&this->stat_timer_start, NULL);
	gettimeofday(&this->stat_timer_end, NULL);
	// 
	ERROR_ASSERT_PRINTF(utils_t::check_if_power_of_two(NUMBER_OF_PROCESSORS)==OK,"Error - # of processors must be power of 2 value")
	this->NUMBER_OF_PROCESSORS = NUMBER_OF_PROCESSORS;
	this->trace_reader = new trace_reader_t[NUMBER_OF_PROCESSORS]();
	this->processor = new processor_t[NUMBER_OF_PROCESSORS]();
	this->configuration = new configure_t();
	this->branchPredictor = new branch_predictor_t[NUMBER_OF_PROCESSORS]();
	this->cacheManager = new cache_manager_t();
	this->memory_controller = new memory_controller_t();
	this->hive_controller = new hive_controller_t();
	this->vima_controller = new vima_controller_t();
    	this->instruction_set = new instruction_set_t;
}

bool orcs_engine_t::get_simulation_alive(uint32_t NUMBER_OF_PROCESSORS) {
	for(uint16_t cpu=0;cpu<NUMBER_OF_PROCESSORS;cpu++){
		if (this->processor[cpu].isBusy()) return OK;
	}
	if (this->memory_controller->isBusy()) return OK;

	if (this->cacheManager->isBusy()) return OK;

	return FAIL;
}

void orcs_engine_t::reset_statistics(){
	for (uint32_t i = 0; i < NUMBER_OF_PROCESSORS; i++){
		this->processor[i].reset_statistics();
		this->branchPredictor[i].reset_statistics();
		this->cacheManager->reset_statistics(i);
	}
	this->memory_controller->reset_statistics();
	this->hive_controller->reset_statistics();
	this->vima_controller->reset_statistics();
}
