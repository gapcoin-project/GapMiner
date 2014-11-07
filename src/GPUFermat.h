#ifndef __GPU_FERMAT_H__
#define __GPU_FERMAT_H__
#include <CL/cl.hpp>
#include <vector>
#include <string>
#include <gmp.h>
#include <gmpxx.h>

/**
 * A singleton class which is used to run fermat tests on one GPU
 */
class GPUFermat {

  private:

    /* a buffer to synchronize gpu and host memory */
    class clBuffer {
      
      public:

        /* size of this buffer */
        int Size;

        /* host data */
        uint32_t* HostData;

        /* gpu data */
        cl_mem DeviceData;
        
        clBuffer();
        ~clBuffer();
        
        /* initialize this */
        void init(int size, cl_mem_flags flags = 0);
        
        /* copy the clBuffer content to gpu */
        void copyToDevice(cl_command_queue cq, bool blocking = true);
        
        /* copy the clBuffer content to host */
        void copyToHost(cl_command_queue cq, bool blocking = true, unsigned size = 0);
        
        /* access the host data of a clBuffer */
        uint32_t &get(int index);
        
        /* access the host data of a clBuffer */
        uint32_t &operator[](int index);
    };


    /* synchronization mutexes */
    static pthread_mutex_t creation_mutex;

    /* the only instance of this */
    static GPUFermat *only_instance;

    /* indicates if this was initialized */
    static bool initialized;

    /* the GPU work group size */
    static unsigned GroupSize;

    /* the work items for the gpu */
    static unsigned workItems;

    /* the array size of uint32_t for the numbers to test */
    static unsigned operandSize;

    /* the number of prime candidates to test at once */
    unsigned elementsNum;

    /* total number of array elements used fr storing the testing numbers */
    unsigned numberLimbsNum;

    /* number of work groups for the GPU */
    unsigned groupsNum;

    /* the numbers to test for primality */
    clBuffer numbers;

    /* the gpu fermat results */
    clBuffer gpuResults;
    
    /* the opencl program */
    cl_program gProgram;

    /* the fermat kernel to run on the gpu */
    cl_kernel mFermatKernel320;  

    /* cl device to run the test on */
    cl_device_id gpu;

    /* computing units of the used cpu */
    cl_uint computeUnits;

    /* command queue for the GPU */
    cl_command_queue queue;

    /* this is a singleton */
    GPUFermat();


    /* initialize opencl for the given device */
    bool init_cl(unsigned device_id = 0);


    void benchmark2();

    uint32_t rand32();


    /* run the Fermat test on the gpu */
    void run_fermat(cl_command_queue queue,
                    cl_kernel kernel,
                    clBuffer &numbers,
                    clBuffer &gpuResults,
                    unsigned elementsNum);

    /* run a benchmark and print results */
    void fermatTestBenchmark(cl_command_queue queue,
                             cl_kernel kernel,
                             clBuffer &numbers,
                             unsigned elementsNum);

  public :

    /* return the only instance of this */
    static GPUFermat *get_instance();

    /* public interface to the gpu Fermat test */
    void fermat_gpu(uint32_t *candidates, bool *results);

    /* run a benchmark test */
    void benchmark();

    /* test the gpu results */
    void test_gpu();
};
#endif /* __GPU_FERMAT_H__ */
