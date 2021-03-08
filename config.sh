cmake .. \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=true \
  -DENABLE_SIMMETRIX=ON \
  -DSIM_MPI=mpich3.3.2 \
  -DSIM_PARASOLID=ON \
  -DCMAKE_CXX_COMPILER=g++ \
  -DCMAKE_C_COMPILER=gcc  \
  -DSKIP_SIMMETRIX_VERSION_CHECK=true

  #-DSIMMETRIX_INCLUDE_DIR=/opt/scorec/spack/v0154/install2/linux-rhel7-x86_64/gcc-10.1.0/simmetrix-simmodsuite-16.0-201021dev-rnnnsocxcmmq55waddi5do2i7vavxpu3/include/ \
  #-DSIMMETRIX_LIB_DIR=/opt/scorec/spack/v0154/install2/linux-rhel7-x86_64/gcc-10.1.0/simmetrix-simmodsuite-16.0-201021dev-rnnnsocxcmmq55waddi5do2i7vavxpu3/lib/x64_rhel7_gcc48/ \
  #-DCMAKE_CXX_COMPILER=/opt/scorec/spack/v0154/install2/linux-rhel7-x86_64/gcc-6.5.0/gcc-10.1.0-6aux4ghzryg2mkoaacx7lzj7yye65sux/bin/g++
