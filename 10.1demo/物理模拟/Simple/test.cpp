#include <iostream>    // 用于输入输出
#include <sys/mman.h>  // 内存映射相关函数
#include <fcntl.h>     // 文件控制选项
#include <unistd.h>

int main(){
    // 指定文件
    const char* shm_file = "/dev/shm/joy_stick_data";

    // opend the given file in READONLY(O_RDONLY)
    int fd = open(shm_file,O_RDONLY);

    // mapping the file into the memory so that the infoes can be read even faster!

    // the prameters have something to think about:

    // I have create a table to explain that:

    // --------------------------------------------------------------------------------------------------------------
    /*
    extern void *mmap (void *__addr, size_t __len, int __prot,
		   int __flags, int __fd, __off_t __offset) __THROW;
    */
    // --------------------------------------------------------------------------------------------------------------
    
    // 1. "void *__addr" : void *__addr 
    // 这是建议的映射起始地址。通常设置为NULL，表示由内核自动选择地址。如果非空，内核会尽量使用该地址，但可能会因为对齐或其他原因不使用。

    // 2. size_t __len
    // 映射区域的长度，以字节为单位。注意：系统会自动将其调整为页大小的整数倍（通常是4KB）。

    // 3. int __prot
    //指定内存区域的保护方式，可以是以下值的组合（通过按位或）：
        // PROT_READ：页面可读。
        // PROT_WRITE：页面可写。
        // PROT_EXEC：页面可执行。
        // PROT_NONE：页面不可访问。
    
    // 4. int __flags
    /*控制映射的类型和映射页面的行为。常见标志包括：
        MAP_SHARED：对映射区域的修改会写回文件，并且其他进程可见。
        MAP_PRIVATE：创建一个写时复制（copy-on-write）的映射，对映射区域的修改不会写回文件，而是单独复制一份。
        MAP_ANONYMOUS：映射不基于文件，而是匿名内存，内容初始化为0。使用此标志时，__fd参数被忽略，通常设置为-1。
        MAP_FIXED：强制使用__addr作为映射地址，如果该地址无法使用，则失败。
    */

    // 5. int __fd
    /*int __fd:

    文件描述符，指向要映射的文件。如果使用匿名映射（MAP_ANONYMOUS），此参数应设为-1。
    这里我用的文件映射， 所以传的参数是 open(shm_file, O_RDONLY)；
    */

    // 6. __off_t __offset:
    //文件映射的偏移量，通常设置为0。必须是页大小的整数倍。


    void* addr = mmap(NULL, 12, PROT_READ, MAP_SHARED, fd, 0);
    std::cout << addr << std::endl; //  0x7e1c26af7000

    // 访问内存
    int* shared_data = (int*)addr;

    try{
        while(true){
            double x_off = shared_data[0] - (-1186);
            double y_off = shared_data[1] - (-417);

            std:: cout << x_off << " " << y_off << "  " << shared_data[2] << "  " << std::endl;
            usleep(10000);   
        }
    }catch(...){
        std:: cout << "out" << std::endl;
    }

    munmap(addr, 12);
    close(fd);
    std::cout << "close" << std::endl;
}