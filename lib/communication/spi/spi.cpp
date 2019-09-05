/***************************************************************************//**
 *   @file   spi.c
 *   @author DBogdan (dragos.bogdan@analog.com)
********************************************************************************
 * Copyright 2019(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/

#include "spi.h"
#include <thread>
#include <libm2k/m2k.hpp>
#include <libm2k/contextbuilder.hpp>
#include <libm2k/digital/m2kdigital.hpp>
#include <libm2k/analog/m2khardwaretrigger.hpp>

#define CLOCK 0
#define MOSI 1
#define MISO 2

/******************************************************************************/
/************************** Global Variables **********************************/
/******************************************************************************/

static libm2k::devices::M2k *context;
static libm2k::digital::M2kDigital *m2k_digital;

/******************************************************************************/
/************************ Functions Definitions *******************************/
/******************************************************************************/

static void set_output_channel(int channel_index, libm2k::digital::M2kDigital *digital)
{
    digital->setDirection(channel_index, libm2k::digital::DIO_OUTPUT);
    digital->enableChannel(channel_index, true);
}


static void set_input_channel(int channel_index, libm2k::digital::M2kDigital *digital)
{
    digital->setDirection(channel_index, libm2k::digital::DIO_INPUT);
    digital->enableChannel(channel_index, true);
}


static int get_valid_samplerate(int frequency)
{
    return 10000000;
}

static void set_bit(unsigned short *number, unsigned int index)
{
    (*number) |= (1 << index);
}

static void set_bit(uint8_t *number, int index)
{
    (*number) |= (1 << index);
}

static void clear_bit(uint8_t *number, int index)
{
    (*number) &= ~(1 << index);
}

static bool get_bit(unsigned short number, int index)
{
    return (number & ( 1 << index )) >> index;
}

static bool get_bit(uint8_t number, int index)
{
    return (number & ( 1 << index )) >> index;
}



static std::vector<unsigned short> create_buffer(struct spi_desc *desc,
                                                 uint8_t *data,
                                                 uint8_t bytes_number)
{
    std::vector<unsigned short> bufferOut;
    unsigned int samples_per_bit = (m2k_digital->getSampleRateOut()/desc->max_speed_hz);

    bool clock_polarity = (desc->mode & 2) >> 1;
    bool phase = (desc->mode & 1);
    int cnt = 7;
    int data_index = 0;

    for(int i = 0; i < bytes_number*16; ++i) {
        if(cnt == -1)
        {
            cnt = 7;
            data_index++;
        }

        for (unsigned int j = 0; j < samples_per_bit/2; ++j) {

            unsigned short sample = 0;

            //set clock polarity
            sample |= clock_polarity;

            //write data
            if (get_bit(data[data_index], cnt)) {
                set_bit(&sample, 1);
            }

            bufferOut.push_back(sample);
        }
        if(phase)
        {
            if(i%2 == 0 && i > 0)
            {
                cnt--;

            }
        }else{
            if(i%2 != 0)
            {
                cnt--;
            }
        }

        //change polarity
        clock_polarity = !clock_polarity;
    }

    //stop transmission - set CS to high
    for(int i = 0; i < 2; ++i)
    {
        for (unsigned int j = 0; j < samples_per_bit/2; ++j) {
            unsigned short sample;
            if(i < 1)
            {
                sample = 0;
            }else{
                sample = 0x0008;
            }
            sample |= clock_polarity;
            bufferOut.push_back(sample);
        }
    }
    return bufferOut;
}

