#!/bin/sh

find_zynq_base_gpio () {
	for i in /sys/class/gpio/gpiochip*; do
		if [ "zynq_gpio" = `cat $i/label` ]; then
			return `echo $i | sed 's/^[^0-9]\+//'`
			break
		fi
	done
	return -1
}


#Setup 8 Profiles 10MHz spaced
for i in `seq 0 7`
do
  iio_attr -u ip:192.168.86.45 -c ad9361-phy TX_LO frequency $((2400000000 + $i * 1000000))  
  #echo $((2400000000 + $i * 10000000)) > out_altvoltage1_TX_LO_frequency
  echo "Initializing PROFILE $i at $((2400000000 + $i * 1000000)) MHz"
  iio_attr -u ip:192.168.86.45 -c ad9361-phy TX_LO fastlock_store $i
  #echo $i > out_altvoltage1_TX_LO_fastlock_store
  sleep 1
done

#Enable Fastlock Mode
iio_attr -u ip:192.168.86.45 -D ad9361-phy adi,tx-fastlock-pincontrol-enable 1
iio_attr -u ip:192.168.86.45 -c ad9361-phy TX_LO fastlock_recall 0


