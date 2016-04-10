#########################
###    ENVIRONMENT    ###
#########################

TEST_TARGET := google.com:443

PERL := $(shell (command -v perl) 2>/dev/null)
SSL := $(shell (command -v openssl) 2>/dev/null)

OPENSSL_VERISON := $(shell $(SSL) version)
OPENSSL_CIPHERS := $(shell $(SSL) ciphers 'ALL:eNULL' | sed -e 's/:/ /g')

.DEFAULT_GOAL := help

#########################
###      OUTPUT       ###
#########################

COLOR = \
	use Term::ANSIColor; \
	printf("    %s %s\n", colored(["BOLD $$ARGV[0]"], "[$$ARGV[1]]"), join(" ", @ARGV[2..$$\#ARGV]));

HELP_FUNC = \
	%help; \
	while(<>) { \
		if(/^([a-z0-9_-]+):.*\#\#(?:@(\w+))?\s(.*)$$/) { \
			push(@{$$help{$$2}}, [$$1, $$3]); \
		} \
	}; \
	print "usage: make [target]\n\n"; \
	for ( sort keys %help ) { \
		print "$$_:\n"; \
		printf("  %-20s %s\n", $$_->[0], $$_->[1]) for @{$$help{$$_}}; \
		print "\n"; \
	}

#########################
###     MISC          ###
#########################

.PHONY: help scan

help:           ##@miscellaneous Show this help
	@echo ""
	@echo '  _____ ____    ____ ____  _       ____        _ _        '
	@echo ' |  ___| ___|  / ___/ ___|| |     / ___| _   _(_) |_ ___   '
	@echo ' | |_  |___ \  \___ \___ \| |     \___ \| | | | | __/ _ \  '
	@echo ' |  _|  ___) |  ___) |__) | |___   ___) | |_| | | ||  __/  '
	@echo ' |_|   |____/  |____/____/|_____| |____/ \__,_|_|\__\___|  '
	@echo ""
	@$(PERL) -e '$(HELP_FUNC)' $(MAKEFILE_LIST)

scan:           ##@cipher Scan target for allowed ciphers
	@echo "Testing available cipher suite on $(TEST_TARGET)..."
	@for CIPHER in $(OPENSSL_CIPHERS); do \
		echo -n | $(SSL) s_client -cipher "$$CIPHER" -connect $(TEST_TARGET) 2>&1 | grep -q ":error"; \
		if [ "$$?" != "0" ] ; then \
			$(PERL) -e '$(COLOR)' "GREEN" "PASS" "$$CIPHER"; \
		else \
			$(PERL) -e '$(COLOR)' "RED" "FAIL" "$$CIPHER"; \
		fi; \
	done

list:           ##@cipher List available system ciphers
	@echo "Supported openssl ciphers on this system..."
	@for CIPHER in $(OPENSSL_CIPHERS); do printf "    %b\n" $$CIPHER; done

print-%:
	@echo '$*=$($*)'
	@echo '  origin = $(origin $*)'
	@echo '  flavor = $(flavor $*)'
	@echo '   value = $(value  $*)'
