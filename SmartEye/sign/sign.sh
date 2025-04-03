#!/bin/bash
root=$1
origin=$2
output=$3
echo '$1='$1
echo '$2='$2
echo '$3='$3
java -jar ${root}/sign/signapk.jar ${root}/sign/platform.x509.pem ${root}/sign/platform.pk8 ${origin} ${output}