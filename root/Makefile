
configure: buildroot/.config

clean:
	make -C buildroot clean

buildroot/.config: configs/cdf2017p_defconfig
	make -C buildroot BR2_EXTERNAL=.. cdf2017p_defconfig

compile: buildroot/.config
	make -C buildroot

recompile: clean compile

# ssh-keygen -t ecdsa -C principal@robotech -f sshkey -N ''
sshconf: principalconf.sh
	source $$PWD/$<; echo -e "Host principal p\n    User root\n    Hostname $$IP\n    PreferredAuthentications publickey\n    PubkeyAuthentication yes\n    IdentityFile \"$$PWD/sshkey\"" > "$@"
	source $$PWD/$<; echo -e "$$SSHCPRV" > sshkey
	chmod 600 sshkey

ssh: sshconf
	ssh -F sshconf principal

reboot: sshconf
	ssh -F sshconf principal /sbin/reboot

date: sshconf
	ssh -F sshconf principal "date -s $(date +%Y%m%d%H%M.%S)"

upgrade: sshconf
	make -C buildroot target-finalize
	@# TODO C'est horriiiiiiible de faire ça comme ça ^^
	rsync -e 'ssh -F sshconf' -a --chown root:root buildroot/output/target/ principal:/

flash: compile
	dd if=buildroot/output/images/sdcard.img of=/dev/mmcblk0 bs=4M

.PHONY: compile upload reboot

