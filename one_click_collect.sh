#!/bin/bash
export PATH="$PATH:./"

log_collect_file_path="http://mirrors.tencentyun.com/install/cts/linux/log_collect.zip"

ping -c 1 mirrors.tencentyun.com > /dev/null 2>&1
if [ $? -ne 0 ];then
    log_collect_file_path="http://169.254.0.3/install/cts/linux/log_collect.zip"
fi

sudo_operation(){
    sudo echo "[2]Unzip Tool..."
    sudo unzip -o log_collect.zip >> /dev/null
    sudo echo "[3]Runing Tool..."
    cd log_collect
    sudo ./log_collect.sh
    cd ..
    sudo echo "[4]Clearing Env..."
    sudo rm -rf log_collect.zip
    sudo rm -rf log_collect
    sudo echo "[5]done"

}
operation(){
    echo "[2]Unzip Tool..."
    unzip -o log_collect.zip >> /dev/null
    echo "[3]Runing Tool..."
    cd log_collect
    ./log_collect.sh
    cd ..
    echo "[4]Clearing Env..."
    rm -rf log_collect.zip
    rm -rf log_collect
    echo "[5]done"
}


main(){
#判断是否存在wget、curl
check_wget=`which wget`
if [ $? -ne 0 ];then
    echo "wget command not found!"
    check_curl=`which curl`
    if [ $? -ne 0 ];then
        echo "curl command not found!"
        exit 1
    else
        link_cmd="curl"
    fi
else
    link_cmd="wget"
fi
#判断是否为物理产品
if [ -f /etc/uuid ];then 
    if [ x"$link_cmd" == x"wget" ];then
        if [ `whoami` != "root" ];then
            if [ `whoami` = "ubuntu" ];then
                sudo echo "[1]Downloading Tool..."
                sudo wget -q  $log_collect_file_path >> /dev/null
                sudo_operation
            else
                sudo echo " only root can run me"
                sudo exit 1
            fi 
        else
            echo "[1]Downloading Tool..."
            wget -q  $log_collect_file_path >> /dev/null
            operation
        fi
    else
        if [ `whoami` != "root" ];then
            if [ `whoami` = "ubuntu" ];then
                sudo echo "[1]Downloading Tool..."
                sudo curl -O  $log_collect_file_path >> /dev/null
                sudo_operation
            else
                sudo echo " only root can run me"
                sudo exit 1
            fi 
        else
            echo "[1]Downloading Tool..."
            curl -O  $log_collect_file_path >> /dev/null
            operation
        fi
    fi
else
    #判断是否存在namespace问题
    if [[ ! -d  "/var/run/netns/" ]]; then
        has_namespace="no"
        netns_prefix=""
        elif [[ -L "/var/run/netns/default" ]]; then
            has_namespace="yes"
            netns_prefix="ip netns exec default "
        else
        has_namespace="error"
        report 1 "neither [no mgt] nor [default namespace] exists!"
        exit 1
    fi
    if [ x"$link_cmd" == x"wget" ];then
        if [ `whoami` != "root" ];then
            if [ `whoami` = "ubuntu" ];then
                sudo echo "[1]Downloading Tool..."
                sudo $netns_prefix wget -q  $log_collect_file_path >> /dev/null
                sudo_operation
            else
                sudo echo " only root can run me"
                sudo exit 1
            fi 
        else
            echo "[1]Downloading Tool..."
            $netns_prefix wget -q $log_collect_file_path >> /dev/null
            operation
        fi
    else
        if [ `whoami` != "root" ];then
            if [ `whoami` = "ubuntu" ];then
                sudo echo "[1]Downloading Tool..."
                sudo $netns_prefix curl -O $log_collect_file_path >> /dev/null
                sudo_operation
            else
                sudo echo " only root can run me"
                sudo exit 1
            fi 
        else
            echo "[1]Downloading Tool..."
            $netns_prefix curl -O $log_collect_file_path >> /dev/null
            operation
        fi
    fi
fi
}


#判断是否是静态收集
arg1=$1
arg2=$2
case $arg1 in
    "--quiet")
            case $arg2 in
                    "--all")
            if [ `whoami` != "root" ];then
                    echo " only root can run me"
                    exit 1
            fi
                            if [ -f /etc/centos-release ] ;then
                    sosreport
            elif [ -f /etc/lsb-release ] ;then
                    sosreport
                    [ $? -ne 0 ] && echo "not support Ubuntu14 temporarily, you should install sosreport first by running 'apt-get install sosreport'"
            elif [[ -f /etc/os-release && `cat /etc/os-release | grep -iw debian` ]];then
                    echo "doesn't support debian"
            elif [[ -f /etc/os-release && `cat /etc/os-release | grep -i suse` ]];then
                    echo "doesn't support opensuse"
            elif [ -f /etc/SuSE-release ] ;then
                    supportconfig
            fi
            
                            exit 0
                    ;;
                    *)
                            echo "'./one_click_collect.sh --quiet': unrecognized option in" $arg2
                            echo "Try './one_click_collect.sh --help' for more information."
                            exit 0
                            ;;
            esac
            ;;
    "--help")
            echo "Usage: ./one_click_collect [OPTION]... [OPTION]..."
            echo "collect system log for troubleshooting(miniom log collecting by default)."
            echo ""
            echo "  --quiet     collect log in quiet mode"
            echo "          --all   collect all system log in quiet mode by running './one_click_collect.sh --quiet --all'"
            exit 0
            ;;
    "")
    main
            ;;
    *)
            echo "'./one_click_collect.sh ': unrecognized option in" $arg1
            echo "Try './one_click_collect.sh --help' for more information."
            exit 0
            ;;
esac

