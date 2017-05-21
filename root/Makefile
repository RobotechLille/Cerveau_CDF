default: compile

upgrade-all: upgrade-arduinos upgrade-filesystem

# CONSTANTES

# Périphérique bloc à utiliser pour flasher
SDCARD=/dev/mmcblk0

# Liste des programmes Arduinos
PROGS_ARDUINO=ardPince

# SYSTÈME D'EXPLOITATION

# Configuration
buildroot/.config: configs/cdf2017p_defconfig
	make -C buildroot BR2_EXTERNAL=.. cdf2017p_defconfig

configure: buildroot/.config

# Compile l'OS (ça prend quelques heures)
compile: configure
	make -C buildroot target-finalize

# Supprime tous les fichiers de l'OS
clean:
	make -C buildroot clean

# Dernier recours en cas de compilation d'OS foireuse
# (faut être patient)
recompile: clean compile

# Flashe l'image sur la carte SD
flash: compile $(SDCARD)
	dd if=buildroot/output/images/sdcard.img of=$(SDCARD) bs=4M
	sync
	echo -e "resizepart 2\n100%\nquit" | parted $(SDCARD)
	sync
	resize2fs $(SDCARD)*2
	sync

# Graphiques (parce que c'est rigolo)
graphs:
	make -C buildroot graph-{size,build,depends{,-requirements}}

# CONNEXION AU ROBOT

# Il vous faudra pour ces actions le fichier principalconf.sh

# Crée un fichier de conf utilisable pour s'y connecter
sshconf: principalconf.sh
	source $$PWD/$<; echo -e "Host principal p\n    User root\n    Hostname $$IP\n    PreferredAuthentications publickey\n    PubkeyAuthentication yes\n    IdentityFile \"$$PWD/sshkey\"" > "$@"
	source $$PWD/$<; echo -e "$$SSHCPRV" > sshkey
	chmod 600 sshkey

# Lance une connexion SSH, tout bêtement
ssh: sshconf
	ssh -F sshconf principal

# Redémarre le robot
reboot: sshconf
	ssh -F sshconf principal /sbin/reboot

# Met le robot à l'heure (jusqu'au prochain redémarrage)
# (ce qui est une mauvaise idée parce que du coup rsync
# n'écrase pas les fichiers modifiés directement sur le Pi)
#date: sshconf
#	ssh -F sshconf principal "date -s $(date +%Y%m%d%H%M.%S)"

# Met à jour le Raspberry Pi (par rapport à ce dépôt)
upgrade-filesystem: sshconf configure
	make -C buildroot target-finalize
	@# TODO C'est horriiiiiiible de faire ça comme ça ^^
	rsync -e 'ssh -F sshconf' -a --chown root:root buildroot/output/target/ principal:/

# ARDUINOS

../ard%/build/main.hex: ../ard%
	make -C $<

ard%.up: ../ard%/build/main.hex
	scp -F sshconf -q "$<" principal:/tmp/$(basename $@).hex
	ssh -F sshconf principal "avrdude $(shell make --quiet -C ../$(basename $@) ARDUINO_QUIET=true FORCE_MONITOR_PORT=y TARGET_HEX=/tmp/$(basename $@).hex MONITOR_PORT=/opt/cdf/com/$(basename $@) exportOpts) "

upgrade-arduinos: $(addsuffix .up,$(PROGS_ARDUINO))

