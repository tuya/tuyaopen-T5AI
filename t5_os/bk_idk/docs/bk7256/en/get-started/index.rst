Quick Start Guide
==============================================

:link_to_translation:`zh_CN:[中文]`

Take the development board BK7256 as an example, and demonstrate the use method through the project

 - Build Armino's compilation environment
 - Method of configuring Project Armino
 - How to compile and download firmware

Preparation
--------------------------------------------------------

Hardware：

 - Board BK7256
 - Serial port buring tool
 - PC（Windows & Ubuntu）

!Note:

  Armino supports compilation on the Linux platform and also supports compilation on the Linux virtual machine on the Windows platform;
  Armino supports firmware burning on Windows/Linux platforms (refer to the guidance document in the burning tool).


Software：

 - RISCV tool chain, used to compile the Armino RISCV version
 - Build tools, including CMake
 - Armino source code
 - Serial port burning software




Armino SDK Code download
--------------------------------------------------------------------

We can download Armino from gitlab::

    mkdir -p ~/armino
    cd ~/armino
    git clone https://gitlab.bekencorp.com/armino/bk_idk.git

Then switch to the stable branch Tag node, such as v2.0.1.12::

    git checkout -B your_branch_name v2.0.1.12


.. note::

    The latest SDK code is downloaded from gitlab on the official website, and
	relevant accounts can be found on the project to review the application.


Build Compilation Environment
--------------------------------------------------------------------

.. note::

    The Armino compilation environment requires Ubuntu 20.04 LTS version and above. This chapter will take Ubuntu 20.04 LTS version as an example to introduce the construction of the entire compilation environment.



Tool Chain Installation
----------------------------------------------------------------

BK7256 Tool download path：

	Toolchain Download：
	http://dl.bekencorp.com/tools/toolchain/
	Get the latest version in this directory, ps：toolchain_v5.2.1.tgz

After downloading the tool kit, decompress it to '/opt/risc-v' through the following command::

    $ sudo tar -zxvf toolchain_v5.2.1.tgz -C /


.. note::

    Tool chain the default path in the middleware/soc/bk7256/bk7256defconfig file definition, customers can configure::

        CONFIG_TOOLCHAIN_PATH="/opt/risc-v/nds32le-elf-mculib-v5/bin"

    The toolchain can also support relative path configuration, such as putting the toolchain in the sdk directory::

        CONFIG_TOOLCHAIN_PATH="toolchain_v5.2.1/nds32le-elf-mculib-v5/bin"


Program compilation depends on library installation
-----------------------------------------------------------------

Enter the following command in the terminal to install python\CMake\Ninja\crypto::

    sudo dpkg --add-architecture i386
    sudo apt-get update
    sudo apt-get install build-essential cmake python3 python3-pip doxygen ninja-build libc6:i386 libstdc++6:i386 libncurses5-dev lib32z1 -y
    sudo pip3 install pycryptodome click

Document Compilation Dependent Library Installation
------------------------------------------------------------------------------

Enter the following command on the terminal to install the python required for compiling the document::

    sudo pip3 install sphinx_rtd_theme future breathe blockdiag sphinxcontrib-seqdiag sphinxcontrib-actdiag sphinxcontrib-nwdiag sphinxcontrib.blockdiag


Set Python default version::

    sudo ln -s /usr/bin/python3 /usr/bin/python


Compilation project
------------------------------------

Enter the following command in the terminal to compile the Armino default project. PROJECT is an optional parameter, which is app by default. The default project is to start WiFi, BLE, initialize common drivers, and start the Armino default Cli program::

    cd ~/armino/bk_idk
    make bk7256

- BK7256 is a dual CPU core system, which is configured as dual core by default. When compiling the CPU0 system of BK7256 platform, CPU1 and CPU0 will be compiled automatically, and CPU0 and CPU1 systems will be packaged together



- The default project of BK7256 platform uses FreeRTOS V10.4 system::

    components/os_source/freertos_v10

- BK7256 platform supports Hongmeng OS V3.0.1 system and can compile Harmony project::

    cd ~/armino/bk_idk
    make bk7256 PROJECT=harmony



Configuration project
------------------------------------

- We can change the Armino default configuration item through menuconfig::

    cd ~/armino/bk_idk
    make menuconfig

- We can also directly use the project configuration file for differentiated configuration::

    Project Profile Override Chip Profile Override Default Configuration
    Example： bk7256/config >> bk7256.defconfig >> KConfig
    + Example of project configuration file：
        projects/app/config/bk7256/config
        projects/harmony/config/bk7256/config
    + Sample chip configuration file：
        middleware/soc/bk7256/bk7256.defconfig
    + Sample KConfig configuration file：
        middleware/arch/riscv/Kconfig
        components/bk_cli/Kconfig

