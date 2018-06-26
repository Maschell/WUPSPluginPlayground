#include <wups.h>
#include <wut_types.h>
#include "memory_defs.h"
#include "c_retain_vars.h"

DECL_FUNCTION(MEMHeapHandle, MEMCreateFrmHeapEx, void* address, uint32_t size, uint16_t flags) {
    MEMHeapHandle result = real_MEMCreateFrmHeapEx(address,size,flags);
    for(int i = 0; i<MAX_FRM_HEAP_INFOS; i++) {
        if(frmHeapPointer[i].handle == 0) {
            frmHeapPointer[i].handle = result;
            frmHeapPointer[i].address = address;
            frmHeapPointer[i].size = size;
            frmHeapPointer[i].flags = flags;
            break;
        }
    }
    return result;
}

DECL_FUNCTION(MEMHeapHandle, MEMCreateUserHeapHandle, void* startAddress, uint32_t size) {
    MEMHeapHandle result = real_MEMCreateUserHeapHandle(startAddress,size);
    for(int i = 0; i<MAX_USER_HEAP_INFOS; i++) {
        if(userHeapInfos[i].handle == 0) {
            userHeapInfos[i].handle = result;
            userHeapInfos[i].address = startAddress;
            userHeapInfos[i].size = size;
            break;
        }
    }

    return result;
}

DECL_FUNCTION(MEMHeapHandle, MEMCreateExpHeapEx, void* address, uint32_t size, uint16_t flags) {
    uint32_t oldSize = size;
    MEMHeapHandle result = NULL;
    bool allocated = false;

    /*
    real_MEMCreateExpHeapEx Just accepts ALL sizes... so.. well..
    if(size > 0x3B600000 && (uint32_t) address < 0x11000000){
        //We are pretty sure this the MEM2 default heap.
        //Let's try to increase it's capacity.
        //uint32_t maximumSize = 0x50000000 - (uint32_t) address;
        uint32_t maximumSize = 0x80000000;
        //uint32_t maximumSize = size + 0x500000;
        maximumSize -= 0x500000; // remove 5MB, just to leave at least _some_ space.

        uint32_t curSizeTest = maximumSize;

        do{
            if(curSizeTest < oldSize){
                break;
            }
            result = real_MEMCreateExpHeapEx(address,curSizeTest,flags);
            if(result == 0){ // On failure:
                maximumSize -= 0x100000; // reduce by 1MB and try again.
            }else{
                allocated = true;
                break;
            }
        }while(!allocated); // Until the allocation was successful, or we are below default size.
    }*/
    if(!allocated || result == 0) {
        result = real_MEMCreateExpHeapEx(address,size,flags);
    }

    for(int i = 0; i<MAX_EXP_HEAP_INFOS; i++) {
        if(expHeapPointer[i].handle == 0) {
            expHeapPointer[i].handle = result;
            expHeapPointer[i].address = address;
            expHeapPointer[i].size = size;
            expHeapPointer[i].oldSize = oldSize;
            expHeapPointer[i].flags = flags;
            break;
        }
    }

    return result;
}

DECL_FUNCTION(int32_t, MEMCreateUnitHeapEx, void* address, uint32_t heapSize, uint32_t memSize, uint32_t align, uint16_t flags) {
    int32_t result = real_MEMCreateUnitHeapEx(address,heapSize,memSize,align,flags);

    for(int i = 0; i<MAX_UNIT_HEAP_INFOS; i++) {
        if(unitHeapPointer[i].handle == 0) {
            unitHeapPointer[i].handle = result;
            unitHeapPointer[i].address = address;
            unitHeapPointer[i].heapSize = heapSize;
            unitHeapPointer[i].memSize = memSize;
            unitHeapPointer[i].align = align;
            unitHeapPointer[i].flags = flags;
            break;
        }
    }

    return result;
}

DECL_FUNCTION(void *, MEMDestroyExpHeap, MEMHeapHandle heap) {
    void * result = real_MEMDestroyExpHeap(heap);

    for(int i = 0; i<MAX_EXP_HEAP_INFOS; i++) {
        if(expHeapPointer[i].handle == heap) {
            expHeapPointer[i].handle = 0;
            expHeapPointer[i].address = 0;
            expHeapPointer[i].size = 0;
            expHeapPointer[i].flags = 0;
            break;
        }
    }

    return result;
}

DECL_FUNCTION(void *, MEMDestroyFrmHeap, MEMHeapHandle heap) {
    void * result = real_MEMDestroyFrmHeap(heap);

    for(int i = 0; i<MAX_FRM_HEAP_INFOS; i++) {
        if(frmHeapPointer[i].handle == heap) {
            frmHeapPointer[i].handle = 0;
            frmHeapPointer[i].address = 0;
            frmHeapPointer[i].size = 0;
            frmHeapPointer[i].flags = 0;
            break;
        }
    }

    return result;
}

DECL_FUNCTION(void *, MEMDestroyUnitHeap, int32_t heap) {
    void * result = real_MEMDestroyUnitHeap(heap);

    for(int i = 0; i<MAX_UNIT_HEAP_INFOS; i++) {
        if(unitHeapPointer[i].handle == heap) {
            unitHeapPointer[i].handle = 0;
            unitHeapPointer[i].address = 0;
            unitHeapPointer[i].heapSize = 0;
            unitHeapPointer[i].memSize = 0;
            unitHeapPointer[i].align = 0;
            unitHeapPointer[i].flags = 0;
            break;
        }
    }

    return result;
}

