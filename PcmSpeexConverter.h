
#ifndef PROJECT_PCMSPEEXCONVERTER_H
#define PROJECT_PCMSPEEXCONVERTER_H
#define DEFAULT_FRAME_SIZE 320


#include "cluster/utils/AudioConstants.h"
#include "speex/speex.h"


class PcmSpeexConverter {
public:
	PcmSpeexConverter(AudioConstants::SampleRate sr, AudioConstants::ChannelCount cc);

	~PcmSpeexConverter();

	/**
	 * 该对象构造成功返回 false， 否则返回true
	 * @return
	 */
	bool operator!() const;

	/**
	 * 与 ！操作相反， 该对象构造成功返回 true，否则返回false
	 * @return
	 */
	explicit operator bool () const;

	/**
	 * 编码一帧speex数据
	 * @param pcm pcm数据
	 * @param size pcm数据长度
	 * @param speex 输出的speex数据
	 * @param speexBufferSize 输出buffer的容量大小
	 * @return 输出speex的实际长度，如果出错返回-1
	 */
	int Encode(short *pcm, int frame_per_packet, int size, void *speex, int speexBufferSize);


	AudioConstants::ChannelCount GetChannelCount() const { return mChannelCount; }

private:
	void * enc_state;
	SpeexBits enc_bits;
	bool create_obj_success;
	int frame_size;

	AudioConstants::ChannelCount mChannelCount;
};


#endif //PROJECT_PCMSPEEXCONVERTER_H
