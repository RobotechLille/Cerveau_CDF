
reconfigure: buildroot/.config

buildroot/.config:
	make -C buildroot BR2_EXTERNAL=.. cdf2017p_defconfig

compile: buildroot/.config
	make -C buildroot

.principal-ip:
	@echo "Quelle est l'IP du Robot principal ?"
	@read ip; echo $$ip > $@

ssh: .principal-ip
	ssh -l root -i id_rsa $$(cat .principal-ip)

reboot: .principal-ip
	ssh -l root -i id_rsa $$(cat .principal-ip) /sbin/reboot

upgrade: compile .principal-ip
	@# TODO C'est horriiiiiiible de faire ça comme ça ^^
	rsync -e 'ssh -l root -i id_rsa' -a --chown root:root buildroot/output/target/ $$(cat .principal-ip):/

flash: compile
	dd if=buildroot/output/images/sdcard.img of=/dev/mmcblk0 bs=4M

.PHONY: compile upload reboot
