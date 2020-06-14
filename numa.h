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

int NumNUMANodes() {
    if (HaveHwlocTopology()) {
        int num_numanodes = hwloc_get_nbobjs_by_type(hwloc_topology, HWLOC_OBJ_NUMANODE);
        return std::max(1, num_numanodes);
    } else {
        return 1;
    }
}

hwloc_obj_t GetHWLocTypeIndex(hwloc_obj_type_t tp, int index) {
  hwloc_obj_t obj = nullptr;
  if (index >= 0) {
    while ((obj = hwloc_get_next_obj_by_type(hwloc_topology, tp, obj)) !=
           nullptr) {
      if (obj->os_index == index) break;
    }
  }
  return obj;
}

int NUMAThreadBind(int node) {
    if (HaveHwlocTopology()) {
        // Find the corresponding NUMA node topology object.
        hwloc_obj_t obj = GetHWLocTypeIndex(HWLOC_OBJ_NUMANODE, node);
        if (obj) {
            hwloc_set_cpubind(hwloc_topology, obj->cpuset,
                            HWLOC_CPUBIND_THREAD | HWLOC_CPUBIND_STRICT);
        } else {
            ELog("Could not find hwloc NUMA node " + node);
        }
  }
    else return -1;
}

void *NUMAMalloc(int size, int node) {
    if (HaveHwlocTopology()) {
        hwloc_obj_t numa_node = GetHWLocTypeIndex(HWLOC_OBJ_NUMANODE, node);
        if (numa_node) {
            return hwloc_alloc_membind(hwloc_topology, size,
                                    numa_node->nodeset, HWLOC_MEMBIND_BIND,
                                    HWLOC_MEMBIND_BYNODESET);
        } else {
            ELog("Failed to find hwloc NUMA node " + node);
        }
    }
    else{
      ELog("Couldn't allocate on specified node");
      return malloc(size);  
    } 
}

void NUMAFree(void *ptr, int size) {
    if (HaveHwlocTopology()) {
        hwloc_free(hwloc_topology, ptr, size);
    }
    return;
}

int NUMAGetThreadNodeAffinity() {
  int node_index = -1;
  if (HaveHwlocTopology()) {
    hwloc_cpuset_t thread_cpuset = hwloc_bitmap_alloc();
    hwloc_get_cpubind(hwloc_topology, thread_cpuset,
                    HWLOC_CPUBIND_THREAD);
    hwloc_obj_t obj = nullptr;
    // Return the first NUMA node whose cpuset is a (non-proper) superset of
    // that of the current thread.
    while ((obj = hwloc_get_next_obj_by_type(
                hwloc_topology, HWLOC_OBJ_NUMANODE, obj)) != nullptr) {
    
        if (hwloc_bitmap_isincluded(thread_cpuset, obj->cpuset)) {
            node_index = obj->os_index;
            break;
        }
    }
    hwloc_bitmap_free(thread_cpuset);
    return node_index;
  }
}