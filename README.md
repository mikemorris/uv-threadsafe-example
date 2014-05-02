```
malloc: *** error for object 0x7fa1406ffffc: pointer being freed was not allocated

* thread #2: tid = 0xaa9e9, 0x00007fff8c4d7ba4 libsystem_platform.dylib`_platform_memmove$VARIANT$Unknown + 164, stop reason = EXC_BAD_ACCESS (code=1, address=0x1006ffff4)
  * frame #0: 0x00007fff8c4d7ba4 libsystem_platform.dylib`_platform_memmove$VARIANT$Unknown + 164
    frame #1: 0x000000010000333a main.o`std::__1::vector<int, std::__1::allocator<int> >::__swap_out_circular_buffer(std::__1::__split_buffer<int, std::__1::allocator<int>&>&) + 250
    frame #2: 0x00000001000031bd main.o`void std::__1::vector<int, std::__1::allocator<int> >::__push_back_slow_path<int const&>(int const&&&) + 829
    frame #3: 0x00000001000012a0 main.o`AsyncReadWriteVector(uv_work_s*) + 656
    frame #4: 0x0000000100012e15 libuv.dylib`worker + 86
    frame #5: 0x00000001000158a9 libuv.dylib`uv__thread_start + 25
    frame #6: 0x00007fff8bd1a899 libsystem_pthread.dylib`_pthread_body + 138
    frame #7: 0x00007fff8bd1a72a libsystem_pthread.dylib`_pthread_start + 137
```