static void process_samples(struct spi_desc *desc,
                            uint8_t *data,
                            uint8_t bytes_number,
                            std::vector<unsigned short> &samples)
{
    int cnt = 7;
    int data_index = 0;
    unsigned short previous_sample = samples.at(0);
    for(auto it = samples.begin()+1; it != samples.end(); ++it)
    {
        if(data_index == bytes_number)
            break;
        if(cnt == -1)
        {
            cnt = 7;
            data_index++;
        }
        //raising edge
        if(desc->mode == 0 || desc->mode == 3) {
            if (get_bit(previous_sample, CLOCK) < get_bit((*it), CLOCK)) {
                if(get_bit(*it, MISO) == 0)
                {
                    clear_bit(&data[data_index], cnt);
                }else{
                    set_bit(&data[data_index], cnt);
                }
                cnt--;
            }
            //falling edge
        }else{
            if (get_bit(previous_sample, CLOCK) > get_bit((*it), CLOCK)) {
                if(get_bit(*it, MISO) == 0)
                {
                    clear_bit(&data[data_index], cnt);
                }else{
                    set_bit(&data[data_index], cnt);
                }
                cnt--;
            }
        }
        previous_sample = (*it);
    }
}

static void read(struct spi_desc *desc,
                 uint8_t *data,
                 uint8_t bytes_number)
{
    //get samples
    unsigned int samples_per_bit = (m2k_digital->getSampleRateIn()/desc->max_speed_hz);

    //set the trigger on CS
    libm2k::analog::M2kHardwareTrigger *trigger = m2k_digital->getTrigger();
    trigger->setDigitalCondition(desc->chip_select, libm2k::analog::FALLING_EDGE);
    std::vector<unsigned short> samples = m2k_digital->getSamples((bytes_number*8)*samples_per_bit);

    //process samples
    process_samples(desc, data, bytes_number, samples);
}

static void write(std::vector<unsigned short> &buffer)
{
    m2k_digital->setCyclic(false);
    m2k_digital->push(buffer);
}

/**
 * @brief Initialize the SPI communication peripheral.
 * @param desc - The SPI descriptor.
 * @param init_param - The structure that contains the SPI parameters.
 * @return SUCCESS in case of success, FAILURE otherwise.
 */
int32_t spi_init(struct spi_desc **desc,
                 const struct spi_init_param *param)
{
    // initialize the attributes
    (*desc) = new spi_desc;
    (*desc)->max_speed_hz = param->max_speed_hz;
    (*desc)->chip_select = param->chip_select;
    (*desc)->mode = param->mode;
    (*desc)->extra = param->extra;

    int sample_rate = get_valid_samplerate((*desc)->max_speed_hz);

    //m2k connection
    if((*desc)->extra == nullptr)
    {
        context = libm2k::devices::m2kOpen();
    }else{
        context = libm2k::devices::m2kOpen((const char*)(*desc)->extra);
    }
    m2k_digital = context->getDigital();

    //set sampling frequencies
    m2k_digital->setSampleRateOut(sample_rate);
    m2k_digital->setSampleRateIn(2*sample_rate);

    //enable the channels
    set_output_channel((*desc)->chip_select, m2k_digital);
    set_output_channel(CLOCK, m2k_digital);
    set_output_channel(MOSI, m2k_digital);
    set_input_channel(MISO, m2k_digital);

    //set the state of CS and SCLK to idle
    m2k_digital->setValueRaw((*desc)->chip_select, libm2k::digital::HIGH);
    if((*desc)->mode >> 1)
        m2k_digital->setValueRaw(CLOCK, libm2k::digital::HIGH);
    else
        m2k_digital->setValueRaw(CLOCK, libm2k::digital::LOW);
    return 0;
}

/**
 * @brief Free the resources allocated by spi_init().
 * @param desc - The SPI descriptor.
 * @return SUCCESS in case of success, FAILURE otherwise.
 */
int32_t spi_remove(struct spi_desc *desc)
{
    delete desc;
    desc = nullptr;
    deviceClose(context, false);
    return 0;
}

/**
 * @brief Write and read data to/from SPI.
 * @param desc - The SPI descriptor.
 * @param data - The buffer with the transmitted/received data.
 * @param bytes_number - Number of bytes to write/read.
 * @return SUCCESS in case of success, FAILURE otherwise.
 */
int32_t spi_write_and_read(struct spi_desc *desc,
                           uint8_t *data,
                           uint8_t bytes_number)
{
    std::thread thread_read(read, desc, data, bytes_number);
    std::vector<unsigned short> buffer = create_buffer(desc, data, bytes_number);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    write(buffer);
    thread_read.join();
    return 0;
}
