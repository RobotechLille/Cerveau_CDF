################################################################################
#
# cdf
#
################################################################################
CDF2017P_VERSION = 1.0
CDF2017P_SITE = $(BR2_EXTERNAL_CDF_PATH)/package/robotech/cdf2017p/code
CDF2017P_SITE_METHOD = local

define CDF2017P_BUILD_CMDS
	$(MAKE) -C $(@D) clean
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D) all
endef

define CDF2017P_INSTALL_TARGET_CMDS
	$(INSTALL) -d -m 0755 $(TARGET_DIR)/opt/cdf/bin
	$(INSTALL) -d -m 0755 $(TARGET_DIR)/opt/cdf/com
	$(INSTALL) -D -m 0755 $(@D)/bin/* $(TARGET_DIR)/opt/cdf/bin
	$(INSTALL) -D -m 0755 $(@D)/run.sh $(TARGET_DIR)/opt/cdf
endef

$(eval $(generic-package))
