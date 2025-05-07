Open source for Andes DSP library

- User manual:

   * docs/html/index.html


- How to build libdsp.a

  - execute the script "build_purec_lib.sh", then "libdsp.a" will be compiled in the "build_dir/" folder

     $ ./build_purec_lib.sh ${COMPILER_NAME}
       - $1: COMPILER_NAME (ex: riscv32-elf-gcc, riscv64-elf-gcc, gcc, riscv32-elf-clang, riscv64-elf-clang , ...)

     Ex: ./build_purec_lib.sh "riscv32-elf-gcc"  ## for Andes toolchain <br>
     Ex: ./build_purec_lib.sh "gcc"              ## for x86 toolchain

- How to link libdsp.a

  - Here is a demo.c for function "riscv_dsp_add_f32"

     $ riscv32-elf-gcc -I./include -o demo.adx demo.c build_dir/libdsp.a  <br>
     $ gcc -I./include -o demo.adx demo.c build_dir/libdsp.a
