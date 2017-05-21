#!/bin/sh

if file ${BR2_EXTERNAL_CDF_PATH}/principalconf.sh &> /dev/null
then
    source ${BR2_EXTERNAL_CDF_PATH}/principalconf.sh

    # SSH configuration
    rm ${TARGET_DIR}/etc/dropbear &> /dev/null
    mkdir -p ${TARGET_DIR}/etc/dropbear/
    echo "$SSHSPRV" | base64 -d > ${TARGET_DIR}/etc/dropbear/dropbear_ecdsa_host_key
    mkdir -p ${TARGET_DIR}/root/.ssh/
    echo "$SSHCPUB" > ${TARGET_DIR}/root/.ssh/authorized_keys
else
    echo "Récupérez le fichier principalconf.sh pour pouvoir vous connecter au vrai robot !"
fi

dd if=/dev/urandom of=${TARGET_DIR}/etc/random-seed bs=512 count=1

