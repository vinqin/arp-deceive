# arp-deceive
Deceive gateway of lan using gratuitous arp reply.

> Require: 
>
>  	1. cmake3.5+
>  	2. libnet.so

## Usage
[Ubuntu]

```bash
$ # install cmake and libnet.so
$ sudo apt-get update
$ sudo apt-get install cmake -y
$ sudo apt-get install libnet-dev -y
$ 
$ # compile
$ cmake .
$ make
$
$ # execute below command after you have configurated the arp.conf
$ sudo ./arp_deceive
```

[CentOS]
1. Install cmake and libnet.so
2. Modify CMakeLists.txt in which you should specify where the libnet.so is located.
3. Compile
4. Execute

