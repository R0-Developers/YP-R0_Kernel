YP-R0_Kernel
============

#COMPILE
export PATH="/usr/lib/lightdm/lightdm:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/arm-2008q1/bin"
export CROSS_COMPILE=arm-none-eabi-

#TOOLCHAIN
arm-2008q1.tar.gz

#REMOVE SAMSUNG LOGO:
/home/pietro/Dev/R0/kernel/linux-2.6.24-2.4.2-base/drivers/video/mxc/
SOVRASCRIVERE IL FILE mxcfb_smd_qvga.c CON QUELLO IN QUESTA DIR
