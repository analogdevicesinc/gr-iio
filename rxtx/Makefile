TARGETS := fmcomms_transceiver

.PHONY: all
all: $(TARGETS)

fmcomms_transceiver: iio_transceiver.py
	mv $< $@

iio_transceiver.py: transceiver.grc .build_encoder .build_decoder
	grcc -d . $<

.build_encoder: encoder.grc
	grcc $<
	touch $@

.build_decoder: decoder.grc
	grcc $<
	touch $@

.PHONY: clean
clean:
	rm -f $(TARGETS) .build_encoder .build_decoder
