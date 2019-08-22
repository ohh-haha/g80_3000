# g80-3000

![g80-3000](imgur.com image replace me!)

使用atmega32u4  和 两个HC4067 替换g80_3000 本身的主控。
其中行为8 列为16  因此 左Menu键和 右Fn键 和小键盘的\*无法使用。
其中列的对应关系为  
S0  => D2
S1  => D3
S2  => D4
S3  => D5
SIN => D6

其中行的对应关系为
S0  => D8
S1  => D9
S2  => D10
S3  => D14
SIN  => D15

Keyboard Maintainer: [ftfunjth](https://github.com/yourusername)  
Hardware Supported: The PCBs, controllers supported  
Hardware Availability: links to where you can find this hardware

Make example for this keyboard (after setting up your build environment):

    make g80-3000:default

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).