DECL_FUNCTION(int32_t, MEMSetBaseHeapHandle, int32_t arena, int32_t heap) {
    int32_t result = real_MEMSetBaseHeapHandle(arena,heap);
    for(int i = 0; i<MAX_BASE_HEAP_INFOS; i++) {
        if(baseHeaphandles[i].newHandle == 0) {
            baseHeaphandles[i].oldHandle = result;
            baseHeaphandles[i].newHandle = heap;
            baseHeaphandles[i].memArena = arena;
            break;
        }
    }
    return result;
}

DECL_FUNCTION(int32_t, MEMInitBlockHeap,void * tracking, void * startAddress,void * endAddress, void * initTrackMemory, uint32_t initTrackMemoryBytes, uint16_t flags) {
    int32_t result = real_MEMInitBlockHeap(tracking,startAddress,endAddress,initTrackMemory,initTrackMemoryBytes,flags);
    for(int i = 0; i<MAX_BLOCK_HEAP_INFOS; i++) {
        if(blockHeapInfos[i].startAddress == NULL) {
            blockHeapInfos[i].handle = result;
            blockHeapInfos[i].tracking = tracking;
            blockHeapInfos[i].startAddress = startAddress;
            blockHeapInfos[i].endAddress = endAddress;
            blockHeapInfos[i].initTrackMem = initTrackMemory;
            blockHeapInfos[i].initTrackMemBytes = initTrackMemoryBytes;
            blockHeapInfos[i].flags = flags;
            break;
        }
    }
    return result;
}

WUPS_MUST_REPLACE(MEMInitBlockHeap,         WUPS_LOADER_LIBRARY_COREINIT,   MEMInitBlockHeap);
WUPS_MUST_REPLACE(MEMSetBaseHeapHandle,     WUPS_LOADER_LIBRARY_COREINIT,   MEMSetBaseHeapHandle);
WUPS_MUST_REPLACE(MEMDestroyUnitHeap,       WUPS_LOADER_LIBRARY_COREINIT,   MEMDestroyUnitHeap);
WUPS_MUST_REPLACE(MEMDestroyFrmHeap,        WUPS_LOADER_LIBRARY_COREINIT,   MEMDestroyFrmHeap);
WUPS_MUST_REPLACE(MEMDestroyExpHeap,        WUPS_LOADER_LIBRARY_COREINIT,   MEMDestroyExpHeap);
WUPS_MUST_REPLACE(MEMCreateUnitHeapEx,      WUPS_LOADER_LIBRARY_COREINIT,   MEMCreateUnitHeapEx);
WUPS_MUST_REPLACE(MEMCreateExpHeapEx,       WUPS_LOADER_LIBRARY_COREINIT,   MEMCreateExpHeapEx);
WUPS_MUST_REPLACE(MEMCreateUserHeapHandle,  WUPS_LOADER_LIBRARY_COREINIT,   MEMCreateUserHeapHandle);
WUPS_MUST_REPLACE(MEMCreateFrmHeapEx,       WUPS_LOADER_LIBRARY_COREINIT,   MEMCreateFrmHeapEx);

/*
Unused stuff that might get useful in the future.

void * allocateFromExpHeap(int32_t * usedHandle, uint32_t size, uint32_t align){
    void * result = NULL;
    for(int i = 0;i<MAX_EXP_HEAP_INFOS;i++){
        if(expHeapPointer[i].handle != 0){
            result = MEMAllocFromExpHeapEx(expHeapPointer[i].handle, size, align);
            if(result != NULL){
                *usedHandle = expHeapPointer[i].handle;
                return result;
            }
        }
    }
    return NULL;
}

void * allocateFromFrmHeap(int32_t * usedHandle, uint32_t size, uint32_t align){
    void * result = NULL;
    for(int i = 0;i<MAX_FRM_HEAP_INFOS;i++){
        if(frmHeapPointer[i].handle != 0){
            result = MEMAllocFromFrmHeapEx(frmHeapPointer[i].handle, size, align);
            if(result != NULL){
                *usedHandle = frmHeapPointer[i].handle;
                return result;
            }
        }
    }
    return NULL;
}

void freeToExpHeap(int32_t usedHandle, void * addr){
    MEMFreeToExpHeap(usedHandle, addr);
}
allocatorExpInfo allocatorExpPointer[MAX_ALLOCATOR_EXP_INFOS] __attribute__((section(".data")));

typedef struct allocatorExpInfo_{
    void * allocator;
    int32_t heap;
    int32_t align;
} allocatorExpInfo;

DECL(void , MEMInitAllocatorForExpHeap, void* allocator, int32_t heap, int32_t align){
    for(int i = 0;i<MAX_ALLOCATOR_EXP_INFOS;i++){
        if(allocatorExpPointer[i].allocator == 0){
            allocatorExpPointer[i].allocator = (void*)allocator;
            allocatorExpPointer[i].heap = heap;
            allocatorExpPointer[i].align = align;
            break;
        }
    }
    real_MEMInitAllocatorForExpHeap(allocator,heap,align);
    return;
}
*/
