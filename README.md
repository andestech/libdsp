## Andes DSP library

Andes DSP Library provides comprehensive functions designed to simplify and accelerate the development
of digital signal processing systems, ensuring high code efficiency and streamlined code size. These
functions include basic operations for vectors, matrices, and complex vectors, as well as functions that
compute advanced algorithms such as filtering and transform functions

- Andes DSP Library user manual
    The user manual of Andes DSP Library can be found under docs/html.
- Prebuilt library
    The prebuilt library compiled with x86_64 GCC is available in the `prebuilt/` folder.
- For any toolchains
    - Steps to build the Andes DSP Library
        Run ***build_purec_lib.sh*** with the compiler name as the argument. For example:

        `./build_purec_lib.sh "riscv32-unknown-elf-gcc"`
        
        Once the build is complete, a static library named ***libdsp.a*** will be generated under ***build_dir*** folder. For more usage details, please refer to the comments at the beginning of ***build_purec_lib.sh***.
    - Steps to build the example
        1. Navigate to *example* folder
        2. Build the example with the following command (assumes *riscv32-unknown-elf-gcc* or "gcc" as the compiler):
           
           `riscv32-elf-gcc -I./../include -o demo.adx demo.c ../build_dir/libdsp.a`

           `gcc  -I./../include/ -o demo.adx demo.c ../build_dir/libdsp.a`
