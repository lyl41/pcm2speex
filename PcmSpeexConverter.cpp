#define MS_CLASS "PcmSpeexConverter"
#include "Logger.hpp"
#include "cluster/utils/PcmSpeexConverter.h"

PcmSpeexConverter::PcmSpeexConverter(AudioConstants::SampleRate sr, AudioConstants::ChannelCount cc)
{
	this->create_obj_success = false;

    MS_ASSERT(cc == AudioConstants::CHANNEL_COUNT_MONO, "Only mono audio is supported yet.");
    MS_ASSERT(sr == AudioConstants::SAMPLE_RATE_16000, "Only audio of sample rate 16000 is supported yet.");

    speex_bits_init(&(this->enc_bits));
    //宽带模式
    enc_state = speex_encoder_init(&speex_wb_mode);
	MS_ASSERT(enc_state != NULL, "speex_encoder_init return NULL, fail to create encode state.")
    int quality = 6;
    speex_encoder_ctl(enc_state, SPEEX_SET_QUALITY, &quality);
	speex_encoder_ctl(enc_state, SPEEX_GET_FRAME_SIZE, &frame_size);
//    speex_encoder_ctl(enc_state, SPEEX_SET_SAMPLING_RATE, &sr); //TODO sample rate?

	this->create_obj_success = true;

}

PcmSpeexConverter::operator bool() const
{
	return this->create_obj_success;
}

bool PcmSpeexConverter::operator!() const
{
	return !(this->create_obj_success);
}

PcmSpeexConverter::~PcmSpeexConverter()
{
	this->create_obj_success = false;
    speex_bits_destroy(&enc_bits);
    speex_encoder_destroy(enc_state);
	enc_state = nullptr;
}

int PcmSpeexConverter::Encode(short *pcm, int frames_per_packet, int size, void *speex, int speexBufferSize)
{
    if(pcm == nullptr || frames_per_packet <= 0 || size <= 0 || speex == nullptr || speexBufferSize <= 0) {
        return -1;
    }
	speex_bits_reset(&enc_bits);

	int offset = 0;
	while(frames_per_packet--) {
		speex_encode_int(enc_state, pcm + offset, &enc_bits); //TODO 
		offset += frame_size;
	}

    int enc_bit_size = speex_bits_nbytes(&enc_bits);
    enc_bit_size = speexBufferSize < enc_bit_size ? speexBufferSize : enc_bit_size;

    int enc_size = speex_bits_write(&enc_bits, (char *)speex, enc_bit_size);

    return enc_size < 0 ? -1 : enc_size;
}
