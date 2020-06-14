#include <iostream>
#include <hwloc.h>
#include "logging.h"

const int kNUMANoAffinity = -1;

static hwloc_topology_t hwloc_topology;

bool HaveHwlocTopology() {
    static bool init = [](){
        if (hwloc_topology_init(&hwloc_topology)) {
            ELog("Could not initialize hwloc topology");
            return false;
        }
        if (hwloc_topology_load(hwloc_topology)) {
            ELog("Could not load hwloc topology");
            return false;
        }
        return true;
    }();
    return init;
}

int NUMAThreadBind(int numa_node) {
    if (HaveHwlocTopology()) {

    }
    else return -1;
}

void *NUMAMalloc(int size) {
    if (HaveHwlocTopology()) {

    }
    else return -1;
}

int NUMAGetThreadNodeAffinity() {
  int node_index = -1;
    hwloc_cpuset_t thread_cpuset = hwloc_bitmap_alloc();
    hwloc_get_cpubind(hwloc_topology_handle, thread_cpuset,
                      HWLOC_CPUBIND_THREAD);
    hwloc_obj_t obj = nullptr;
    // Return the first NUMA node whose cpuset is a (non-proper) superset of
    // that of the current thread.
    while ((obj = hwloc_get_next_obj_by_type(
                hwloc_topology_handle, HWLOC_OBJ_NUMANODE, obj)) != nullptr) {
      
      if (hwloc_bitmap_isincluded(thread_cpuset, obj->cpuset)) {
        node_index = obj->os_index;
        break;
      }
    }
    hwloc_bitmap_free(thread_cpuset);
  return node_index;
}