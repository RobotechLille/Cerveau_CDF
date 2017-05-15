
compile:
	make -C buildroot BR2_EXTERNAL=.. cdf2017p_defconfig
	make -C buildroot

.principal-ip:
	@echo "Quelle est l'IP du Robot principal ?"
	@read ip; echo $$ip > $@

ssh: .principal-ip
	ssh -l root -i id_rsa $$(cat .principal-ip)

reboot: .principal-ip
	ssh -l root -i id_rsa $$(cat .principal-ip) /sbin/reboot

upgrade: compile .principal-ip
	# TODO C'est horriiiiiiible de faire ça comme ça ^^
	rsync rsync -e 'ssh -l root -i id_rsa' -a --chown root:root buildroot/output/target/ $$(cat .principal-ip):/

.PHONY: compile upload reboot
