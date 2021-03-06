// Copyright (c) 2019, Lawrence Livermore National Security, LLC.
// See top-level LICENSE file for details.

#include "caliper/caliper-config.h"

#include "caliper/ConfigManager.h"

namespace cali
{

extern ConfigManager::ConfigInfo cuda_activity_controller_info;
extern ConfigManager::ConfigInfo event_trace_controller_info;
extern ConfigManager::ConfigInfo nvprof_controller_info;
extern ConfigManager::ConfigInfo hatchet_region_profile_controller_info;
extern ConfigManager::ConfigInfo hatchet_sample_profile_controller_info;
extern ConfigManager::ConfigInfo runtime_report_controller_info;

ConfigManager::ConfigInfo* builtin_controllers_table[] = {
    &cuda_activity_controller_info,
    &event_trace_controller_info,
    &nvprof_controller_info,
    &hatchet_region_profile_controller_info,
    &hatchet_sample_profile_controller_info,
    &runtime_report_controller_info,
    nullptr
};

const char* builtin_option_specs =
    "["
    "{"
    " \"name\"        : \"profile.mpi\","
    " \"type\"        : \"bool\","
    " \"description\" : \"Profile MPI functions\","
    " \"category\"    : \"region\","
    " \"services\"    : [ \"mpi\" ],"
    " \"extra_config_flags\": { \"CALI_MPI_BLACKLIST\": \"MPI_Comm_rank,MPI_Comm_size,MPI_Wtick,MPI_Wtime\" }"
    "},"
    "{"
    " \"name\"        : \"profile.cuda\","
    " \"type\"        : \"bool\","
    " \"description\" : \"Profile CUDA API functions\","
    " \"category\"    : \"region\","
    " \"services\"    : [ \"cupti\"  ]"
    "},"
    "{"
    " \"name\"        : \"profile.kokkos\","
    " \"type\"        : \"bool\","
    " \"description\" : \"Profile Kokkos functions\","
    " \"category\"    : \"region\","
    " \"services\"    : [ \"kokkostime\"  ]"
    "},"
    "{"
    " \"name\"        : \"io.bytes.written\","
    " \"description\" : \"Report I/O bytes written\","
    " \"type\"        : \"bool\","
    " \"category\"    : \"metric\","
    " \"services\"    : [ \"io\" ],"
    " \"query args\"  : "
    " ["
    "   { \"level\": \"serial\", \"select\": [ { \"expr\": \"sum(sum#io.bytes.written)\", \"as\": \"Bytes written\" } ] },"
    "   { \"level\": \"local\",  \"select\": [ { \"expr\": \"sum(sum#io.bytes.written)\" } ] },"
    "   { \"level\": \"cross\",  \"select\":"
    "     [ { \"expr\": \"avg(sum#sum#io.bytes.written)\", \"as\": \"Avg written\"},"
    "       { \"expr\": \"sum(sum#sum#io.bytes.written)\", \"as\": \"Total written\"}"
    "     ]"
    "   }"
    " ]"
    "},"
    "{"
    " \"name\"        : \"io.bytes.read\","
    " \"description\" : \"Report I/O bytes read\","
    " \"type\"        : \"bool\","
    " \"category\"    : \"metric\","
    " \"services\"    : [ \"io\" ],"
    " \"query args\"  : "
    " ["
    "   { \"level\": \"serial\", \"select\": [ { \"expr\": \"sum(sum#io.bytes.read)\", \"as\": \"Bytes read\" } ] },"
    "   { \"level\": \"local\",  \"select\": [ { \"expr\": \"sum(sum#io.bytes.read)\" } ] },"
    "   { \"level\": \"cross\",  \"select\":"
    "     [ { \"expr\": \"avg(sum#sum#io.bytes.read)\", \"as\": \"Avg read\" },"
    "       { \"expr\": \"sum(sum#sum#io.bytes.read)\", \"as\": \"Total read\" }"
    "     ]"
    "   }"
    " ]"
    "},"
    "{"
    " \"name\"        : \"io.bytes\","
    " \"description\" : \"Report I/O bytes written and read\","
    " \"type\"        : \"bool\","
    " \"category\"    : \"metric\","
    " \"inherit\"     : [ \"io.bytes.read\", \"io.bytes.written\" ]"
    "},"
    "{"
    " \"name\"        : \"io.read.bandwidth\","
    " \"description\" : \"Report I/O read bandwidth\","
    " \"type\"        : \"bool\","
    " \"category\"    : \"metric\","
    " \"services\"    : [ \"io\" ],"
    " \"query args\"  :"
    " ["
    "  { \"level\": \"serial\", \"group by\": \"io.region\", \"select\":"
    "   ["
    "    { \"expr\": \"io.region\", \"as\": \"I/O\" },"
    "    { \"expr\": \"ratio(sum#io.bytes.read,sum#time.duration,8e-6)\", \"as\": \"Read Mbit/s\" }"
    "   ]"
    "  },"
    "  { \"level\": \"local\", \"group by\": \"io.region\", \"select\":"
    "   [ { \"expr\": \"ratio(sum#io.bytes.read,sum#time.duration,8e-6)\" } ]"
    "  },"
    "  { \"level\": \"cross\", \"select\":"
    "   [ "
    "    { \"expr\": \"avg(sum#io.bytes.read/sum#time.duration)\", \"as\": \"Avg read Mbit/s\" },"
    "    { \"expr\": \"max(sum#io.bytes.read/sum#time.duration)\", \"as\": \"Max read Mbit/s\" }"
    "   ]"
    "  }"
    " ]"
    "},"
    "{"
    " \"name\"        : \"io.write.bandwidth\","
    " \"description\" : \"Report I/O write bandwidth\","
    " \"type\"        : \"bool\","
    " \"category\"    : \"metric\","
    " \"services\"    : [ \"io\" ],"
    " \"query args\"  :"
    " ["
    "  { \"level\": \"serial\", \"group by\": \"io.region\", \"select\":"
    "   ["
    "    { \"expr\": \"io.region\", \"as\": \"I/O\" },"
    "    { \"expr\": \"ratio(sum#io.bytes.written,sum#time.duration,8e-6)\", \"as\": \"Write Mbit/s\" }"
    "   ]"
    "  },"
    "  { \"level\": \"local\", \"group by\": \"io.region\", \"select\":"
    "   [ { \"expr\": \"ratio(sum#io.bytes.written,sum#time.duration,8e-6)\" } ]"
    "  },"
    "  { \"level\": \"cross\", \"select\":"
    "   [ "
    "    { \"expr\": \"avg(sum#io.bytes.written/sum#time.duration)\", \"as\": \"Avg write Mbit/s\" },"
    "    { \"expr\": \"max(sum#io.bytes.written/sum#time.duration)\", \"as\": \"Max write Mbit/s\" }"
    "   ]"
    "  }"
    " ]"
    "},"
    "{"
    " \"name\"        : \"mem.highwatermark\","
    " \"description\" : \"Record memory high-water mark for regions\","
    " \"type\"        : \"bool\","
    " \"category\"    : \"metric\","
    " \"services\"    : [ \"alloc\", \"sysalloc\" ],"
    " \"extra_config_flags\" : { \"CALI_ALLOC_TRACK_ALLOCATIONS\": \"false\", \"CALI_ALLOC_RECORD_HIGHWATERMARK\": \"true\" },"
    " \"query args\"  : "
    " ["
    "   { \"level\": \"serial\", \"select\": [ { \"expr\": \"max(max#alloc.region.highwatermark)\", \"as\": \"Max Alloc'd Mem\" } ] },"
    "   { \"level\": \"local\",  \"select\": [ { \"expr\": \"max(max#alloc.region.highwatermark)\" } ] },"
    "   { \"level\": \"cross\",  \"select\": [ { \"expr\": \"max(max#max#alloc.region.highwatermark)\", \"as\": \"Max Alloc'd Mem\" } ] }"
    " ]"
    "},"
    "{"
    " \"name\"        : \"topdown.toplevel\","
    " \"description\" : \"Top-down analysis for Intel CPUs (top level)\","
    " \"type\"        : \"bool\","
    " \"category\"    : \"metric\","
    " \"services\"    : [ \"topdown\" ],"
    " \"extra_config_flags\" : { \"CALI_TOPDOWN_LEVEL\": \"top\" },"
    " \"query args\"  : "
    " ["
    "  { \"level\": \"serial\", \"select\": "
    "   ["
    "    { \"expr\": \"any(topdown.retiring)\", \"as\": \"Retiring\" },"
    "    { \"expr\": \"any(topdown.backend_bound)\", \"as\": \"Backend bound\" },"
    "    { \"expr\": \"any(topdown.frontend_bound)\", \"as\": \"Frontend bound\" },"
    "    { \"expr\": \"any(topdown.bad_speculation)\", \"as\": \"Bad speculation\" }"
    "   ]"
    "  },"
    "  { \"level\": \"local\", \"select\": "
    "   ["
    "    { \"expr\": \"any(topdown.retiring)\", \"as\": \"Retiring\" },"
    "    { \"expr\": \"any(topdown.backend_bound)\", \"as\": \"Backend bound\" },"
    "    { \"expr\": \"any(topdown.frontend_bound)\", \"as\": \"Frontend bound\" },"
    "    { \"expr\": \"any(topdown.bad_speculation)\", \"as\": \"Bad speculation\" }"
    "   ]"
    "  },"
    "  { \"level\": \"cross\", \"select\": "
    "   ["
    "    { \"expr\": \"any(any#topdown.retiring)\", \"as\": \"Retiring\" },"
    "    { \"expr\": \"any(any#topdown.backend_bound)\", \"as\": \"Backend bound\" },"
    "    { \"expr\": \"any(any#topdown.frontend_bound)\", \"as\": \"Frontend bound\" },"
    "    { \"expr\": \"any(any#topdown.bad_speculation)\", \"as\": \"Bad speculation\" }"
    "   ]"
    "  }"
    " ]"
    "},"
    "{"
    " \"name\"        : \"topdown.all\","
    " \"description\" : \"Top-down analysis for Intel CPUs (all levels)\","
    " \"type\"        : \"bool\","
    " \"category\"    : \"metric\","
    " \"services\"    : [ \"topdown\" ],"
    " \"extra_config_flags\" : { \"CALI_TOPDOWN_LEVEL\": \"all\" },"
    " \"query args\"  : "
    " ["
    "  { \"level\": \"serial\", \"select\": "
    "   ["
    "    { \"expr\": \"any(topdown.retiring)\", \"as\": \"Retiring\" },"
    "    { \"expr\": \"any(topdown.backend_bound)\", \"as\": \"Backend bound\" },"
    "    { \"expr\": \"any(topdown.frontend_bound)\", \"as\": \"Frontend bound\" },"
    "    { \"expr\": \"any(topdown.bad_speculation)\", \"as\": \"Bad speculation\" },"
    "    { \"expr\": \"any(topdown.branch_mispredict)\", \"as\": \"Branch mispredict\" },"
    "    { \"expr\": \"any(topdown.machine_clears)\", \"as\": \"Machine clears\" },"
    "    { \"expr\": \"any(topdown.frontend_latency)\", \"as\": \"Frontend latency\" },"
    "    { \"expr\": \"any(topdown.frontend_bandwidth)\", \"as\": \"Frontend bandwidth\" },"
    "    { \"expr\": \"any(topdown.memory_bound)\", \"as\": \"Memory bound\" },"
    "    { \"expr\": \"any(topdown.core_bound)\", \"as\": \"Core bound\" },"
    "    { \"expr\": \"any(topdown.ext_mem_bound)\", \"as\": \"External Memory\" },"
    "    { \"expr\": \"any(topdown.l1_bound)\", \"as\": \"L1 bound\" },"
    "    { \"expr\": \"any(topdown.l2_bound)\", \"as\": \"L2 bound\" },"
    "    { \"expr\": \"any(topdown.l3_bound)\", \"as\": \"L3 bound\" }"
    "   ]"
    "  },"
    "  { \"level\": \"local\", \"select\": "
    "   ["
    "    { \"expr\": \"any(topdown.retiring)\", \"as\": \"Retiring\" },"
    "    { \"expr\": \"any(topdown.backend_bound)\", \"as\": \"Backend bound\" },"
    "    { \"expr\": \"any(topdown.frontend_bound)\", \"as\": \"Frontend bound\" },"
    "    { \"expr\": \"any(topdown.bad_speculation)\", \"as\": \"Bad speculation\" },"
    "    { \"expr\": \"any(topdown.branch_mispredict)\", \"as\": \"Branch mispredict\" },"
    "    { \"expr\": \"any(topdown.machine_clears)\", \"as\": \"Machine clears\" },"
    "    { \"expr\": \"any(topdown.frontend_latency)\", \"as\": \"Frontend latency\" },"
    "    { \"expr\": \"any(topdown.frontend_bandwidth)\", \"as\": \"Frontend bandwidth\" },"
    "    { \"expr\": \"any(topdown.memory_bound)\", \"as\": \"Memory bound\" },"
    "    { \"expr\": \"any(topdown.core_bound)\", \"as\": \"Core bound\" },"
    "    { \"expr\": \"any(topdown.ext_mem_bound)\", \"as\": \"External Memory\" },"
    "    { \"expr\": \"any(topdown.l1_bound)\", \"as\": \"L1 bound\" },"
    "    { \"expr\": \"any(topdown.l2_bound)\", \"as\": \"L2 bound\" },"
    "    { \"expr\": \"any(topdown.l3_bound)\", \"as\": \"L3 bound\" }"
    "   ]"
    "  },"
    "  { \"level\": \"cross\", \"select\": "
    "   ["
    "    { \"expr\": \"any(any#topdown.retiring)\", \"as\": \"Retiring\" },"
    "    { \"expr\": \"any(any#topdown.backend_bound)\", \"as\": \"Backend bound\" },"
    "    { \"expr\": \"any(any#topdown.frontend_bound)\", \"as\": \"Frontend bound\" },"
    "    { \"expr\": \"any(any#topdown.bad_speculation)\", \"as\": \"Bad speculation\" },"
    "    { \"expr\": \"any(any#topdown.branch_mispredict)\", \"as\": \"Branch mispredict\" },"
    "    { \"expr\": \"any(any#topdown.machine_clears)\", \"as\": \"Machine clears\" },"
    "    { \"expr\": \"any(any#topdown.frontend_latency)\", \"as\": \"Frontend latency\" },"
    "    { \"expr\": \"any(any#topdown.frontend_bandwidth)\", \"as\": \"Frontend bandwidth\" },"
    "    { \"expr\": \"any(any#topdown.memory_bound)\", \"as\": \"Memory bound\" },"
    "    { \"expr\": \"any(any#topdown.core_bound)\", \"as\": \"Core bound\" },"
    "    { \"expr\": \"any(any#topdown.ext_mem_bound)\", \"as\": \"External Memory\" },"
    "    { \"expr\": \"any(any#topdown.l1_bound)\", \"as\": \"L1 bound\" },"
    "    { \"expr\": \"any(any#topdown.l2_bound)\", \"as\": \"L2 bound\" },"
    "    { \"expr\": \"any(any#topdown.l3_bound)\", \"as\": \"L3 bound\" }"
    "   ]"
    "  }"
    " ]"
    "},"
    "{"
    " \"name\"        : \"topdown-counters.toplevel\","
    " \"description\" : \"Raw counter values for Intel top-down analysis (top level)\","
    " \"type\"        : \"bool\","
    " \"category\"    : \"metric\","
    " \"services\"    : [ \"papi\" ],"
    " \"extra_config_flags\" : "
    " {"
    "   \"CALI_PAPI_COUNTERS\": "
    "     \"CPU_CLK_THREAD_UNHALTED:THREAD_P,UOPS_RETIRED:RETIRE_SLOTS,UOPS_ISSUED:ANY,INT_MISC:RECOVERY_CYCLES,IDQ_UOPS_NOT_DELIVERED:CORE\""
    " },"
    " \"query args\"  : "
    " ["
    "  { \"level\": \"serial\", \"select\": "
    "   ["
    "    { \"expr\": \"inclusive_sum(sum#papi.CPU_CLK_THREAD_UNHALTED:THREAD_P)\", \"as\": \"cpu_clk_thread_unhalted:thread_p\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.UOPS_RETIRED:RETIRE_SLOTS)\",   \"as\": \"uops_retired:retire_slots\"    },"
    "    { \"expr\": \"inclusive_sum(sum#papi.UOPS_ISSUED:ANY)\",             \"as\": \"uops_issued:any\"              },"
    "    { \"expr\": \"inclusive_sum(sum#papi.INT_MISC:RECOVERY_CYCLES)\",    \"as\": \"int_misc:recovery_cycles\"     },"
    "    { \"expr\": \"inclusive_sum(sum#papi.IDQ_UOPS_NOT_DELIVERED:CORE)\", \"as\": \"idq_uops_note_delivered:core\" }"
    "   ]"
    "  },"
    "  { \"level\": \"local\", \"select\": "
    "   ["
    "    { \"expr\": \"inclusive_sum(sum#papi.CPU_CLK_THREAD_UNHALTED:THREAD_P)\", \"as\": \"cpu_clk_thread_unhalted:thread_p\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.UOPS_RETIRED:RETIRE_SLOTS)\",   \"as\": \"uops_retired:retire_slots\"    },"
    "    { \"expr\": \"inclusive_sum(sum#papi.UOPS_ISSUED:ANY)\",             \"as\": \"uops_issued:any\"              },"
    "    { \"expr\": \"inclusive_sum(sum#papi.INT_MISC:RECOVERY_CYCLES)\",    \"as\": \"int_misc:recovery_cycles\"     },"
    "    { \"expr\": \"inclusive_sum(sum#papi.IDQ_UOPS_NOT_DELIVERED:CORE)\", \"as\": \"idq_uops_note_delivered:core\" }"
    "   ]"
    "  },"
    "  { \"level\": \"cross\", \"select\": "
    "   ["
    "    { \"expr\": \"sum(inclusive#sum#papi.CPU_CLK_THREAD_UNHALTED:THREAD_P)\", \"as\": \"cpu_clk_thread_unhalted:thread_p\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.UOPS_RETIRED:RETIRE_SLOTS)\",   \"as\": \"uops_retired:retire_slots\"    },"
    "    { \"expr\": \"sum(inclusive#sum#papi.UOPS_ISSUED:ANY)\",             \"as\": \"uops_issued:any\"              },"
    "    { \"expr\": \"sum(inclusive#sum#papi.INT_MISC:RECOVERY_CYCLES)\",    \"as\": \"int_misc:recovery_cycles\"     },"
    "    { \"expr\": \"sum(inclusive#sum#papi.IDQ_UOPS_NOT_DELIVERED:CORE)\", \"as\": \"idq_uops_note_delivered:core\" }"
    "   ]"
    "  }"
    " ]"
    "},"
    "{"
    " \"name\"        : \"topdown-counters.all\","
    " \"description\" : \"Raw counter values for Intel top-down analysis (all levels)\","
    " \"type\"        : \"bool\","
    " \"category\"    : \"metric\","
    " \"services\"    : [ \"papi\" ],"
    " \"extra_config_flags\" : "
    " {"
    "   \"CALI_PAPI_COUNTERS\": "
    "     \"BR_MISP_RETIRED:ALL_BRANCHES"
    "      ,CPU_CLK_THREAD_UNHALTED:THREAD_P"
    "      ,CYCLE_ACTIVITY:CYCLES_NO_EXECUTE"
    "      ,CYCLE_ACTIVITY:STALLS_L1D_PENDING"
    "      ,CYCLE_ACTIVITY:STALLS_L2_PENDING"
    "      ,CYCLE_ACTIVITY:STALLS_LDM_PENDING"
    "      ,IDQ_UOPS_NOT_DELIVERED:CORE"
    "      ,IDQ_UOPS_NOT_DELIVERED:CYCLES_0_UOPS_DELIV_CORE"
    "      ,INT_MISC:RECOVERY_CYCLES"
    "      ,MACHINE_CLEARS:COUNT"
    "      ,MEM_LOAD_UOPS_RETIRED:L3_HIT"
    "      ,MEM_LOAD_UOPS_RETIRED:L3_MISS"
    "      ,UOPS_EXECUTED:CORE_CYCLES_GE_1"
    "      ,UOPS_EXECUTED:CORE_CYCLES_GE_2"
    "      ,UOPS_ISSUED:ANY"
    "      ,UOPS_RETIRED:RETIRE_SLOTS\""
    " },"
    " \"query args\"  : "
    " ["
    "  { \"level\": \"serial\", \"select\": "
    "   ["
    "    { \"expr\": \"inclusive_sum(sum#papi.BR_MISP_RETIRED:ALL_BRANCHES)\", \"as\": \"br_misp_retired:all_branches\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.CPU_CLK_THREAD_UNHALTED:THREAD_P)\", \"as\": \"cpu_clk_thread_unhalted:thread_p\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.CYCLE_ACTIVITY:CYCLES_NO_EXECUTE)\", \"as\": \"cycle_activity:cycles_no_execute\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.CYCLE_ACTIVITY:STALLS_L1D_PENDING)\", \"as\": \"cycle_activity:stalls_l1d_pending\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.CYCLE_ACTIVITY:STALLS_L2_PENDING)\", \"as\": \"cycle_activity:stalls_l2_pending\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.CYCLE_ACTIVITY:STALLS_LDM_PENDING)\", \"as\": \"cycle_activity:stalls_ldm_pending\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.IDQ_UOPS_NOT_DELIVERED:CORE)\", \"as\": \"idq_uops_note_delivered:core\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.IDQ_UOPS_NOT_DELIVERED:CYCLES_0_UOPS_DELIV_CORE)\", \"as\": \"idq_uops_note_delivered:cycles_0_uops_deliv_core\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.INT_MISC:RECOVERY_CYCLES)\", \"as\": \"int_misc:recovery_cycles\"     },"
    "    { \"expr\": \"inclusive_sum(sum#papi.MACHINE_CLEARS:COUNT)\", \"as\": \"machine_clears:count\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.MEM_LOAD_UOPS_RETIRED:L3_HIT)\", \"as\": \"mem_load_uops_retired:l3_hit\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.MEM_LOAD_UOPS_RETIRED:L3_MISS)\", \"as\": \"mem_load_uops_retired:l3_miss\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.UOPS_EXECUTED:CORE_CYCLES_GE_1)\", \"as\": \"uops_executed:core_cycles_ge_1\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.UOPS_EXECUTED:CORE_CYCLES_GE_2)\", \"as\": \"uops_executed:core_cycles_ge_2\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.UOPS_ISSUED:ANY)\",             \"as\": \"uops_issued:any\"              },"
    "    { \"expr\": \"inclusive_sum(sum#papi.UOPS_RETIRED:RETIRE_SLOTS)\",   \"as\": \"uops_retired:retire_slots\"    }"
    "   ]"
    "  },"
    "  { \"level\": \"local\", \"select\": "
    "   ["
    "    { \"expr\": \"inclusive_sum(sum#papi.BR_MISP_RETIRED:ALL_BRANCHES)\", \"as\": \"br_misp_retired:all_branches\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.CPU_CLK_THREAD_UNHALTED:THREAD_P)\", \"as\": \"cpu_clk_thread_unhalted:thread_p\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.CYCLE_ACTIVITY:CYCLES_NO_EXECUTE)\", \"as\": \"cycle_activity:cycles_no_execute\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.CYCLE_ACTIVITY:STALLS_L1D_PENDING)\", \"as\": \"cycle_activity:stalls_l1d_pending\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.CYCLE_ACTIVITY:STALLS_L2_PENDING)\", \"as\": \"cycle_activity:stalls_l2_pending\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.CYCLE_ACTIVITY:STALLS_LDM_PENDING)\", \"as\": \"cycle_activity:stalls_ldm_pending\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.IDQ_UOPS_NOT_DELIVERED:CORE)\", \"as\": \"idq_uops_note_delivered:core\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.IDQ_UOPS_NOT_DELIVERED:CYCLES_0_UOPS_DELIV_CORE)\", \"as\": \"idq_uops_note_delivered:cycles_0_uops_deliv_core\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.INT_MISC:RECOVERY_CYCLES)\", \"as\": \"int_misc:recovery_cycles\"     },"
    "    { \"expr\": \"inclusive_sum(sum#papi.MACHINE_CLEARS:COUNT)\", \"as\": \"machine_clears:count\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.MEM_LOAD_UOPS_RETIRED:L3_HIT)\", \"as\": \"mem_load_uops_retired:l3_hit\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.MEM_LOAD_UOPS_RETIRED:L3_MISS)\", \"as\": \"mem_load_uops_retired:l3_miss\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.UOPS_EXECUTED:CORE_CYCLES_GE_1)\", \"as\": \"uops_executed:core_cycles_ge_1\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.UOPS_EXECUTED:CORE_CYCLES_GE_2)\", \"as\": \"uops_executed:core_cycles_ge_2\" },"
    "    { \"expr\": \"inclusive_sum(sum#papi.UOPS_ISSUED:ANY)\",             \"as\": \"uops_issued:any\"              },"
    "    { \"expr\": \"inclusive_sum(sum#papi.UOPS_RETIRED:RETIRE_SLOTS)\",   \"as\": \"uops_retired:retire_slots\"    }"
    "   ]"
    "  },"
    "  { \"level\": \"cross\", \"select\": "
    "   ["
    "    { \"expr\": \"sum(inclusive#sum#papi.BR_MISP_RETIRED:ALL_BRANCHES)\", \"as\": \"br_misp_retired:all_branches\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.CPU_CLK_THREAD_UNHALTED:THREAD_P)\", \"as\": \"cpu_clk_thread_unhalted:thread_p\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.CYCLE_ACTIVITY:CYCLES_NO_EXECUTE)\", \"as\": \"cycle_activity:cycles_no_execute\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.CYCLE_ACTIVITY:STALLS_L1D_PENDING)\", \"as\": \"cycle_activity:stalls_l1d_pending\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.CYCLE_ACTIVITY:STALLS_L2_PENDING)\", \"as\": \"cycle_activity:stalls_l2_pending\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.CYCLE_ACTIVITY:STALLS_LDM_PENDING)\", \"as\": \"cycle_activity:stalls_ldm_pending\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.IDQ_UOPS_NOT_DELIVERED:CORE)\", \"as\": \"idq_uops_note_delivered:core\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.IDQ_UOPS_NOT_DELIVERED:CYCLES_0_UOPS_DELIV_CORE)\", \"as\": \"idq_uops_note_delivered:cycles_0_uops_deliv_core\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.INT_MISC:RECOVERY_CYCLES)\", \"as\": \"int_misc:recovery_cycles\"     },"
    "    { \"expr\": \"sum(inclusive#sum#papi.MACHINE_CLEARS:COUNT)\", \"as\": \"machine_clears:count\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.MEM_LOAD_UOPS_RETIRED:L3_HIT)\", \"as\": \"mem_load_uops_retired:l3_hit\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.MEM_LOAD_UOPS_RETIRED:L3_MISS)\", \"as\": \"mem_load_uops_retired:l3_miss\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.UOPS_EXECUTED:CORE_CYCLES_GE_1)\", \"as\": \"uops_executed:core_cycles_ge_1\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.UOPS_EXECUTED:CORE_CYCLES_GE_2)\", \"as\": \"uops_executed:core_cycles_ge_2\" },"
    "    { \"expr\": \"sum(inclusive#sum#papi.UOPS_ISSUED:ANY)\",             \"as\": \"uops_issued:any\"              },"
    "    { \"expr\": \"sum(inclusive#sum#papi.UOPS_RETIRED:RETIRE_SLOTS)\",   \"as\": \"uops_retired:retire_slots\"    }"
    "   ]"
    "  }"
    " ]"
    "},"
    "{"
    " \"name\"        : \"output\","
    " \"description\" : \"Output location ('stdout', 'stderr', or filename)\","
    " \"type\"        : \"string\","
    " \"category\"    : \"output\""
    "}"
    "]";

}
