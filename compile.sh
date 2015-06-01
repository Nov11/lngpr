#param='ARCH=mips CROSS_COMPILE=mips64el-linux-'
make -C /lib/modules/3.10.0-2015.03.17.fc19.mipsel/build/ M=`pwd` ${1:- modules}