- Important configuration instructions
    + The operating system is configured as FreeRTOS V10::

        #
        # FreeRTOS
        #
        CONFIG_FREERTOS=y
        # CONFIG_LITEOS_M is not set
        CONFIG_FREERTOS_V9=n
        CONFIG_FREERTOS_V10=y

    + The operating system is configured as Hongmeng OS::

        # LITEOS_M
        CONFIG_LITEOS_M_V3=y
        CONFIG_LITEOS_M_BK=y

        # FreeRTOS
        CONFIG_FREERTOS=n
        CONFIG_FREERTOS_V9=n
        CONFIG_FREERTOS_V10=n

- Use and Difference of Series Chip Macros

    + Macro CONFIG_SOC_BK7256XX represents BK7256 series::

        It belongs to BK7235/BK7237/BK7256 common chip macro. CPU1 also needs to define this macro
		collocation method：CONFIG_SOC_BK7256XX=y
		

    + Distinguish macros of the same series of chips (not used to distinguish other chips)::

		CONFIG_SOC_BK7256, CPU1 needs to define the macro and CONFIG_ SLAVE_ CORE Combination Differentiation BK7256_ CPU1
		collocation method：CONFIG_SOC_BK7256=y
		

    + Macro of string series chips (used to distinguish other chips during compilation)::

         cpu0/cpu1 distinguish：
		 CPU0：CONFIG_SOC_STR="bk7256"
		 CPU1：CONFIG_SOC_STR="bk7256_cp1"


    + Dual core CPU0, CPU1 related macro differentiation (for code differentiation)::

        CONFIG_DUAL_CORE            #Dual core function
        CONFIG_MASTER_CORE          #Depends on CONFIG_ DUAL_ CORE, the difference between CPU0 and CPU1
        CONFIG_SLAVE_CORE           #Depends on CONFIG_ DUAL_ CORE, the difference between CPU0 and CPU1
        Single core: none of the above three macros is defined

- Modules Excutes In Which CPUx

    + BK7256 has dual cores and software uses AMP arch,CPU0 and CPU1 software are compliled dependence,
      but CPU0 and CPU1 uses the same SDK,so some modules need to defined to excecut in which CPUx.
      I.E:There is a TRNG(Random Data Module) module in BK7256,if uses dual-core arch,the TRNG should
      be set excute in which CPUx with mutual exclusion.
      If CPU0 needs TRNG module, it should set CONFIG_TRNG=y in bk7256.defconfig(or bk7256*.config and so on),
      and set CONFIG_TRNG=n in bk7256_cp1.defconfig(or bk7256_cp1*.config and so on).

    + Example codes::

         #if CONFIG_TRNG             #Uses module macro to seperate CPUx software whether enable TRNG
         #include "driver/trng.h"
         #endif
         ...
         #if CONFIG_TRNG             #Uses module macro to seperate CPUx software whether enable TRNG
         bk_rand();
         #endif

New project
------------------------------------

The default project is projects/app. For new projects, please refer to projects/harmony project


Burn Code
------------------------------------

On the Windows platform, Armino currently supports UART burning.



Burn through serial port
****************************************

!note:

    Armino supports UART burning. It is recommended to use the CH340 serial port tool board to download.

Serial port burning tool is shown in the figure below:

.. figure:: ../../_static/download_tool_uart.png
    :align: center
    :alt: Uart
    :figclass: align-center

    UART

Acquisition of burning tools：

	http://dl.bekencorp.com/tools/flash/
	Get the latest version in this directory. Ex：BEKEN_BKFIL_V2.1.6.0_20231123.zip

BKFIL.exe The interface and related configurations are shown in the figure below：

.. figure:: ../../_static/download_uart_bk7256_en.png
    :align: center
    :alt: BKFIL GUI
    :figclass: align-center

    BKFIL GUI


Burn the serial port UART1, click "" Burn "" to burn the version, and then power down and restart the device after burning.


Serial port Log and Command Line
------------------------------------

- At present, on the BK7256 platform, the serial port Log and Command Line commands are input on the UART1 port; You can view the list of supported commands through the help command;
- The log of CPU 1 is also output through the UART1 serial port of CPU 0, and the log of CPU 1 is marked with "cpu 1";
- Command Line of CPU1 can be executed through UART1 of CPU0, such as:

    Cpu1 help//Output the command list of cpu1

    Cpu1 time//Output the current running time of cpu1


Compile options and link options
------------------------------------------------------

- BK7256, with default compile option "-mstrict-align", link option "-wl,--defsym,memcpy=memcpy_ss"
- To compile the lib library separately, you need to add the compile option "-mstrict-align".
- If you do not use platform linking commands, such as compiling HarmonyOS, for Andes v5.1.1 tool chain, you need to add the link option "-wl,--defsym,memcpy=memcpy_ss".

