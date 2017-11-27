又买了俩开发板  

一个是MicoKit3165,wiki在developer.mico.io/handbooks/5  
这个库是示例程序的SDK跟源码什么的developer.mico.io/developer/md/bWljby1oYW5kYm9vay9Eb3dubG9hZC85Lk1pQ09fRW5qb3lfU0RLLm1k  

示例程序叫Mico总动员  

内容：

1,device_sdk：MiCO 设备端应用程序源码，和 连接 FogCloud 的库组件；  
2,app_sdk: 基于APICloud平台开发的 MiCO总动员APP_V2 源码;  
3,apicloud_mico2_zip： 基于mico的自定义模块组件；  
4,使用说明：MiCO设备端及APP端连接到FogCloudV2.0的具体步骤说明。  

DYNMB  

2017年10月30日14:26:51  

  #Add 2017年11月27日11:35:53 mico.io这个域名废了,现在叫developer.mxchip.com

继续  
2017年11月23日14:50:00  

嵌入式端 FOG 库版本:FOG2.4.0  
MQTT 组件库版本:0.1.8  
IDE:MiCoder  
依赖 MiCOSDK:MiCO SDK 3.1  

头疼了很久的依赖问题,通过virtualenv解决  


<pre>
    <code>
senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY$ virtualenv -p /usr/bin/python venv
Running virtualenv with interpreter /usr/bin/python
New python executable in /home/senrsl/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY/venv/bin/python
Installing setuptools, pip, wheel...done.
senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY$ source venv/bin/activate
(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY$ pip --version
pip 9.0.1 from /home/senrsl/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY/venv/local/lib/python2.7/site-packages (python 2.7)
(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY$ pip install mico-cube
Collecting mico-cube
  Downloading mico-cube-1.0.17.tar.gz
Building wheels for collected packages: mico-cube
  Running setup.py bdist_wheel for mico-cube ... done
  Stored in directory: /home/senrsl/.cache/pip/wheels/a8/62/26/9b32ed19edc54228ced8c65a9fc411708f020b18de492cdcda
Successfully built mico-cube
Installing collected packages: mico-cube
Successfully installed mico-cube-1.0.17
(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY$ mico --version
1.0.17
(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY$ pip install --upgrade mico-cube
Requirement already up-to-date: mico-cube in ./venv/lib/python2.7/site-packages
(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY$ mico config --global MICODER ~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY/MiCoder
[mico] /home/senrsl/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY/MiCoder now set as global MICODER
(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY$ mico config --list
[mico] Global config:
MICODER=/home/senrsl/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY/MiCoder

[mico] Local config (/home/senrsl/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY):
Couldn't find valid mico program in /home/senrsl/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY
(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY$ mico import https://code.aliyun.com/dong_M/helloworld.git
[mico] Importing program "helloworld" from "https://code.aliyun.com/dong_M/helloworld.git" at latest revision in the current branch
[mico] Adding component "mico-os" from "https://code.aliyun.com/mico/mico-os.git" at rev #01092eb3610c



# 2017年11月23日19:13:33

(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY$ cd helloworld/
(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY/helloworld$ mico make helloworld@MK3165
Making config file for first time
processing components: helloworld MK3165 FreeRTOS LwIP wolfSSL MiCO
MiCO core based on pre-build library: ===MiCO.3165.GCC.a=== 
Skipping building bootloader due to "total" is not set

Compiling App_Helloworld
Compiling Board_MK3165
Compiling FreeRTOS
Compiling LwIP
Compiling Lib_SPI_Flash_Library_MK3165
Compiling Lib_MiCOKit_EXT_MK3165
Compiling MiCO_FreeRTOS_Interface
Compiling Lib_MiCO_Kernel
Compiling Lib_MiCO_System
Compiling Lib_MiCO_Security
Compiling Lib_Utilities
Compiling STM32F4xx
Compiling Lib_VGM128064_MK3165
Compiling Lib_gpio_button_MK3165
Compiling Lib_dc_motor_MK3165
Compiling Lib_RGB_P9813_MK3165
Compiling Sensor_BME280_MK3165
Compiling Sensor_DHT11_MK3165
Compiling Sensor_APDS9930_MK3165
Compiling Sensor_Light_adc_MK3165
Compiling Sensor_Infrared_Reflective_MK3165
Compiling Lib_MiCO_System_QC
Compiling Lib_JSON_C
Compiling GCC
Compiling STM32F4xx_Peripheral_Drivers
Compiling Lib_MFi_Auth_MK3165
Compiling STM32F4xx_Peripheral_Libraries
Making build/helloworld@MK3165/libraries/App_Helloworld.a
Making build/helloworld@MK3165/libraries/Board_MK3165.a
Making build/helloworld@MK3165/libraries/FreeRTOS.a
Making build/helloworld@MK3165/libraries/LwIP.a
Making build/helloworld@MK3165/libraries/Lib_SPI_Flash_Library_MK3165.a
Making build/helloworld@MK3165/libraries/Lib_MiCOKit_EXT_MK3165.a
Making build/helloworld@MK3165/libraries/MiCO_FreeRTOS_Interface.a
Making build/helloworld@MK3165/libraries/Lib_MiCO_Kernel.a
Making build/helloworld@MK3165/libraries/Lib_MiCO_System.a
Making build/helloworld@MK3165/libraries/Lib_MiCO_Security.a
Making build/helloworld@MK3165/libraries/Lib_Utilities.a
Making build/helloworld@MK3165/libraries/STM32F4xx.a
Making build/helloworld@MK3165/libraries/Lib_VGM128064_MK3165.a
Making build/helloworld@MK3165/libraries/Lib_gpio_button_MK3165.a
Making build/helloworld@MK3165/libraries/Lib_dc_motor_MK3165.a
Making build/helloworld@MK3165/libraries/Lib_RGB_P9813_MK3165.a
Making build/helloworld@MK3165/libraries/Sensor_BME280_MK3165.a
Making build/helloworld@MK3165/libraries/Sensor_DHT11_MK3165.a
Making build/helloworld@MK3165/libraries/Sensor_APDS9930_MK3165.a
Making build/helloworld@MK3165/libraries/Sensor_Light_adc_MK3165.a
Making build/helloworld@MK3165/libraries/Sensor_Infrared_Reflective_MK3165.a
Making build/helloworld@MK3165/libraries/Lib_MiCO_System_QC.a
Making build/helloworld@MK3165/libraries/Lib_JSON_C.a
Making build/helloworld@MK3165/libraries/GCC.a
Making build/helloworld@MK3165/libraries/STM32F4xx_Peripheral_Drivers.a
Making build/helloworld@MK3165/libraries/Lib_MFi_Auth_MK3165.a
Making build/helloworld@MK3165/libraries/STM32F4xx_Peripheral_Libraries.a
Making helloworld@MK3165.elf

Making helloworld@MK3165.bin
Making helloworld@MK3165.hex

                        MICO MEMORY MAP                            
|=================================================================|
| MODULE                                   | ROM       | RAM      |
|=================================================================|
| App_Helloworld                           | 141       | 0        |
| aws.Cortex-M4.GCC.release                | 20        | 4        |
| Board_MK3165                             | 1192      | 120      |
| FreeRTOS                                 | 5968      | 432      |
| GCC                                      | 204       | 20       |
| libc                                     | 42755     | 2268     |
| libgcc                                   | 4636      | 0        |
| Lib_gpio_button_MK3165                   | 224       | 200      |
| Lib_JSON_C                               | 6762      | 12       |
| Lib_MiCO_Kernel                          | 12        | 12       |
| Lib_MiCO_System                          | 31518     | 487      |
| Lib_MiCO_System_QC                       | 1015      | 16       |
| Lib_SPI_Flash_Library_MK3165             | 942       | 0        |
| Lib_Utilities                            | 9027      | 0        |
| Lib_wolfSSL.Cortex-M4.GCC.release        | 14867     | 8        |
| LwIP                                     | 112       | 0        |
| MiCO.3165.GCC                            | 106403    | 6857     |
| MiCO_FreeRTOS_Interface                  | 3003      | 40       |
| STM32F4xx                                | 5648      | 2215     |
| STM32F4xx_Peripheral_Drivers             | 9444      | 252      |
| STM32F4xx_Peripheral_Libraries           | 5410      | 16       |
| *fill*                                   | 238       | 925      |
|=================================================================|
| TOTAL (bytes)                            | 249541    | 13884    |
|=================================================================|
Build complete
Making .gdbinit
Making .openocd_cfg
(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY/helloworld$ 



    </code>
</pre>


如果提示

<pre><code>
 (venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY$ mico import https://code.aliyun.com/dong_M/mico-demos.git
[mico] Importing program "mico-demos" from "https://code.aliyun.com/dong_M/mico-demos.git" at latest revision in the current branch
[mico] Adding component "mico-os" from "https://code.aliyun.com/dong_M/mico-os.git" at rev #cbdd3bdc329c
[mico] ERROR: Unknown Error: [Errno 2] No such file or directory: '/home/senrsl/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY/mico-demos/mico-os/makefiles/eclipse_project/Linux64/.project'
</code></pre>

那就把项目全删了,重新下载  

一开始helloworld下载提示这个找不到,编译一堆问题,重新下了下,不提示这个问题了,直接就编译成功了....  

下月初的香港酒店怎么这么贵.....   

顺丰珠海寄北京,三天了还没出珠海....  

搞了半天,往板子上烧还需要买个JLink,淘宝108买个JLink,北京普通快递全都叫停了,又花20多买了个顺丰.....  


JLink v9买到了....

然后JLink 去连接设备

<pre><code>
senrsl@senrsl-T540p:/opt/SEGGER/JLink$ ./JLinkExe 
SEGGER J-Link Commander V6.22 (Compiled Nov 24 2017 18:35:58)
DLL version V6.22, compiled Nov 24 2017 18:35:53

Connecting to J-Link via USB...O.K.
Firmware: J-Link V9 compiled Nov 23 2017 16:41:31
Hardware version: V9.40
S/N: 59425868
License(s): RDI, GDB, FlashDL, FlashBP, JFlash, RDDI
VTref = 3.266V


Type "connect" to establish a target connection, '?' for help
J-Link>connect
Please specify device / core. <Default>: Unspecified
Type '?' for selection dialog
Device>STM32F411CE
Please specify target interface:
  J) JTAG (Default)
  S) SWD
TIF>S
Specify target interface speed [kHz]. <Default>: 4000 kHz
Speed>
Device "STM32F411CE" selected.


Connecting to target via SWD
Found SW-DP with ID 0x2BA01477
Found SW-DP with ID 0x2BA01477
Scanning AP map to find all available APs
AP[1]: Stopped AP scan as end of AP map has been reached
AP[0]: AHB-AP (IDR: 0x24770011)
Iterating through AP map to find AHB-AP to use
AP[0]: Core found
AP[0]: AHB-AP ROM base: 0xE00FF000
CPUID register: 0x410FC241. Implementer code: 0x41 (ARM)
Found Cortex-M4 r0p1, Little endian.
FPUnit: 6 code (BP) slots and 2 literal slots
CoreSight components:
ROMTbl[0] @ E00FF000
ROMTbl[0][0]: E000E000, CID: B105E00D, PID: 000BB00C SCS
ROMTbl[0][1]: E0001000, CID: B105E00D, PID: 003BB002 DWT
ROMTbl[0][2]: E0002000, CID: B105E00D, PID: 002BB003 FPB
ROMTbl[0][3]: E0000000, CID: B105E00D, PID: 003BB001 ITM
ROMTbl[0][4]: E0040000, CID: B105900D, PID: 000BB9A1 TPIU
ROMTbl[0][5]: E0041000, CID: B105900D, PID: 000BB925 ETM
Cortex-M4 identified.
J-Link>usb
Disconnecting from J-Link...O.K.
Disconnecting from J-Link...O.K.
Connecting to J-Link via USB...O.K.
Firmware: J-Link V9 compiled Nov 23 2017 16:41:31
Hardware version: V9.40
S/N: 59425868
License(s): RDI, GDB, FlashDL, FlashBP, JFlash, RDDI
VTref = 3.261V
Device "STM32F411CE" selected.


Connecting to target via SWD
Found SW-DP with ID 0x2BA01477
Found SW-DP with ID 0x2BA01477
Scanning AP map to find all available APs
AP[1]: Stopped AP scan as end of AP map has been reached
AP[0]: AHB-AP (IDR: 0x24770011)
Iterating through AP map to find AHB-AP to use
AP[0]: Core found
AP[0]: AHB-AP ROM base: 0xE00FF000
CPUID register: 0x410FC241. Implementer code: 0x41 (ARM)
Found Cortex-M4 r0p1, Little endian.
FPUnit: 6 code (BP) slots and 2 literal slots
CoreSight components:
ROMTbl[0] @ E00FF000
ROMTbl[0][0]: E000E000, CID: B105E00D, PID: 000BB00C SCS
ROMTbl[0][1]: E0001000, CID: B105E00D, PID: 003BB002 DWT
ROMTbl[0][2]: E0002000, CID: B105E00D, PID: 002BB003 FPB
ROMTbl[0][3]: E0000000, CID: B105E00D, PID: 003BB001 ITM
ROMTbl[0][4]: E0040000, CID: B105900D, PID: 000BB9A1 TPIU
ROMTbl[0][5]: E0041000, CID: B105900D, PID: 000BB925 ETM
Cortex-M4 identified.
J-Link>r
Reset delay: 0 ms
Reset type NORMAL: Resets core & peripherals via SYSRESETREQ & VECTRESET bit.
Reset: Halt core after reset via DEMCR.VC_CORERESET.
Reset: Reset device via AIRCR.SYSRESETREQ.
J-Link>r
Reset delay: 0 ms
Reset type NORMAL: Resets core & peripherals via SYSRESETREQ & VECTRESET bit.
Reset: Halt core after reset via DEMCR.VC_CORERESET.
Reset: Reset device via AIRCR.SYSRESETREQ.
J-Link>usb
Disconnecting from J-Link...O.K.
Disconnecting from J-Link...O.K.
Connecting to J-Link via USB...O.K.
Firmware: J-Link V9 compiled Nov 23 2017 16:41:31
Hardware version: V9.40
S/N: 59425868
License(s): RDI, GDB, FlashDL, FlashBP, JFlash, RDDI
VTref = 3.287V
Device "STM32F411CE" selected.


Connecting to target via SWD
Found SW-DP with ID 0x2BA01477
Found SW-DP with ID 0x2BA01477
Scanning AP map to find all available APs
AP[1]: Stopped AP scan as end of AP map has been reached
AP[0]: AHB-AP (IDR: 0x24770011)
Iterating through AP map to find AHB-AP to use
AP[0]: Core found
AP[0]: AHB-AP ROM base: 0xE00FF000
CPUID register: 0x410FC241. Implementer code: 0x41 (ARM)
Found Cortex-M4 r0p1, Little endian.
FPUnit: 6 code (BP) slots and 2 literal slots
CoreSight components:
ROMTbl[0] @ E00FF000
ROMTbl[0][0]: E000E000, CID: B105E00D, PID: 000BB00C SCS
ROMTbl[0][1]: E0001000, CID: B105E00D, PID: 003BB002 DWT
ROMTbl[0][2]: E0002000, CID: B105E00D, PID: 002BB003 FPB
ROMTbl[0][3]: E0000000, CID: B105E00D, PID: 003BB001 ITM
ROMTbl[0][4]: E0040000, CID: B105900D, PID: 000BB9A1 TPIU
ROMTbl[0][5]: E0041000, CID: B105900D, PID: 000BB925 ETM
Cortex-M4 identified.
J-Link>r
Reset delay: 0 ms
Reset type NORMAL: Resets core & peripherals via SYSRESETREQ & VECTRESET bit.
Reset: Halt core after reset via DEMCR.VC_CORERESET.
Reset: Reset device via AIRCR.SYSRESETREQ.
J-Link>usb
Disconnecting from J-Link...O.K.
Disconnecting from J-Link...O.K.
Connecting to J-Link via USB...O.K.
Firmware: J-Link V9 compiled Nov 23 2017 16:41:31
Hardware version: V9.40
S/N: 59425868
License(s): RDI, GDB, FlashDL, FlashBP, JFlash, RDDI
VTref = 3.279V
Device "STM32F411CE" selected.


Connecting to target via SWD
Found SW-DP with ID 0x2BA01477
Found SW-DP with ID 0x2BA01477
Scanning AP map to find all available APs
AP[1]: Stopped AP scan as end of AP map has been reached
AP[0]: AHB-AP (IDR: 0x24770011)
Iterating through AP map to find AHB-AP to use
AP[0]: Core found
AP[0]: AHB-AP ROM base: 0xE00FF000
CPUID register: 0x410FC241. Implementer code: 0x41 (ARM)
Found Cortex-M4 r0p1, Little endian.
FPUnit: 6 code (BP) slots and 2 literal slots
CoreSight components:
ROMTbl[0] @ E00FF000
ROMTbl[0][0]: E000E000, CID: B105E00D, PID: 000BB00C SCS
ROMTbl[0][1]: E0001000, CID: B105E00D, PID: 003BB002 DWT
ROMTbl[0][2]: E0002000, CID: B105E00D, PID: 002BB003 FPB
ROMTbl[0][3]: E0000000, CID: B105E00D, PID: 003BB001 ITM
ROMTbl[0][4]: E0040000, CID: B105900D, PID: 000BB9A1 TPIU
ROMTbl[0][5]: E0041000, CID: B105900D, PID: 000BB925 ETM
Cortex-M4 identified.
J-Link>

</code></pre>

连接上之后 执行r命令开发板灯灭,执行usb命令开发板灯亮....



然后接着下载....

<pre><code>
(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY/helloworld$ mico help
usage: mico [-h] [--version]             ...

Code management tool for MXCHIP MiCO OS - https://code.aliyun.com/mico/mico-os
version 1.0.17

Use 'mico <command> -h|--help' for detailed help.
Online manual and guide available at https://code.aliyun.com/mico/mico-cube

optional arguments:
  -h, --help   show this help message and exit
  --version    print version number and exit

Commands:
             
    new        Create new mico program or component
    import     Import program from URL
    add        Add component from URL
    remove     Remove component
    deploy     Find and add missing components and source codes
    codes      Import the optional component from the remote repository
    publish    Publish program or component
    update     Update to branch, tag, revision or latest
    sync       Synchronize mico component references
    ls         View dependency tree
    status     Show version control status
               
    make       Make mico program/component
    makelib    Compile static library
               
    config     Tool configuration
               
    help       This help screen
(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY/helloworld$ 

</code></pre>

特么的,原来往里烧东西需要改开关....
官方FAQ群里问了好几天没人告诉.......

主板上有俩开关
SW1(BOOT) SW2(STATUS)     MODE
OFF(1)     OFF(1)         Application(自动连接网络,并进入透明传输模式)
OFF(1)     ON(0)          Application(not used)(命令控制模式)
ON(0)      OFF(1)         Bootloader(固件升级模式 FW UPDATE)
ON(0)      ON(0)          MFG(生产测试模式)

特么的,就一个开关,告诉一下会死啊.....

<pre><code>
(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY/helloworld$ mico make helloworld@MK3165 download
Skipping building bootloader due to "total" is not set


                        MICO MEMORY MAP                            
|=================================================================|
| MODULE                                   | ROM       | RAM      |
|=================================================================|
| App_Helloworld                           | 141       | 0        |
| aws.Cortex-M4.GCC.release                | 20        | 4        |
| Board_MK3165                             | 1192      | 120      |
| FreeRTOS                                 | 5968      | 432      |
| GCC                                      | 204       | 20       |
| libc                                     | 42755     | 2268     |
| libgcc                                   | 4636      | 0        |
| Lib_gpio_button_MK3165                   | 224       | 200      |
| Lib_JSON_C                               | 6762      | 12       |
| Lib_MiCO_Kernel                          | 12        | 12       |
| Lib_MiCO_System                          | 31518     | 487      |
| Lib_MiCO_System_QC                       | 1015      | 16       |
| Lib_SPI_Flash_Library_MK3165             | 942       | 0        |
| Lib_Utilities                            | 9027      | 0        |
| Lib_wolfSSL.Cortex-M4.GCC.release        | 14867     | 8        |
| LwIP                                     | 112       | 0        |
| MiCO.3165.GCC                            | 106403    | 6857     |
| MiCO_FreeRTOS_Interface                  | 3003      | 40       |
| STM32F4xx                                | 5648      | 2215     |
| STM32F4xx_Peripheral_Drivers             | 9444      | 252      |
| STM32F4xx_Peripheral_Libraries           | 5410      | 16       |
| *fill*                                   | 238       | 925      |
|=================================================================|
| TOTAL (bytes)                            | 249541    | 13884    |
|=================================================================|
Downloading Bootloader ...
Skipping download bootloader due to "total" is not set

Downloading application to partition: 2 size: 252440 bytes...
Download complete

Build complete
Making .gdbinit
Making .openocd_cfg
(venv) senrsl@senrsl-T540p:~/test/iot/MicoKit3165/Fog_v2_MiCOKIT_ENJOY/helloworld$ 

</code></pre>

烧入helloworld之后,主板上那个绿灯一直闪,扩展板不亮.

然后usb连串口,开串口工具

点侧边那个RESET按钮,就会输出helloworld.....

<pre><code>
Welcome to minicom 2.7

OPTIONS: I18n 
Compiled on Jan  1 2014, 17:13:19.
Port /dev/ttyUSB0, 15:25:39

Press CTRL-A Z for help on special keys

[0][RTOS: mico_rtos_common.c:  84] Started MiCO RTOS interface for FreeRTOS v9.0
[25][Platform: mico_platform_common.c:  98] Platform initialised, build by GNUC
[607][SYSTEM: system_misc.c: 226] Free memory 94272 bytes
[614][SYSTEM: system_misc.c: 232] Kernel version: 31621002.052
[619][SYSTEM: system_misc.c: 235] MiCO version: 3.6.0
[624][SYSTEM: system_misc.c: 237] Wi-Fi driver version wl0: Sep 10 2014 11:28:4C
[637][SYSTEM: config_server.c: 154] Config Server established at port: 8000, fd1
[836][helloworld: helloworld.c:  43] Hello world!


-----------------------------------------------

Welcome to minicom 2.7

OPTIONS: I18n 
Compiled on Jan  1 2014, 17:13:19.
Port /dev/ttyUSB0, 15:47:19

Press CTRL-A Z for help on special keys

[0][RTOS: mico_rtos_common.c:  84] Started MiCO RTOS interface for FreeRTOS v9.0
[25][Platform: mico_platform_common.c:  98] Platform initialised, build by GNUC
[604][SYSTEM: system_misc.c: 226] Free memory 94272 bytes
[611][SYSTEM: system_misc.c: 232] Kernel version: 31621002.052
[616][SYSTEM: system_misc.c: 235] MiCO version: 3.6.0
[621][SYSTEM: system_misc.c: 237] Wi-Fi driver version wl0: Sep 10 2014 11:28:4C
[634][SYSTEM: config_server.c: 154] Config Server established at port: 8000, fd1
[833][helloworld: helloworld.c:  43] Hello world!Fuck World!


</code></pre>


Hello World 终于调通了......


之前找到的烧固件流程:
必须要先 把串口线跟JLink线断电，然后改开关，然后插串口线跟JLink线通电，然后烧固件，然后 拔串口跟JLink线断电，然后改开关，然后插串口线跟JLink线。。。。。

新发现流程:
调开关,按RESET键,就起作用了....






