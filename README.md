Installing XDMF
===============

Brief instructions for building XDMF from source code: 

* Clone the git repository:

  ```sh
  git clone git://xdmf.org/Xdmf.git
  ```

* Create a directory for building the code:

  ```sh
  cd Xdmf
  mkdir build 
  cd build
  ```

* Prepare the CMake-based build. There are two options:

  1. Use the following command and select options using a menu.

     ```sh
     ccmake ..
     ```

  2. Set options on the command line. A typical command line for building 
     XDMF with Python bindings may look like the following:

     ```sh
     export XDMF_INSTALL_DIR=/opt/Xdmf/
     cmake .. -DCMAKE_INSTALL_PREFIX=${XDMF_INSTALL_DIR} \
              -DBUILD_SHARED_LIBS=1 -DXDMF_WRAP_PYTHON=1 -Wno-dev
     ```

* Build and install XDMF:

  ```sh
  make 
  make install
  ```
   
